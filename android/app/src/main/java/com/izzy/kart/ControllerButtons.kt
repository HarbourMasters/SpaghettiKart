package com.izzy.kart

/**
 * Indices into the virtual SDL game controller created by the native bridge.
 *
 * They follow SDL's own `SDL_GameControllerButton` / `SDL_GameControllerAxis`
 * ordering. A negative button id addresses the axis with that (positive) index
 * instead, which is how the shoulder triggers are driven from a plain on/off
 * button in the overlay.
 */
object ControllerButtons {
    const val BUTTON_A = 0
    const val BUTTON_B = 1
    const val BUTTON_X = 2
    const val BUTTON_Y = 3
    const val BUTTON_BACK = 4
    const val BUTTON_START = 6
    const val BUTTON_LB = 9
    const val BUTTON_RB = -5

    const val AXIS_LX = 0
    const val AXIS_LY = 1
    const val AXIS_RX = 2
    const val AXIS_RY = 3
    const val AXIS_LT = -2
    const val AXIS_RT = -4
}
