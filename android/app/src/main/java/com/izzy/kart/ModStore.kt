package com.izzy.kart

import android.content.Context
import android.net.Uri
import android.provider.DocumentsContract
import android.util.Log
import java.io.File
import java.io.FileOutputStream
import java.io.IOException
import java.util.zip.ZipEntry
import java.util.zip.ZipOutputStream

/**
 * The mods folder, as the engine sees it.
 *
 * ModManager loads everything in `mods/` sorted by path, and a later archive
 * overrides an earlier one. So load order is filename order, and reordering
 * means renaming: every mod carries a `NNN_` prefix that this class owns and
 * hides from the user.
 *
 * A mod is one of three things, all of which the engine accepts:
 *   - an `.o2r` archive
 *   - a `.zip` archive
 *   - a plain folder
 *
 * Renaming anything to end in `.disabled` makes ModManager skip it, which is
 * how the on/off state is stored.
 */
object ModStore {

    private const val TAG = "ModStore"
    private const val DISABLED_SUFFIX = ".disabled"
    private val ORDER_PREFIX = Regex("""^\d{3}_""")

    data class Mod(
        val file: File,
        /** Filename with the order prefix and disabled marker stripped off. */
        val name: String,
        val enabled: Boolean,
        val isFolder: Boolean
    ) {
        val kind: String
            get() = when {
                isFolder -> "folder"
                name.endsWith(".zip", ignoreCase = true) -> "zip"
                else -> "o2r"
            }
    }

    /** Mods in load order: the last entry wins where two mods touch the same file. */
    fun list(context: Context): List<Mod> {
        val dir = GameAssets.modsDir(context)
        dir.mkdirs()

        return (dir.listFiles() ?: emptyArray())
            .filter { it.name != "place_mods_here.txt" }
            .sortedBy { it.name }
            .map { file ->
                val enabled = !file.name.endsWith(DISABLED_SUFFIX, ignoreCase = true)
                var name = file.name.removePrefix(ORDER_PREFIX.find(file.name)?.value ?: "")
                if (!enabled) {
                    name = name.dropLast(DISABLED_SUFFIX.length)
                }
                Mod(file, name, enabled, file.isDirectory)
            }
    }

    fun setEnabled(mod: Mod, enabled: Boolean): String? {
        if (mod.enabled == enabled) return null

        val target = if (enabled) {
            File(mod.file.parentFile, mod.file.name.dropLast(DISABLED_SUFFIX.length))
        } else {
            File(mod.file.parentFile, mod.file.name + DISABLED_SUFFIX)
        }
        return if (mod.file.renameTo(target)) null else "Could not rename ${mod.name}."
    }

    fun delete(mod: Mod): String? =
        if (mod.file.deleteRecursively()) null else "Could not delete ${mod.name}."

    /**
     * Rewrites the order prefixes so the mods load in the given order.
     *
     * Done in two passes through temporary names, because the new name of one
     * mod is frequently the current name of another.
     */
    fun applyOrder(mods: List<Mod>): String? {
        val staged = mutableListOf<Pair<File, String>>()

        mods.forEachIndexed { index, mod ->
            val bare = mod.file.name.removePrefix(ORDER_PREFIX.find(mod.file.name)?.value ?: "")
            val finalName = "%03d_%s".format(index, bare)
            val temporary = File(mod.file.parentFile, ".reorder_$index")

            if (mod.file.name == finalName) return@forEachIndexed
            if (!mod.file.renameTo(temporary)) {
                return "Could not reorder ${mod.name}."
            }
            staged += temporary to finalName
        }

        for ((temporary, finalName) in staged) {
            if (!temporary.renameTo(File(temporary.parentFile, finalName))) {
                return "Could not finish reordering; the mods folder may need a look."
            }
        }
        return null
    }

    /** Copies an `.o2r`/`.zip` picked through the document picker into the mods folder. */
    fun importArchive(context: Context, uri: Uri): String? {
        val displayName = queryDisplayName(context, uri) ?: return "Could not read the file's name."
        if (!displayName.endsWith(".o2r", true) && !displayName.endsWith(".zip", true)) {
            return "$displayName is not a mod. Mods are .o2r or .zip files, or folders."
        }

        val target = nextFreeFile(context, displayName)
        return try {
            context.contentResolver.openInputStream(uri).use { input ->
                requireNotNull(input) { "The document provider did not open $displayName." }
                FileOutputStream(target).use { output -> input.copyTo(output, COPY_BUFFER) }
            }
            null
        } catch (error: Exception) {
            target.delete()
            Log.e(TAG, "Import of $displayName failed", error)
            "Could not import $displayName: ${error.message ?: error}"
        }
    }

