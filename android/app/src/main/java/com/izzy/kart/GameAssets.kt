package com.izzy.kart

import android.content.Context
import android.util.Log
import androidx.core.content.pm.PackageInfoCompat
import java.io.File
import java.io.FileOutputStream
import java.io.IOException
import java.security.MessageDigest

/**
 * Everything the game needs on disk, and how it gets there.
 *
 * libultraship resolves every runtime path on Android through
 * `SDL_AndroidGetExternalStoragePath()`, i.e. `getExternalFilesDir(null)`. That
 * is also somewhere the user can reach with a file manager, which is what makes
 * dropping mods in possible, so everything lives there:
 *
 *     Android/data/com.izzy.kart/files/
 *       baserom.us.z64      the user's ROM, copied in by the launcher
 *       config.yml, yamls/  Torch's extraction recipes, unpacked from the APK
 *       meta/mods.toml      packed into mk64.o2r as its mod manifest
 *       spaghetti.o2r       engine assets, built on CI and shipped in the APK
 *       mk64.o2r            generated on-device from the ROM
 *       mods/               user mods (.o2r, .zip or plain folders)
 */
object GameAssets {

    const val ROM_NAME = "baserom.us.z64"
    const val ROM_SHA1 = "579c48e211ae952530ffc8738709f078d5dd215e"

    private const val TAG = "GameAssets"
    private const val GAME_ARCHIVE = "mk64.o2r"
    private const val ENGINE_ARCHIVE = "spaghetti.o2r"
    private const val CONTROLLER_DB = "gamecontrollerdb.txt"
    private const val TORCH_HASHES = "torch.hash.yml"
    private const val STAMP = ".staged-assets"

    /** Recipes Torch reads while extracting; replaced whenever the APK changes. */
    private val BUNDLED_FILES = listOf("config.yml", ENGINE_ARCHIVE)
    private val BUNDLED_DIRECTORIES = listOf("yamls", "meta")

    fun gameDir(context: Context): File =
        (context.getExternalFilesDir(null) ?: context.filesDir).also { it.mkdirs() }

    fun romFile(context: Context) = File(gameDir(context), ROM_NAME)

    fun gameArchive(context: Context) = File(gameDir(context), GAME_ARCHIVE)

    fun modsDir(context: Context) = File(gameDir(context), "mods")

    fun isExtracted(context: Context) = gameArchive(context).length() > 0

    /**
     * Unpacks the APK-bundled extraction inputs into [gameDir].
     *
     * Re-runs whenever the installed package changes so an app update ships new
     * yamls and a new spaghetti.o2r without the user clearing data. Returns an
     * error message, or null when the directory is ready.
     */
    fun stageBundledAssets(context: Context): String? {
        val target = gameDir(context)
        val stamp = File(target, STAMP)
        val expected = installStamp(context)

        if (stamp.takeIf { it.isFile }?.readText() == expected) {
            return null
        }

        return try {
            for (name in BUNDLED_FILES) {
                copyAsset(context, name, File(target, name))
            }
            for (name in BUNDLED_DIRECTORIES) {
                copyAssetTree(context, name, File(target, name))
            }

            // Users are meant to edit this one, so only seed it once.
            val controllerDb = File(target, CONTROLLER_DB)
            if (!controllerDb.isFile) {
                copyAsset(context, CONTROLLER_DB, controllerDb)
            }

            val mods = modsDir(context)
            mods.mkdirs()
            if (mods.list().isNullOrEmpty()) {
                copyAssetTree(context, "mods", mods)
            }

            // New recipes mean the previous archive is stale, and Torch skips
            // work whose inputs it believes are unchanged.
            gameArchive(context).delete()
            File(target, TORCH_HASHES).delete()

            stamp.writeText(expected)
            null
        } catch (error: IOException) {
            Log.e(TAG, "Could not unpack the bundled assets", error)
            "Could not unpack the bundled game files: ${error.message}"
        }
    }

    /**
     * Runs Torch over [romFile] to produce mk64.o2r. Blocking and slow — expect
     * a minute or more on older hardware. Returns an error message, or null on
     * success.
     */
    fun generateGameArchive(context: Context): String? {
        val dir = gameDir(context)
        return nativeGenerateGameArchive(romFile(context).absolutePath, dir.absolutePath, dir.absolutePath)
    }

    fun sha1Of(file: File): String {
        val digest = MessageDigest.getInstance("SHA-1")
        file.inputStream().use { input ->
            val buffer = ByteArray(1 shl 17)
            while (true) {
                val read = input.read(buffer)
                if (read < 0) break
                digest.update(buffer, 0, read)
            }
        }
        return digest.digest().joinToString("") { "%02x".format(it) }
    }

    private fun installStamp(context: Context): String {
        val info = context.packageManager.getPackageInfo(context.packageName, 0)
        // lastUpdateTime moves on every reinstall, so development builds that
        // keep the same versionCode still pick up new assets.
        return "${PackageInfoCompat.getLongVersionCode(info)}:${info.lastUpdateTime}"
    }

    private fun copyAsset(context: Context, assetPath: String, target: File) {
        target.parentFile?.mkdirs()
        context.assets.open(assetPath).use { input ->
            FileOutputStream(target).use { output ->
                input.copyTo(output)
                output.fd.sync()
            }
        }
    }

    private fun copyAssetTree(context: Context, assetPath: String, target: File) {
        val children = context.assets.list(assetPath) ?: return
        if (children.isEmpty()) {
            copyAsset(context, assetPath, target)
            return
        }

        target.mkdirs()
        for (child in children) {
            copyAssetTree(context, "$assetPath/$child", File(target, child))
        }
    }

    private external fun nativeGenerateGameArchive(rom: String, sourceDir: String, destDir: String): String?

    init {
        // libSpaghettify.so carries both the game and the Torch extractor.
        System.loadLibrary("SDL2")
        System.loadLibrary("Spaghettify")
    }
}
