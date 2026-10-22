package com.izzy.kart

import android.annotation.SuppressLint
import android.content.Intent
import android.content.SharedPreferences
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.KeyEvent
import android.view.LayoutInflater
import android.view.MotionEvent
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.FrameLayout
import android.widget.ImageView
import org.libsdl.app.SDLActivity
import kotlin.math.sqrt

/**
 * The game itself, plus the on-screen controller drawn over the SDL surface.
 *
 * The overlay drives a virtual SDL joystick (see src/port/android/AndroidBridge.cpp)
 * rather than synthesising input events, so it goes through libultraship's normal
 * controller mapping and can be remapped in-game like any other pad.
 *
 * [LauncherActivity] guarantees mk64.o2r exists before this activity starts.
 */
class MainActivity : SDLActivity() {

    private lateinit var preferences: SharedPreferences
    private lateinit var modsButton: Button

    private var controlsEnabled = true
    private var menuOpen = false
    private var controlsHidden = false

    private val handler = Handler(Looper.getMainLooper())
    private val menuWatcher = object : Runnable {
        override fun run() {
            syncMenuState()
            handler.postDelayed(this, MENU_POLL_MS)
        }
    }

    // org/libsdl/app is kept byte-identical to the SDL release libultraship
    // pins, so the game library is named here rather than patched in there.
    // SDLActivity refuses to start if the Java glue and libSDL2.so disagree on
    // their version, so both have to move together.
    override fun getLibraries(): Array<String> = arrayOf("SDL2", "Spaghettify")

    private external fun attachController()
    private external fun detachController()
    private external fun setButton(button: Int, value: Boolean)
    private external fun setAxis(axis: Int, value: Short)
    private external fun isMenuOpen(): Boolean

    override fun onCreate(savedInstanceState: Bundle?) {
        preferences = getSharedPreferences(PREFS, MODE_PRIVATE)
        super.onCreate(savedInstanceState)
        setupControllerOverlay()
        attachController()
    }

    override fun onResume() {
        super.onResume()
        // Re-sync immediately as well as on the timer, so coming back from the
        // mods manager never leaves the overlay showing a stale menu state.
        syncMenuState()
        handler.removeCallbacks(menuWatcher)
        handler.postDelayed(menuWatcher, MENU_POLL_MS)
    }

    override fun onPause() {
        handler.removeCallbacks(menuWatcher)
        super.onPause()
    }

    override fun onDestroy() {
        handler.removeCallbacks(menuWatcher)
        detachController()
        super.onDestroy()
    }

    /**
     * Mirrors the engine's menu state onto the overlay.
     *
     * The menu is not only driven by the on-screen Menu button: a physical
     * keyboard, a gamepad, or the menu dismissing itself all change it without
     * the overlay hearing about it. Tracking it with a local flag drifted out
     * of sync and left the controls dead with the Mods button stuck on screen,
     * so the engine is asked instead.
     */
    private fun syncMenuState() {
        val open = runCatching { isMenuOpen() }.getOrDefault(false)
        if (open == menuOpen) return

        menuOpen = open
        controlsEnabled = !open
        // Managing mods is a settings action, so it stays out of reach until
        // the menu is up.
        modsButton.visibility = if (open) View.VISIBLE else View.GONE

        if (open) {
            releaseAllInputs()
        }
    }

    /**
     * Clears anything the overlay was holding down. Without this, opening the
     * menu mid-input leaves that button or stick stuck on in the virtual pad,
     * because the touch listeners stop reporting once controls are disabled.
     */
    private fun releaseAllInputs() {
        for (button in intArrayOf(
            ControllerButtons.BUTTON_A, ControllerButtons.BUTTON_B,
            ControllerButtons.BUTTON_X, ControllerButtons.BUTTON_Y,
            ControllerButtons.BUTTON_LB, ControllerButtons.BUTTON_RB,
            ControllerButtons.BUTTON_START, ControllerButtons.BUTTON_BACK,
            ControllerButtons.AXIS_RT
        )) {
            setButton(button, false)
        }
        for (axis in intArrayOf(
            ControllerButtons.AXIS_LX, ControllerButtons.AXIS_LY,
            ControllerButtons.AXIS_RX, ControllerButtons.AXIS_RY
        )) {
            setAxis(axis, 0)
        }
    }

    private fun setupControllerOverlay() {
        val inflater = getSystemService(LAYOUT_INFLATER_SERVICE) as LayoutInflater
        val overlay = inflater.inflate(R.layout.touchcontrol_overlay, null).apply {
            layoutParams = FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.MATCH_PARENT,
                FrameLayout.LayoutParams.MATCH_PARENT
            )
        }
        findViewById<ViewGroup>(android.R.id.content).addView(overlay)

        val buttonGroup = overlay.findViewById<ViewGroup>(R.id.button_group)

        overlay.bindButton(R.id.buttonA, ControllerButtons.BUTTON_A)
        overlay.bindButton(R.id.buttonB, ControllerButtons.BUTTON_B)
        overlay.bindButton(R.id.buttonX, ControllerButtons.BUTTON_X)
        overlay.bindButton(R.id.buttonY, ControllerButtons.BUTTON_Y)
        overlay.bindButton(R.id.buttonLB, ControllerButtons.BUTTON_LB)
        overlay.bindButton(R.id.buttonRB, ControllerButtons.BUTTON_RB)
        overlay.bindButton(R.id.buttonZ, ControllerButtons.AXIS_RT)
        overlay.bindButton(R.id.buttonStart, ControllerButtons.BUTTON_START)
        overlay.bindButton(R.id.buttonBack, ControllerButtons.BUTTON_BACK)

