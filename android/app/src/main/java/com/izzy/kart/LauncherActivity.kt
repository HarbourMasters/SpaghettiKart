package com.izzy.kart

import android.content.Intent
import android.graphics.Color
import android.net.Uri
import android.os.Bundle
import android.view.Gravity
import android.view.View
import android.view.ViewGroup
import android.view.WindowManager
import android.widget.Button
import android.widget.LinearLayout
import android.widget.ProgressBar
import android.widget.ScrollView
import android.widget.TextView
import androidx.activity.ComponentActivity
import androidx.activity.result.contract.ActivityResultContracts
import java.io.File
import java.io.FileOutputStream
import kotlin.concurrent.thread

/**
 * First-run flow: unpack the bundled game files, take a ROM from the user, and
 * turn it into mk64.o2r before handing off to the game.
 *
 * Nothing about the game is shipped in the APK — the ROM stays on the device
 * and the extraction runs here, on demand.
 */
class LauncherActivity : ComponentActivity() {

    private lateinit var status: TextView
    private lateinit var chooseButton: Button
    private lateinit var progress: ProgressBar

    private val pickRom =
        registerForActivityResult(ActivityResultContracts.OpenDocument()) { uri: Uri? ->
            if (uri != null) importRom(uri)
        }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        buildUi()
        prepare()
    }

    /**
     * Unpacks the bundled game files, then decides what still has to happen:
     * nothing (start the game), extract an already-imported ROM, or ask for one.
     *
     * The unpacking moves tens of megabytes out of the APK, so it cannot run on
     * the main thread.
     */
    private fun prepare() {
        setBusy(true, getString(R.string.launcher_preparing))

        thread(name = "mk64-prepare") {
            val stagingError = GameAssets.stageBundledAssets(this)

            runOnUiThread {
                when {
                    // Nothing else will work without the bundled files, so
                    // leave the button disabled rather than invite a retry.
                    stagingError != null -> {
                        setBusy(false, stagingError)
                        chooseButton.isEnabled = false
                    }
                    GameAssets.isExtracted(this) -> startGame()
                    GameAssets.romFile(this).isFile -> extractRom()
                    else -> setBusy(false, romInstructions)
                }
            }
        }
    }

    private fun buildUi() {
        val padding = (24 * resources.displayMetrics.density).toInt()
        val root = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            gravity = Gravity.CENTER
            setPadding(padding, padding, padding, padding)
            setBackgroundColor(Color.rgb(8, 16, 28))
        }

        status = TextView(this).apply {
            setTextColor(Color.WHITE)
            textSize = 16f
            gravity = Gravity.CENTER
        }
        chooseButton = Button(this).apply {
            text = getString(R.string.launcher_choose_rom)
            setOnClickListener { pickRom.launch(arrayOf("*/*")) }
        }
        progress = ProgressBar(this).apply { visibility = View.GONE }

        // The failure messages can run long, and the launcher is fixed to
        // landscape, so let the text scroll rather than push the button away.
        val scroller = ScrollView(this).apply {
            addView(
                status,
                ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT)
            )
        }

        root.addView(scroller, LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 0, 1f))
        root.addView(chooseButton)
        root.addView(progress)
        setContentView(root)
    }

    private fun importRom(uri: Uri) {
        setBusy(true, getString(R.string.launcher_verifying))

        thread(name = "mk64-rom-import") {
            val result = runCatching {
                val target = GameAssets.romFile(this)
                val partial = File(target.parentFile, "${GameAssets.ROM_NAME}.part")
                partial.delete()

                contentResolver.openInputStream(uri).use { input ->
                    requireNotNull(input) { "The document provider did not open the file." }
                    FileOutputStream(partial).use { output ->
                        input.copyTo(output, COPY_BUFFER_BYTES)
                        output.fd.sync()
                    }
                }

                val sha1 = GameAssets.sha1Of(partial)
                if (sha1 != GameAssets.ROM_SHA1) {
                    partial.delete()
                    error(
                        "That is not the supported Mario Kart 64 (USA) ROM.\n\n" +
                            "Expected SHA-1 ${GameAssets.ROM_SHA1}\nGot      $sha1"
                    )
                }

                if (target.exists() && !target.delete()) {
                    error("Could not replace the previous ROM.")
                }
                check(partial.renameTo(target)) { "Could not store the ROM in the app's files directory." }
            }

            runOnUiThread {
                result.fold(
                    onSuccess = { extractRom() },
                    onFailure = {
                        setBusy(false, "${it.message ?: it}\n\n$romInstructions")
                    }
                )
            }
        }
    }

    private fun extractRom() {
        setBusy(true, getString(R.string.launcher_extracting))

        thread(name = "mk64-extract") {
            val error = GameAssets.generateGameArchive(this)

            runOnUiThread {
                if (error == null) {
                    startGame()
                } else {
                    setBusy(false, "$error\n\n$romInstructions")
                }
            }
        }
    }

    /**
     * Both long-running steps keep the screen awake: the extraction has no
     * progress to report and can outlast the display timeout, and being killed
     * part-way through leaves a truncated archive behind.
     */
    private fun setBusy(busy: Boolean, message: String) {
        status.text = message
        chooseButton.isEnabled = !busy
        progress.visibility = if (busy) View.VISIBLE else View.GONE
        if (busy) {
            window.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON)
        } else {
            window.clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON)
        }
    }

    private fun startGame() {
        startActivity(Intent(this, MainActivity::class.java))
        finish()
    }

    /** Spelled out from the real path so debug builds don't point at the release one. */
    private val romInstructions: String
        get() = "Select your own Mario Kart 64 (USA) .z64 ROM. It is verified, kept in this app's " +
            "private storage and converted into the game's asset archive on this device — no " +
            "game data ships with the app.\n\n" +
            "Mods go in ${GameAssets.modsDir(this)}."

    private companion object {
        const val COPY_BUFFER_BYTES = 1 shl 17
    }
}
