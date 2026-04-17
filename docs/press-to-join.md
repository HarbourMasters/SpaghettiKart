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
| `StopMultiplayer()` | Back to state 1. Wipe `mIgnoredInstanceIds`, then call `RefreshConnectedSDLGamepads()` — the wipe matters because port 0 accumulated "ignore everyone except the assigned device" during multiplayer, and refresh alone only repopulates ports 1-3 |
| `GetPortDeviceStatus(uint8_t port)` | 0 = unassigned, 1 = assigned+connected, -1 = assigned+disconnected |

LUS handles press-to-join detection and assignment internally — when `mPressToJoinActive` is true, it polls for input each frame and assigns to empty active ports. The game doesn't need to drive this — it just reads port status.

**Polling logic (each frame when `mPressToJoinActive`):**

Two-phase timing guards:

1. **Activation grace (15 frames):** skip polling for the first 15 frames after `mPressToJoinActive` goes true. The button that triggered the menu transition into char select is typically still held for a few frames, and we don't want it counted. Implemented via function-local static frame counter.

2. **Release-edge detection:** track per-device "had any input last frame" state. Assign on the transition held→not-held. The alternative (assign on press) fails because `ReadToPad` runs right after `PollPressToJoin` in the same frame — if we claim the port while the button is still held, the game sees the join press as a confirm/select. Assigning on release means SDL state reads empty on the assign frame.

Inside those guards, iterate active ports in order (0 through portCount-1). For each port without an assigned device:
1. If this port has keyboard mappings and `Window::GetLastScancode() != -1` → assign keyboard (pseudo instance ID `-2`). Keyboard uses `GetLastScancode` which is already event-based (fires once per press), so release-edge isn't required there.
2. Otherwise, pick the first device in the release-edge set that isn't already assigned to another port.

Assignment always goes to the lowest-indexed empty active port. If port 2 disconnects while ports 0, 1, 3 are active, the next press fills port 2.

Keyboard can only join the port it's already mapped to (usually port 0). There's no concept of ignoring keyboard, and we don't move keyboard mappings at runtime. This is a POC limitation. Gamepads fill remaining ports as normal.

**Modify existing methods:**

- `RefreshConnectedSDLGamepads()`: Wrap the "ignore all on ports 1-3" logic in `if (!mMultiplayerActive)`. When multiplayer IS active, rebuild ignore lists from `mPressToJoinAssignments` instead — for each active port, ignore all devices except the assigned one; for unassigned ports, ignore everything.

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

**Files:** `src/menus.c`, `src/menu_items.c`

Single source of truth for "is this port joined" is `MultiplayerGetPortStatus(port) == 1`. We deliberately do not overload `gCharacterGridSelections == 0` to mean "unjoined."

**Character select init (menus.c, `MENU_FADE_TYPE_MAIN` case):** Always set `gCharacterGridSelections[i] = i + 1` for `i < gPlayerCount`, regardless of press-to-join state. Unjoined cursors still render at their default grid slot — just greyed out.

**Per-frame logic (menus.c, `player_select_menu_act`):** On `PLAYER_ONE`'s call, iterate ports and detect status transitions 0→1 to play the join sound. A `static s8 sPrevPortStatus[MAXCONTROLLERS]` tracks previous-frame status. Grid selection itself is never modified here; the cursor's *position* is already driven by grid, and its *color* is driven by port status.

**Input routing for unjoined ports:** No explicit gate needed. The ignore lists handle it: every unjoined port has every device in its ignore list, so `controller->ReadToPad` writes zero and `btnAndStick` is 0. The switch cases never see input. (Edge case: if the user manually configured keyboard mappings on port 2/3, keyboard input could leak to those unjoined ports — POC ignores this.)

**Cursor render (menu_items.c, `render_cursor_player`):** When press-to-join is enabled and `MultiplayerGetPortStatus(port) != 1`, wrap the cursor render in `gDPSetGrayscaleColor(0xFF, 0xFF, 0xFF, 0xFF)` + `gSPGrayscale(true/false)`. Keeps the colored frame and number-badge visuals intact but desaturates them. This is the same mechanism used by `MAIN_MENU_BACKGROUND` rendering elsewhere in the file.

Using `gSPGrayscale` rather than swapping prim color: each player's border is two chained textures, and only the first responds to prim color tint. The second (`gTextureP1BorderBlue` etc.) has color baked into texture data. Grayscale post-process desaturates both.

**All-selected check (menus.c, around line 1618):** transition to OK state only when no cursor is unconfirmed AND (if press-to-join is active) every active port has `MultiplayerGetPortStatus == 1`.

### Part 6: SpaghettiKart — Race Start / Menu Return Hooks

**File:** `src/menus.c`

| Location | Call |
|----------|------|
| A in `PLAYER_SELECT_MENU_OK` case (forward to map select) | `MultiplayerStopPressToJoin()` |
| B in `SUB_MENU_MAP_SELECT_CUP` (back to char select) | `MultiplayerStartPressToJoin()` |
| B in `SUB_MENU_MAP_SELECT_BATTLE_COURSE` (back to char select) | `MultiplayerStartPressToJoin()` |
| B in `PLAYER_SELECT_MENU_MAIN` (back to main menu, both `savedSelection == 0` and cursor paths) | `MultiplayerStop()` |

