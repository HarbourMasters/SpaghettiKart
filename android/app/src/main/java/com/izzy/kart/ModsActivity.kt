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
 * Mod manager: import, export, enable, delete and reorder what is in the mods
 * folder.
 *
 * Mods are read once when the game starts, so anything changed here takes
 * effect on the next launch — hence the restart button rather than pretending
 * it applied live.
 */
class ModsActivity : ComponentActivity() {

    private lateinit var listContainer: LinearLayout
    private lateinit var emptyLabel: TextView
    private var mods: MutableList<ModStore.Mod> = mutableListOf()
    private var dirty = false

    private val importArchives =
        registerForActivityResult(ActivityResultContracts.OpenMultipleDocuments()) { uris ->
            if (uris.isNullOrEmpty()) return@registerForActivityResult
            runInBackground("Importing…") {
                uris.mapNotNull { ModStore.importArchive(this, it) }
            }
        }

    private val importFolder =
        registerForActivityResult(ActivityResultContracts.OpenDocumentTree()) { uri ->
            if (uri == null) return@registerForActivityResult
            runInBackground("Importing folder…") {
                listOfNotNull(ModStore.importFolder(this, uri))
            }
        }

    /** Set just before the export picker opens, since the contract carries only a filename. */
    private var pendingExport: ModStore.Mod? = null

    private val exportMod =
        registerForActivityResult(ActivityResultContracts.CreateDocument("application/octet-stream")) { uri ->
            val mod = pendingExport ?: return@registerForActivityResult
            pendingExport = null
            if (uri == null) return@registerForActivityResult
            runInBackground("Exporting…") { listOfNotNull(ModStore.export(this, mod, uri)) }
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
            text = "Mods"
            setTextColor(Color.WHITE)
            textSize = 22f
            setTypeface(typeface, Typeface.BOLD)
        })

        root.addView(TextView(this).apply {
            text = "Loaded top to bottom. A mod lower in the list overrides the ones above it."
            setTextColor(SUBTLE)
            textSize = 12f
            setPadding(0, dp(4), 0, dp(8))
        })

        emptyLabel = TextView(this).apply {
            text = "No mods yet.\n\nImport an .o2r or .zip, or a mod folder — or copy files straight into\n" +
                "${GameAssets.modsDir(this@ModsActivity)}"
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
        actions.addView(button("Import mod") { importArchives.launch(arrayOf("*/*")) })
        actions.addView(button("Import folder") { importFolder.launch(null) })
        actions.addView(button("Saves") { startActivity(Intent(this, SavesActivity::class.java)) })
        actions.addView(View(this), LinearLayout.LayoutParams(0, 1, 1f))
        actions.addView(button("Restart game") { restartGame() })
        actions.addView(button("Done") { finish() })
        root.addView(actions)

        setContentView(root)
    }

    private fun refresh() {
        mods = ModStore.list(this).toMutableList()
        listContainer.removeAllViews()
        emptyLabel.visibility = if (mods.isEmpty()) View.VISIBLE else View.GONE

        mods.forEachIndexed { index, mod -> listContainer.addView(row(mod, index)) }
    }

    private fun row(mod: ModStore.Mod, index: Int): View {
        val row = LinearLayout(this).apply {
            orientation = LinearLayout.HORIZONTAL
            gravity = Gravity.CENTER_VERTICAL
            setPadding(dp(8), dp(6), dp(8), dp(6))
            setBackgroundColor(if (index % 2 == 0) ROW_EVEN else ROW_ODD)
        }

        row.addView(button("▲") { move(index, index - 1) }.apply { isEnabled = index > 0 })
        row.addView(button("▼") { move(index, index + 1) }.apply { isEnabled = index < mods.size - 1 })

        val label = LinearLayout(this).apply { orientation = LinearLayout.VERTICAL }
        label.addView(TextView(this).apply {
            text = mod.name
            setTextColor(if (mod.enabled) Color.WHITE else SUBTLE)
            textSize = 15f
        })
        label.addView(TextView(this).apply {
            text = if (mod.enabled) mod.kind else "${mod.kind} · disabled"
            setTextColor(SUBTLE)
            textSize = 11f
        })
        row.addView(label, LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 1f))

        row.addView(button(if (mod.enabled) "Disable" else "Enable") {
            apply(ModStore.setEnabled(mod, !mod.enabled))
        })
        row.addView(button("Export") {
            pendingExport = mod
            exportMod.launch(ModStore.exportName(mod))
        })
        row.addView(button("Delete") { confirmDelete(mod) })

        return row
    }

    private fun move(from: Int, to: Int) {
        if (to !in mods.indices) return
        mods.add(to, mods.removeAt(from))
        apply(ModStore.applyOrder(mods))
    }

    private fun confirmDelete(mod: ModStore.Mod) {
        AlertDialog.Builder(this)
            .setTitle("Delete ${mod.name}?")
            .setMessage("This removes it from the device. Exporting first keeps a copy.")
            .setPositiveButton("Delete") { _, _ -> apply(ModStore.delete(mod)) }
            .setNegativeButton("Cancel", null)
            .show()
    }

    /** Applies the result of a store operation: report the error, or refresh. */
    private fun apply(error: String?) {
        if (error != null) {
            Toast.makeText(this, error, Toast.LENGTH_LONG).show()
        } else {
            dirty = true
        }
        refresh()
    }

    /**
     * Imports and exports move real data around, so they run off the main
     * thread behind a blocking progress dialog.
     */
    private fun runInBackground(message: String, work: () -> List<String>) {
        val progress = AlertDialog.Builder(this)
            .setMessage(message)
            .setCancelable(false)
            .show()

        thread(name = "mods-io") {
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
            Toast.makeText(this, "Mod changes apply the next time the game starts.", Toast.LENGTH_LONG).show()
        }
        super.finish()
    }

    /**
     * Mods are read once during engine start-up, so the process has to go.
     * Relaunching through the launcher picks the new set up on the way back in.
     */
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
        val ROW_ODD = Color.TRANSPARENT
    }
}
