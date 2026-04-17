# Press-to-Join Multiplayer Controller System

## Context

LUS PR #792/#793 simplified the controller system by putting all devices on port 0. Multiplayer still works, but requires manually opening the controller menu and assigning physical devices to ports every launch. Open PR #964 (auto-distribute by enumeration) has been stalled 4+ months with fundamental issues. Issue #1058 asks for persistent device IDs, which is a known bad path.

This implements a press-to-join model where device identity never matters — only user actions drive assignment, removing the need for manual controller menu configuration.

Core principle: never try to identify a controller. Phantom controllers never press buttons. Duplicate devices don't matter because the one the user is holding is the one that fires input. Device identity doesn't need to survive reconnects because the user just presses a button again.

## Three States

1. **Single player** — all devices on port 0 (current default behavior)
2. **Press-to-join active** — character select screen only. LUS detects unassigned device input and assigns to the next empty active port. On disconnect, port is freed so it can be filled again by the next press. This is the only screen where players have direct visual feedback (cursor/border) for joins and disconnects
3. **Press-to-join inactive** — anywhere outside character select (map select, racing, etc.). LUS does nothing automatic. Ports stay as they are. Controllers do unpredictable things — we don't silently reassign when there's no visual feedback

Transitions:
- User picks 2P+, enters character select → state 1 → state 2 (`MultiplayerStart`)
- Leave character select forward to map select → state 2 → state 3 (`StopPressToJoin`)
- Return to character select from map select → state 3 → state 2 (`StartPressToJoin`)
- Back to main menu from character select → state 2 → state 1 (`MultiplayerStop`)
- Back to main menu from race/map select → state 3 → state 1 (`MultiplayerStop`)

## Feature Gate