        // The C buttons sit on the right stick, which is how the N64 pad is
        // mapped, so the d-pad cluster drives that axis rather than the d-pad.
        overlay.bindAxisButton(R.id.buttonDpadUp, ControllerButtons.AXIS_RY, positive = false)
        overlay.bindAxisButton(R.id.buttonDpadDown, ControllerButtons.AXIS_RY, positive = true)
        overlay.bindAxisButton(R.id.buttonDpadLeft, ControllerButtons.AXIS_RX, positive = false)
        overlay.bindAxisButton(R.id.buttonDpadRight, ControllerButtons.AXIS_RX, positive = true)

        modsButton = overlay.findViewById(R.id.buttonMods)
        modsButton.setOnClickListener { startActivity(Intent(this, ModsActivity::class.java)) }

        setupMenuButton(overlay.findViewById(R.id.buttonMenu))
        setupJoystick(overlay.findViewById(R.id.left_joystick), overlay.findViewById(R.id.left_joystick_knob))
        setupToggleButton(overlay.findViewById(R.id.buttonToggle), buttonGroup)
    }

    @SuppressLint("ClickableViewAccessibility")
    private fun View.bindButton(id: Int, button: Int) {
        val view = findViewById<Button>(id)
        view.setOnTouchListener { _, event ->
            if (!controlsEnabled) return@setOnTouchListener false
            when (event.actionMasked) {
                MotionEvent.ACTION_DOWN -> {
                    setButton(button, true)
                    view.isPressed = true
                    true
                }
                MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                    setButton(button, false)
                    view.isPressed = false
                    true
                }
                else -> false
            }
        }
    }

    @SuppressLint("ClickableViewAccessibility")
    private fun View.bindAxisButton(id: Int, axis: Int, positive: Boolean) {
        val view = findViewById<Button>(id)
        val pressed = if (positive) Short.MAX_VALUE else Short.MIN_VALUE
        view.setOnTouchListener { _, event ->
            if (!controlsEnabled) return@setOnTouchListener false
            when (event.actionMasked) {
                MotionEvent.ACTION_DOWN -> {
                    setAxis(axis, pressed)
                    view.isPressed = true
                    true
                }
                MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                    setAxis(axis, 0)
                    view.isPressed = false
                    true
                }
                else -> false
            }
        }
    }

    /**
     * Escape opens libultraship's menu. While it is up the overlay stops
     * feeding the virtual pad so taps land on the menu instead of the game.
     */
    @SuppressLint("ClickableViewAccessibility")
    private fun setupMenuButton(button: Button) {
        button.setOnTouchListener { _, event ->
            when (event.actionMasked) {
                MotionEvent.ACTION_DOWN -> {
                    onNativeKeyDown(KeyEvent.KEYCODE_ESCAPE)
                    button.isPressed = true
                    // Don't assume this opened or closed the menu — syncMenuState
                    // picks up whatever the engine actually did.
                    true
                }
                MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                    onNativeKeyUp(KeyEvent.KEYCODE_ESCAPE)
                    button.isPressed = false
                    true
                }
                else -> false
            }
        }
    }

    private fun setupToggleButton(button: Button, group: ViewGroup) {
        controlsHidden = preferences.getBoolean(KEY_CONTROLS_HIDDEN, false)
        group.visibility = if (controlsHidden) View.INVISIBLE else View.VISIBLE

        button.setOnClickListener {
            controlsHidden = !controlsHidden
            group.visibility = if (controlsHidden) View.INVISIBLE else View.VISIBLE
            preferences.edit().putBoolean(KEY_CONTROLS_HIDDEN, controlsHidden).apply()
        }
    }

    @SuppressLint("ClickableViewAccessibility")
    private fun setupJoystick(joystick: FrameLayout, knob: ImageView) {
        joystick.post {
            val centerX = joystick.width / 2f
            val centerY = joystick.height / 2f
            val maxRadius = joystick.width / 2f - knob.width / 2f

            fun recenter() {
                knob.x = centerX - knob.width / 2f
                knob.y = centerY - knob.height / 2f
            }

            joystick.setOnTouchListener { _, event ->
                if (!controlsEnabled) return@setOnTouchListener false
                when (event.actionMasked) {
                    MotionEvent.ACTION_DOWN, MotionEvent.ACTION_MOVE -> {
                        var deltaX = event.x - centerX
                        var deltaY = event.y - centerY
                        val distance = sqrt(deltaX * deltaX + deltaY * deltaY)
                        if (distance > maxRadius) {
                            val scale = maxRadius / distance
                            deltaX *= scale
                            deltaY *= scale
                        }

                        knob.x = centerX + deltaX - knob.width / 2f
                        knob.y = centerY + deltaY - knob.height / 2f

                        setAxis(ControllerButtons.AXIS_LX, (deltaX / maxRadius * Short.MAX_VALUE).toInt().toShort())
                        setAxis(ControllerButtons.AXIS_LY, (deltaY / maxRadius * Short.MAX_VALUE).toInt().toShort())
                        true
                    }
                    MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                        recenter()
                        setAxis(ControllerButtons.AXIS_LX, 0)
                        setAxis(ControllerButtons.AXIS_LY, 0)
                        true
                    }
                    else -> false
                }
            }
        }
    }

    private companion object {
        const val PREFS = "com.izzy.kart.prefs"
        const val KEY_CONTROLS_HIDDEN = "controlsHidden"

        /** Fast enough to feel immediate on a menu toggle, cheap enough to ignore. */
        const val MENU_POLL_MS = 150L
    }
}