    /** Copies a folder picked through the tree picker into the mods folder. */
    fun importFolder(context: Context, treeUri: Uri): String? {
        val documentId = DocumentsContract.getTreeDocumentId(treeUri)
        val rootUri = DocumentsContract.buildDocumentUriUsingTree(treeUri, documentId)
        val displayName = queryDisplayName(context, rootUri) ?: documentId.substringAfterLast('/')

        // ModManager reads everything after the last dot as an extension and
        // ignores folders whose "extension" it does not know, so a folder named
        // "Cool.Mod v2" would silently never load.
        val safeName = displayName.replace('.', '_')
        val target = nextFreeFile(context, safeName)

        return try {
            copyTree(context, rootUri, treeUri, target)
            null
        } catch (error: Exception) {
            target.deleteRecursively()
            Log.e(TAG, "Import of folder $displayName failed", error)
            "Could not import $displayName: ${error.message ?: error}"
        }
    }

    /** Writes a mod back out to a location the user picked. Folders are zipped. */
    fun export(context: Context, mod: Mod, destination: Uri): String? {
        return try {
            context.contentResolver.openOutputStream(destination).use { output ->
                requireNotNull(output) { "The document provider did not open the destination." }
                if (mod.isFolder) {
                    ZipOutputStream(output.buffered()).use { zip -> zipTree(mod.file, mod.file, zip) }
                } else {
                    mod.file.inputStream().use { input -> input.copyTo(output, COPY_BUFFER) }
                }
            }
            null
        } catch (error: Exception) {
            Log.e(TAG, "Export of ${mod.name} failed", error)
            "Could not export ${mod.name}: ${error.message ?: error}"
        }
    }

    /** A folder mod leaves as a .zip, which the engine loads just the same. */
    fun exportName(mod: Mod): String = if (mod.isFolder) "${mod.name}.zip" else mod.name

    private const val COPY_BUFFER = 1 shl 16

    private fun nextFreeFile(context: Context, name: String): File {
        val dir = GameAssets.modsDir(context)
        dir.mkdirs()

        // Land at the end of the load order; the user can move it from there.
        val order = list(context).size
        var candidate = File(dir, "%03d_%s".format(order, name))
        var attempt = 1
        while (candidate.exists()) {
            candidate = File(dir, "%03d_%d_%s".format(order, attempt, name))
            attempt++
        }
        return candidate
    }

    private fun queryDisplayName(context: Context, uri: Uri): String? =
        context.contentResolver.query(uri, arrayOf(DocumentsContract.Document.COLUMN_DISPLAY_NAME), null, null, null)
            ?.use { cursor -> if (cursor.moveToFirst()) cursor.getString(0) else null }

    private fun copyTree(context: Context, documentUri: Uri, treeUri: Uri, target: File) {
        target.mkdirs()

        val childrenUri = DocumentsContract.buildChildDocumentsUriUsingTree(
            treeUri,
            DocumentsContract.getDocumentId(documentUri)
        )
        val projection = arrayOf(
            DocumentsContract.Document.COLUMN_DOCUMENT_ID,
            DocumentsContract.Document.COLUMN_DISPLAY_NAME,
            DocumentsContract.Document.COLUMN_MIME_TYPE
        )

        context.contentResolver.query(childrenUri, projection, null, null, null)?.use { cursor ->
            while (cursor.moveToNext()) {
                val childId = cursor.getString(0)
                val childName = cursor.getString(1)
                val isDirectory = cursor.getString(2) == DocumentsContract.Document.MIME_TYPE_DIR
                val childUri = DocumentsContract.buildDocumentUriUsingTree(treeUri, childId)
                val childTarget = File(target, childName)

                if (isDirectory) {
                    copyTree(context, childUri, treeUri, childTarget)
                } else {
                    context.contentResolver.openInputStream(childUri).use { input ->
                        if (input == null) throw IOException("Could not read $childName.")
                        FileOutputStream(childTarget).use { output -> input.copyTo(output, COPY_BUFFER) }
                    }
                }
            }
        }
    }

    private fun zipTree(root: File, current: File, zip: ZipOutputStream) {
        for (child in current.listFiles() ?: emptyArray()) {
            val relative = child.relativeTo(root).path
            if (child.isDirectory) {
                zip.putNextEntry(ZipEntry("$relative/"))
                zip.closeEntry()
                zipTree(root, child, zip)
            } else {
                zip.putNextEntry(ZipEntry(relative))
                child.inputStream().use { input -> input.copyTo(zip, COPY_BUFFER) }
                zip.closeEntry()
            }
        }
    }
}
