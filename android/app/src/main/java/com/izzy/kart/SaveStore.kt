package com.izzy.kart

import android.content.Context
import android.net.Uri
import android.provider.DocumentsContract
import android.util.Log
import java.io.File
import java.io.FileOutputStream
import java.io.IOException
import java.util.zip.ZipEntry
import java.util.zip.ZipInputStream
import java.util.zip.ZipOutputStream

/**
 * The game's save files.
 *
 * Everything the engine saves is a `.sav` sitting directly in the app
 * directory: `default.sav` is the 512-byte EEPROM holding records, ghosts and
 * settings, and the Controller Pak lives in `controllerPak_header.sav` plus a
 * `controllerPak_file_N.sav` per slot.
 *
 * They are matched by extension rather than by name so a new save file added
 * upstream is picked up without changes here.
 */
object SaveStore {

    private const val TAG = "SaveStore"
    private const val COPY_BUFFER = 1 shl 16

    const val BUNDLE_NAME = "spaghettikart-saves.zip"

    data class Save(val file: File) {
        val label: String
            get() = when {
                file.name == "default.sav" -> "Game save"
                file.name == "controllerPak_header.sav" -> "Controller Pak header"
                file.name.startsWith("controllerPak_file_") ->
                    "Controller Pak slot ${file.name.removePrefix("controllerPak_file_").removeSuffix(".sav")}"
                else -> file.name
            }
    }

    fun list(context: Context): List<Save> =
        (GameAssets.gameDir(context).listFiles() ?: emptyArray())
            .filter { it.isFile && it.name.endsWith(".sav", ignoreCase = true) }
            .sortedBy { it.name }
            .map { Save(it) }

    fun delete(save: Save): String? =
        if (save.file.delete()) null else "Could not delete ${save.label}."

    fun export(context: Context, save: Save, destination: Uri): String? = runOrReport("export") {
        context.contentResolver.openOutputStream(destination).use { output ->
            requireNotNull(output) { "The document provider did not open the destination." }
            save.file.inputStream().use { input -> input.copyTo(output, COPY_BUFFER) }
        }
    }

    fun exportAll(context: Context, destination: Uri): String? = runOrReport("export") {
        val saves = list(context)
        check(saves.isNotEmpty()) { "There are no saves to export yet." }

        context.contentResolver.openOutputStream(destination).use { output ->
            requireNotNull(output) { "The document provider did not open the destination." }
            ZipOutputStream(output.buffered()).use { zip ->
                for (save in saves) {
                    zip.putNextEntry(ZipEntry(save.file.name))
                    save.file.inputStream().use { input -> input.copyTo(zip, COPY_BUFFER) }
                    zip.closeEntry()
                }
            }
        }
    }

    /**
     * Restores a single `.sav`, or every `.sav` out of a zip written by
     * [exportAll]. Existing files of the same name are replaced.
     */
    fun import(context: Context, uri: Uri): String? {
        val displayName = queryDisplayName(context, uri) ?: return "Could not read the file's name."

        return when {
            displayName.endsWith(".zip", true) -> importBundle(context, uri)
            displayName.endsWith(".sav", true) -> runOrReport("import") {
                context.contentResolver.openInputStream(uri).use { input ->
                    requireNotNull(input) { "The document provider did not open $displayName." }
                    FileOutputStream(File(GameAssets.gameDir(context), displayName)).use { output ->
                        input.copyTo(output, COPY_BUFFER)
                    }
                }
            }
            else -> "$displayName is not a save. Saves are .sav files, or a .zip of them."
        }
    }

    private fun importBundle(context: Context, uri: Uri): String? = runOrReport("import") {
        val target = GameAssets.gameDir(context)
        var restored = 0

        context.contentResolver.openInputStream(uri).use { input ->
            requireNotNull(input) { "The document provider did not open the archive." }
            ZipInputStream(input.buffered()).use { zip ->
                while (true) {
                    val entry = zip.nextEntry ?: break
                    val name = File(entry.name).name

                    // Only ever write a plain .sav straight into the game
                    // directory; an archive from elsewhere could otherwise
                    // carry entries like ../../ pointing out of it.
                    if (entry.isDirectory || !name.endsWith(".sav", true)) {
                        zip.closeEntry()
                        continue
                    }

                    FileOutputStream(File(target, name)).use { output -> zip.copyTo(output, COPY_BUFFER) }
                    zip.closeEntry()
                    restored++
                }
            }
        }

        check(restored > 0) { "That archive has no save files in it." }
    }

    private fun runOrReport(verb: String, work: () -> Unit): String? = try {
        work()
        null
    } catch (error: Exception) {
        Log.e(TAG, "Save $verb failed", error)
        "Could not $verb: ${error.message ?: error}"
    }

    private fun queryDisplayName(context: Context, uri: Uri): String? =
        context.contentResolver.query(uri, arrayOf(DocumentsContract.Document.COLUMN_DISPLAY_NAME), null, null, null)
            ?.use { cursor -> if (cursor.moveToFirst()) cursor.getString(0) else null }
}