The planned "B in `MAIN_MENU_MODE_SELECT`" hook wasn't needed in practice — `MultiplayerStart` only fires at A in `MAIN_MENU_OK_SELECT`, so there's no active multiplayer state to tear down when backing out of mode select.

### Part 7: Disconnect During Race (minimal POC)

Controller disconnects during race → kart sits idle, race continues. `mPressToJoinActive` is false, so LUS does nothing automatic. No port freeing, no reassignment, no UI. The kart just stops receiving input.

If the player needs time, they ask another player to pause (existing pause mechanic). Social layer handles it.

Future enhancement: game polls `MultiplayerGetPortStatus()` to detect -1 (assigned+disconnected) and show a notification via LUS notification system.

### Part 8: Defaults on ports 1-3 (POC)

**File:** `libultraship/src/ship/controller/controldeck/ControlDeck.cpp`, gated behind `#ifdef ENABLE_PRESS_TO_JOIN`.

Stock LUS only applies default mappings to port 0. Without mappings on ports 1-3, press-to-join flips the ignore lists but `ReadToPad` has nothing to translate SDL state into OSContPad — the game gets zero input on joined ports 2/3/4.

For the POC, `ControlDeck::Init` extends the existing port-0 defaults block with a follow-up loop that calls `AddDefaultMappings(PhysicalDeviceType::SDLGamepad)` on each other port that has `HasConfig() == false`. Keyboard/Mouse are excluded — those only make sense on one port.

**This is flagged as POC in code.** A proper solution would let games express per-port defaults via `ControllerDefaultMappings` rather than hardcoding a loop in library code. The current hack is unconditional when `ENABLE_PRESS_TO_JOIN` is on, which is fine for Spaghetti but wouldn't generalize.

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
- **Game-side source of truth is the status bridge**: `MultiplayerGetPortStatus` is the only signal the game consults for "is this port joined." Grid selection is never overloaded with that meaning
- **Assign on release, not press**: Prevents the join button from also being read as a confirm by the same-frame `ReadToPad`. Costs ~30ms of perceived latency on a tap — acceptable for POC
- **Input Editor UX**: Press-to-join checkbox sits next to the per-device-per-port checkboxes. When on, those checkboxes are disabled. Unchecking press-to-join gives immediate manual control
- **Controllers do unpredictable things**: This is why press-to-join only operates during the explicit join phase (character select), never during gameplay. No silent reassignment during races

## POC Limitations / Known Gaps

Not blockers for the POC, but flagged for any path to merge:

- **Per-port defaults** is a hardcoded library-layer loop (Part 8). Wants a proper API for games.
- **Keyboard release-edge** isn't implemented; if a user has keyboard mapped to multiple ports, they could race-claim two ports at once. `GetLastScancode` is event-based so the press-side case is usually fine.
- **Controller "stolen input" mystery** observed once, not reproducible. Suspected to be keyboard multi-port or a race condition in refresh timing.
- **Analog stick feel on release-edge:** user may find stick-deflect-as-join awkward. Revisit if it does.
- **Disconnect-during-race** leaves the kart idle (Part 7). Acceptable; notification UI is a future nicety.

## Build Note

Configure with `-DCMAKE_BUILD_TYPE=Debug` (or Release) — a bare `cmake -B build` without a build type triggers `-Wincompatible-pointer-types` errors in `courses/*/course_offsets.c` on GCC 15. The course offset files declare `const Gfx*` arrays initialized from `const char*` generated symbols, which is a warning under default flags but becomes an error without an explicit build type on this GCC version.

```
cmake -DCMAKE_BUILD_TYPE=Debug -B build -G Ninja
cmake --build build --target Spaghettify
```

## Verification

1. Delete `build/spaghettify.cfg.json` to simulate fresh install so Part 8 defaults apply to ports 1-3.
2. Build SpaghettiKart.
3. Connect 2+ controllers.
4. Select 2P mode — verify transition to character select with two greyed-out cursors (frames and number badges still visible, just desaturated).
5. Press and release a button on controller 1 — verify it claims P1 (port 0), cursor lights up to full color, and the release press does NOT also confirm a character.
6. Press and release a button on controller 2 — verify it claims P2 (port 1) and gets a cursor.
7. Both players select characters — verify game proceeds to race.
8. Disconnect controller 2 mid-race — verify kart sits idle, no automatic reassignment.
9. Back to main menu — verify all controllers work normally again (all on port 0). This is the `StopMultiplayer` ignore-list wipe at work.
10. Disconnect during character select — verify port is freed and can be re-joined.
11. Uncheck press-to-join in Input Editor — verify multiplayer still works via manual assignment.
12. Verify checkbox only appears when `ENABLE_PRESS_TO_JOIN` is ON.
