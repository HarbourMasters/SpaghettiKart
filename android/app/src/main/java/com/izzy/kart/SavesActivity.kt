package com.izzy.kart

import android.app.AlertDialog
import android.content.Intent
import android.graphics.Color
import android.graphics.Typeface
import android.net.Uri
import android.os.Bundle
import android.view.Gravity
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.LinearLayout
import android.widget.ScrollView
import android.widget.TextView
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.result.contract.ActivityResultContracts
import kotlin.concurrent.thread

/**
 * Save manager: back saves up, restore them, or clear one out.
 *
 * The running game rewrites its EEPROM whenever it saves, so anything restored
 * here is only safe once the process has been through a restart — hence the
 * warning and the restart button rather than a silent hand-off.
 */
class SavesActivity : ComponentActivity() {

    private lateinit var listContainer: LinearLayout
    private lateinit var emptyLabel: TextView
    private var dirty = false

    private val importSaves =
        registerForActivityResult(ActivityResultContracts.OpenMultipleDocuments()) { uris ->
            if (uris.isNullOrEmpty()) return@registerForActivityResult
            runInBackground("Importing…") { uris.mapNotNull { SaveStore.import(this, it) } }
        }

    /** Set just before the picker opens, since the contract carries only a filename. */
    private var pendingExport: SaveStore.Save? = null

    private val exportSave =
        registerForActivityResult(ActivityResultContracts.CreateDocument("application/octet-stream")) { uri ->
            val save = pendingExport
            pendingExport = null
            if (uri == null) return@registerForActivityResult

            runInBackground("Exporting…") {
                listOfNotNull(
                    if (save != null) SaveStore.export(this, save, uri) else SaveStore.exportAll(this, uri)
                )
            }
        }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        buildUi()
        refresh()
    }

    private fun buildUi() {
        val pad = dp(16)
        val root = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(pad, pad, pad, pad)
            setBackgroundColor(BACKGROUND)
        }

        root.addView(TextView(this).apply {
            text = "Saves"
            setTextColor(Color.WHITE)
            textSize = 22f
            setTypeface(typeface, Typeface.BOLD)
        })

        root.addView(TextView(this).apply {
            text = "Records, ghosts and Controller Pak data. The game rewrites these as you play, " +
                "so restart after importing or it will save over what you restored."
            setTextColor(SUBTLE)
            textSize = 12f
            setPadding(0, dp(4), 0, dp(8))
        })

        emptyLabel = TextView(this).apply {
            text = "Nothing saved yet.\n\nPlay a race, or import a backup."
            setTextColor(SUBTLE)
            gravity = Gravity.CENTER
            setPadding(0, dp(24), 0, dp(24))
        }
        root.addView(emptyLabel)

        listContainer = LinearLayout(this).apply { orientation = LinearLayout.VERTICAL }
        root.addView(
            ScrollView(this).apply { addView(listContainer) },
            LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 0, 1f)
        )

        val actions = LinearLayout(this).apply {
            orientation = LinearLayout.HORIZONTAL
            setPadding(0, dp(8), 0, 0)
        }
        actions.addView(button("Import") { importSaves.launch(arrayOf("*/*")) })
        actions.addView(button("Back up all") {
            pendingExport = null
            exportSave.launch(SaveStore.BUNDLE_NAME)
        })
        actions.addView(View(this), LinearLayout.LayoutParams(0, 1, 1f))
        actions.addView(button("Restart game") { restartGame() })
        actions.addView(button("Done") { finish() })
        root.addView(actions)

        setContentView(root)
    }

    private fun refresh() {
        val saves = SaveStore.list(this)
        listContainer.removeAllViews()
        emptyLabel.visibility = if (saves.isEmpty()) View.VISIBLE else View.GONE

        saves.forEachIndexed { index, save -> listContainer.addView(row(save, index)) }
    }

    private fun row(save: SaveStore.Save, index: Int): View {
        val row = LinearLayout(this).apply {
            orientation = LinearLayout.HORIZONTAL
            gravity = Gravity.CENTER_VERTICAL
            setPadding(dp(8), dp(6), dp(8), dp(6))
            setBackgroundColor(if (index % 2 == 0) ROW_EVEN else Color.TRANSPARENT)
        }

        val label = LinearLayout(this).apply { orientation = LinearLayout.VERTICAL }
        label.addView(TextView(this).apply {
            text = save.label
            setTextColor(Color.WHITE)
            textSize = 15f
        })
        label.addView(TextView(this).apply {
            text = "${save.file.name} · ${save.file.length()} bytes"
            setTextColor(SUBTLE)
            textSize = 11f
        })
        row.addView(label, LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 1f))

        row.addView(button("Export") {
            pendingExport = save
            exportSave.launch(save.file.name)
        })
        row.addView(button("Delete") { confirmDelete(save) })

        return row
    }

    private fun confirmDelete(save: SaveStore.Save) {
        AlertDialog.Builder(this)
            .setTitle("Delete ${save.label}?")
            .setMessage("Records and ghosts in this file are gone for good. Exporting first keeps a copy.")
            .setPositiveButton("Delete") { _, _ ->
                val error = SaveStore.delete(save)
                if (error != null) {
                    Toast.makeText(this, error, Toast.LENGTH_LONG).show()
                } else {
                    dirty = true
                }
                refresh()
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun runInBackground(message: String, work: () -> List<String>) {
        val progress = AlertDialog.Builder(this)
            .setMessage(message)
            .setCancelable(false)
            .show()

        thread(name = "saves-io") {
            val errors = runCatching(work).getOrElse { listOf(it.message ?: it.toString()) }

            runOnUiThread {
                progress.dismiss()
                if (errors.isEmpty()) {
                    dirty = true
                } else {
                    Toast.makeText(this, errors.joinToString("\n\n"), Toast.LENGTH_LONG).show()
                }
                refresh()
            }
        }
    }

    override fun finish() {
        if (dirty) {
            Toast.makeText(this, "Restart the game so it picks up the saves you changed.", Toast.LENGTH_LONG).show()
        }
        super.finish()
    }

    private fun restartGame() {
        val intent = Intent(this, LauncherActivity::class.java)
            .addFlags(Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK)
        startActivity(intent)
        finishAffinity()
        Runtime.getRuntime().exit(0)
    }

    private fun button(label: String, onClick: () -> Unit) = Button(this).apply {
        text = label
        textSize = 12f
        isAllCaps = false
        minWidth = 0
        minimumWidth = 0
        setPadding(dp(10), dp(4), dp(10), dp(4))
        setOnClickListener { onClick() }
    }

    private fun dp(value: Int) = (value * resources.displayMetrics.density).toInt()

    private companion object {
        val BACKGROUND = Color.rgb(8, 16, 28)
        val SUBTLE = Color.rgb(150, 160, 175)
        val ROW_EVEN = Color.argb(24, 255, 255, 255)
    }
}