**CMake option:** `ENABLE_PRESS_TO_JOIN` (default OFF in LUS, ON in SpaghettiKart's CMakeLists.txt)

This gates:
- The press-to-join checkbox in `InputEditorWindow.cpp` (`libultraship/src/ship/window/gui/InputEditorWindow.cpp`)
- All press-to-join state and methods in `ConnectedPhysicalDeviceManager`
- The bridge functions in `multiplayerbridge.h`

**CVar:** `gPressToJoinEnabled` (default ON when feature is compiled in). Controlled by the checkbox in the Input Editor. All bridge functions no-op when disabled.

Games that don't set `ENABLE_PRESS_TO_JOIN` get no checkbox, no press-to-join code, no behavior change.

## Implementation

### Part 1: LUS — Extend ConnectedPhysicalDeviceManager

**Files:**
- `libultraship/include/ship/controller/physicaldevice/ConnectedPhysicalDeviceManager.h`
- `libultraship/src/ship/controller/physicaldevice/ConnectedPhysicalDeviceManager.cpp`

**New private state:**
```cpp
bool mMultiplayerActive = false;        // true in states 2 and 3
bool mPressToJoinActive = false;        // true only in state 2
uint8_t mMultiplayerPortCount = 1;
std::unordered_map<uint8_t, int32_t> mPressToJoinAssignments; // port → instanceId
```

`mPressToJoinAssignments` is bookkeeping for the automatic system only. The real source of truth for what device is on what port is always the existing ignore lists. If someone manually changes assignments via the Input Editor, those take effect immediately — this map may become stale, and that's fine.

**New public methods:**

| Method | Purpose |
|--------|---------|
| `StartMultiplayer(uint8_t portCount)` | Enter state 2. Ignore all devices on all active ports (everyone starts unassigned). Sets both `mMultiplayerActive` and `mPressToJoinActive`. Press-to-join fills ports starting at 0 — first press is P1, second is P2, etc. |
| `StopPressToJoin()` | Enter state 3. Sets `mPressToJoinActive = false`. Assignments stay, LUS stops all automatic behavior |
| `StartPressToJoin()` | Re-enter state 2 (e.g. back to character select from map select). Sets `mPressToJoinActive = true`. Frees any assigned ports whose devices are disconnected, so they can be re-joined |
| `StopMultiplayer()` | Back to state 1. Clear all state, call `RefreshConnectedSDLGamepads()` to restore default all-on-port-0 |
| `GetPortDeviceStatus(uint8_t port)` | 0 = unassigned, 1 = assigned+connected, -1 = assigned+disconnected |

LUS handles press-to-join detection and assignment internally — when `mPressToJoinActive` is true, it polls for input each frame and assigns to empty active ports. The game doesn't need to drive this — it just reads port status.

**Polling logic (each frame when `mPressToJoinActive`):**

Iterate active ports in order (0 through portCount-1). For each port without an assigned device:
1. If this port has keyboard mappings configured and `Window::GetLastScancode() != -1` → assign keyboard (pseudo instance ID `-2`) to this port
2. Check all unassigned SDL gamepads — raw `SDL_GameControllerGetButton`/`SDL_GameControllerGetAxis` with reasonable deadzone — assign first match

Assignment always goes to the lowest-indexed empty active port. If port 2 disconnects while ports 0, 1, 3 are active, the next press fills port 2.

Keyboard can only join the port it's already mapped to (usually port 0). There's no concept of ignoring keyboard, and we don't move keyboard mappings at runtime. This is a POC limitation. Gamepads fill remaining ports as normal.

**Modify existing methods:**

- `RefreshConnectedSDLGamepads()` (lines 103-105): Wrap the "ignore all on ports 1-3" logic in `if (!mMultiplayerActive)`. When multiplayer IS active, rebuild ignore lists from `mPressToJoinAssignments` instead — for each active port, ignore all devices except the assigned one; for unassigned ports, ignore everything.

- `HandlePhysicalDeviceDisconnect()`: After refresh, if `mPressToJoinActive` and disconnected device was in `mPressToJoinAssignments`, free the port — the next press will fill it again. If NOT `mPressToJoinActive` (racing), don't touch anything — the port stays assigned-but-disconnected. No input flows, kart sits idle.

### Part 2: LUS — C Bridge

**New files:**
- `libultraship/include/libultraship/bridge/multiplayerbridge.h`
- `libultraship/src/libultraship/bridge/multiplayerbridge.cpp`

**Modify:**
- `libultraship/include/libultraship/bridge.h` — add `#include` for new header

Bridge functions (extern "C", follows pattern from `controllerbridge.h`):

```c
void MultiplayerStart(uint8_t portCount);
void MultiplayerStopPressToJoin(void);
void MultiplayerStartPressToJoin(void);
void MultiplayerStop(void);
int8_t MultiplayerGetPortStatus(uint8_t port);
```

All functions check the `gPressToJoinEnabled` CVar internally and no-op when disabled. The game always calls them — it doesn't need to check the CVar itself.

Each function reaches through `Ship::Context::GetInstance()->GetControlDeck()->GetConnectedPhysicalDeviceManager()->...`

### Part 3: LUS — Input Editor Changes

**File:** `libultraship/src/ship/window/gui/InputEditorWindow.cpp` (device assignment section around lines 1193-1216)

Gated behind `#ifdef ENABLE_PRESS_TO_JOIN`:

- Add a "Press-to-Join" checkbox next to the per-device-per-port assignment checkboxes, bound to the `gPressToJoinEnabled` CVar
- When the CVar is ON: disable (grey out) the per-device-per-port checkboxes — press-to-join is managing assignments
- When the CVar is OFF: per-device-per-port checkboxes are fully editable. User has full manual control

### Part 4: SpaghettiKart — CMake + Game Select Hook

**File:** `CMakeLists.txt` — add `set(ENABLE_PRESS_TO_JOIN ON)` before the `add_subdirectory(libultraship)` call.

**File:** `src/menus.c`

**At line 1512** (A_BUTTON in `MAIN_MENU_OK_SELECT` — the final confirmation before entering character select): When `gPlayerCount >= 2`, start multiplayer.

```c
} else if (btnAndStick & A_BUTTON) {
    if (gPlayerCount >= 2) {
        MultiplayerStart(gPlayerCount);
    }
    func_8009E1C0();
    play_sound2(SOUND_MENU_OK_CLICKED);
    setup_game_mode_selected();
    // ... existing code
}
```

This is the moment the user confirms their game mode and transitions into character select. `MultiplayerStart` ignores all devices on all active ports — nobody is assigned yet. Press-to-join on character select will fill ports starting at 0. The person who was navigating menus just presses a button on character select to claim P1.

Keyboard input bypasses the SDL ignore system entirely (goes through `ProcessKeyboardEvent` which isn't filtered), so it works on port 0 regardless.

### Part 5: SpaghettiKart — Character Select Press-to-Join

**File:** `src/menus.c`

**Character select init (line 1987-1994):** When multiplayer is active, all players start with greyed-out cursors at their default grid positions (`gCharacterGridSelections[i] = i + 1` as normal). The grey state indicates "waiting for a controller." When a device joins via press-to-join, the cursor lights up to the player's color. This is clearer than no cursor — users can see the slot exists and is waiting.

**Port status polling:** Each frame during character select, check `MultiplayerGetPortStatus()` for each port. When a port transitions from 0 (unassigned) to 1 (assigned+connected), change the cursor from greyed-out to the player's color via dlist color patching (proven approach from SoH). The player can now navigate and select their character. If a port transitions from 1 to -1 (disconnected) or 0 (freed), grey out the cursor again.

The existing `player_select_menu_act` already handles `gCharacterGridSelections[i] == 0` correctly (line 1580-1586): controllers with no cursor can only press B to go back. Once we set a cursor position, the controller naturally starts working.

**All-selected check (lines 1604-1610):** Currently transitions to OK state if no controller has (grid != 0 AND not selected). An unjoined player (grid == 0, i < gPlayerCount) passes this check incorrectly. Add a check: if `i < gPlayerCount && gCharacterGridSelections[i] == 0`, that player hasn't joined yet → not ready.

### Part 6: SpaghettiKart — Race Start / Menu Return Hooks

**File:** `src/menus.c`

**Leave character select (forward to map select):** When all players have confirmed characters (`PLAYER_SELECT_MENU_OK` state), call `MultiplayerStopPressToJoin()`. LUS stops all automatic behavior. Character select is the only screen with visual feedback for joins/disconnects, so press-to-join shouldn't be active elsewhere.

**Return to character select (back from map select):** Call `MultiplayerStartPressToJoin()` to re-enable automatic assignment.

**Back to main menu:** At `func_8009E208()` (called on B press from character select, line 1582) and when going back from mode select to player select (B in `MAIN_MENU_MODE_SELECT`): call `MultiplayerStop()`.

### Part 7: Disconnect During Race (minimal POC)

Controller disconnects during race → kart sits idle, race continues. `mPressToJoinActive` is false, so LUS does nothing automatic. No port freeing, no reassignment, no UI. The kart just stops receiving input.

If the player needs time, they ask another player to pause (existing pause mechanic). Social layer handles it.

Future enhancement: game polls `MultiplayerGetPortStatus()` to detect -1 (assigned+disconnected) and show a notification via LUS notification system.

## Keyboard Handling

Keyboard has no SDL instance ID and no ignore system — it's fundamentally different from gamepads. `ProcessKeyboardEvent` in `ControlDeck.cpp` (line 43) broadcasts keyboard events to all ports, but only ports with keyboard mappings configured respond.

**Press-to-join detection:** When polling empty active ports, check if the port has keyboard mappings configured and `Window::GetLastScancode() != -1`. If so, assign keyboard (pseudo instance ID `-2`) to that port.

**Keyboard can only join the port it's mapped to** (default port 0). We don't move keyboard mappings at runtime and there's no keyboard ignore system. This is a POC limitation — a full implementation could add keyboard port reassignment.

**What this means in practice:**
- Keyboard user enters 2P → presses a key on character select → joins port 0 (where keyboard is mapped) → gamepad press fills port 1
- Gamepad-only user enters 2P → first gamepad press fills port 0, second fills port 1. Keyboard mapped to port 0 is also there but doesn't matter since nobody is pressing keys
- If keyboard is configured on port 2 for some reason, keyboard join goes to port 2

## Key Design Notes
- **No new classes needed**: All state lives in `ConnectedPhysicalDeviceManager`
- **No persistence**: RAM-only, resets on launch
- **Thread safety**: All on main thread (SDL events, game loop, menus)
- **LUS handles press-to-join internally**: The game doesn't drive detection or assignment — it just reads port status. LUS polls unassigned gamepads via raw SDL when `mPressToJoinActive` is true
- **Press-to-join assignments are bookkeeping only**: The `mPressToJoinAssignments` map tracks what the automatic system did — the ignore lists remain the real source of truth for input routing
- **Input Editor UX**: Press-to-join checkbox sits next to the per-device-per-port checkboxes. When on, those checkboxes are disabled. Unchecking press-to-join gives immediate manual control
- **Controllers do unpredictable things**: This is why press-to-join only operates during the explicit join phase (character select), never during gameplay. No silent reassignment during races

## Verification

1. Build SpaghettiKart with the changes
2. Connect 2+ controllers
3. Select 2P mode — verify transition to character select with two greyed-out cursors
4. Press a button on controller 1 — verify it claims P1 (port 0) and cursor lights up
5. Press a button on controller 2 — verify it claims P2 (port 1) and gets a cursor
6. Both players select characters — verify game proceeds to race
7. Disconnect controller 2 mid-race — verify kart sits idle, no automatic reassignment
8. Back to main menu — verify all controllers work normally again (all on port 0)
9. Disconnect during character select — verify port is freed and can be re-joined
10. Uncheck press-to-join in Input Editor — verify multiplayer still works via manual assignment
11. Verify checkbox only appears when `ENABLE_PRESS_TO_JOIN` is ON
