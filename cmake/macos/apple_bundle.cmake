# macOS .app bundle assembly for SpaghettiKart.
#
# Turns the plain executable into SpaghettiKart.app (MACOSX_BUNDLE), bundles the
# runtime resources the game and the first-run ROM extractor need into
# Contents/Resources, relinks any non-system dylibs into Contents/Frameworks,
# and ad-hoc codesigns the result so it launches without "damaged app" warnings.
#
# Resource layout the runtime expects on macOS:
#   * Ship::Context::GetAppBundlePath()  -> <App>.app/Contents/Resources  (read-only)
#       - spaghetti.o2r            : packed port assets (GenerateO2R target)
#       - config.yml, yamls/, meta/: asset definitions + mods.toml read by the
#                                    first-run ROM extractor (GameExtractor)
#   * Ship::Context::GetAppDirectoryPath() -> SHIP_HOME
#       (~/Library/Application Support/SpaghettiKart, defaulted in Game.cpp)
#       - mk64.o2r is extracted here on first run from the user's ROM, alongside
#         config/saves/mods, instead of scattering into the home folder

set(MACOS_DIR ${CMAKE_SOURCE_DIR}/cmake/macos)
set(ENTITLEMENTS_FILE ${MACOS_DIR}/entitlements.plist)

option(SPAGHETTI_BUNDLE_DEPS "Relink and bundle dylibs into the .app so it is portable" ON)

# ---------------------------------------------------------------------------
# Bundle metadata. OUTPUT_NAME "SpaghettiKart" makes the bundle SpaghettiKart.app
# with Contents/MacOS/SpaghettiKart, matching CFBundleExecutable in Info.plist
# (the build target itself stays named "Spaghettify").
# ---------------------------------------------------------------------------
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "SpaghettiKart"
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Info.plist
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"
    XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS ${ENTITLEMENTS_FILE}
)

# ---------------------------------------------------------------------------
# App icon.
#
# The repo ships the compiled icon artifacts (same approach as other ports that
# ship a prebuilt Assets.car), so no special Xcode is needed at build time:
#   - Assets.car          : compiled asset catalog with the layered Liquid Glass
#                           icon, used on macOS 26+ via CFBundleIconName
#                           ("SpaghettiKartIcon").
#   - SpaghettiKart.icns  : flattened icon for macOS versions before 26, via
#                           CFBundleIconFile.
# Both are built from cmake/macos/SpaghettiKartIcon.icon (Icon Composer package;
# the ship-kart art from icon.png with the subject separated from the background
# so the system can render the glass depth).
#
# Staleness guard: if icon.png ever changes without the icon artifacts being
# regenerated, the build detects it via the source hash stamp and falls back to
# generating a flat icns from the CURRENT icon.png with sips/iconutil, so an
# outdated icon can never ship and updating the logo requires no macOS work.
#
# To regenerate after a logo change (needs a Mac with Xcode 26+):
#   1. Recreate the cut-out art layer and update SpaghettiKartIcon.icon in
#      Apple's Icon Composer.
#   2. xcrun actool cmake/macos/SpaghettiKartIcon.icon --compile /tmp/iconout \
#        --app-icon SpaghettiKartIcon --output-partial-info-plist /tmp/iconout/p.plist \
#        --platform macosx --target-device mac --minimum-deployment-target 11.0
#   3. cp /tmp/iconout/Assets.car cmake/macos/Assets.car
#      cp /tmp/iconout/SpaghettiKartIcon.icns cmake/macos/SpaghettiKart.icns
#   4. shasum -a 256 icon.png > cmake/macos/SpaghettiKartIcon.icon.source-sha256
# ---------------------------------------------------------------------------
set(ICON_SOURCE_STAMP ${MACOS_DIR}/SpaghettiKartIcon.icon.source-sha256)
set(ICON_PREBUILT_CAR ${MACOS_DIR}/Assets.car)
set(ICON_PREBUILT_ICNS ${MACOS_DIR}/SpaghettiKart.icns)
set(ICNS_FILE ${CMAKE_BINARY_DIR}/macosx/SpaghettiKart.icns)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/macosx)

set(ICON_SOURCE_FRESH FALSE)
if(EXISTS ${ICON_SOURCE_STAMP} AND EXISTS ${ICON_PREBUILT_CAR} AND EXISTS ${ICON_PREBUILT_ICNS})
    file(SHA256 ${CMAKE_SOURCE_DIR}/icon.png ICON_PNG_HASH)
    file(READ ${ICON_SOURCE_STAMP} ICON_STAMP_HASH)
    string(STRIP "${ICON_STAMP_HASH}" ICON_STAMP_HASH)
    if(ICON_PNG_HASH STREQUAL ICON_STAMP_HASH)
        set(ICON_SOURCE_FRESH TRUE)
    else()
        message(STATUS "App icon: icon.png changed since the icon artifacts were made; using a flat icon from the current icon.png")
    endif()
endif()

if(ICON_SOURCE_FRESH)
    message(STATUS "App icon: prebuilt Liquid Glass catalog + flattened icns fallback")
    file(COPY_FILE ${ICON_PREBUILT_ICNS} ${ICNS_FILE})
    file(COPY_FILE ${ICON_PREBUILT_CAR} ${CMAKE_BINARY_DIR}/macosx/Assets.car)
    set(ICON_BUNDLE_FILES ${CMAKE_BINARY_DIR}/macosx/Assets.car ${ICNS_FILE})
    set(SPAGHETTI_ICON_HAS_GLASS TRUE)
else()
    set(ICON_SRC ${CMAKE_SOURCE_DIR}/icon.png)
    set(ICONSET_DIR ${CMAKE_BINARY_DIR}/macosx/SpaghettiKart.iconset)
    file(MAKE_DIRECTORY ${ICONSET_DIR})
    foreach(SPEC "16;icon_16x16" "32;icon_16x16@2x" "32;icon_32x32" "64;icon_32x32@2x"
                 "128;icon_128x128" "256;icon_128x128@2x" "256;icon_256x256"
                 "512;icon_256x256@2x" "512;icon_512x512" "1024;icon_512x512@2x")
        list(GET SPEC 0 SZ)
        list(GET SPEC 1 NAME)
        execute_process(COMMAND sips -z ${SZ} ${SZ} ${ICON_SRC} --out ${ICONSET_DIR}/${NAME}.png OUTPUT_QUIET ERROR_QUIET)
    endforeach()
    execute_process(COMMAND iconutil -c icns -o ${ICNS_FILE} ${ICONSET_DIR})
    set(ICON_BUNDLE_FILES ${ICNS_FILE})
    set(SPAGHETTI_ICON_HAS_GLASS FALSE)
endif()

set_source_files_properties(${ICON_BUNDLE_FILES} PROPERTIES GENERATED TRUE MACOSX_PACKAGE_LOCATION "Resources")
target_sources(${PROJECT_NAME} PRIVATE ${ICON_BUNDLE_FILES})

if(NOT SPAGHETTI_ICON_HAS_GLASS)
    # No asset catalog in this build: remove CFBundleIconName so macOS 26+
    # falls back to CFBundleIconFile instead of showing a generic icon.
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND plutil -remove CFBundleIconName "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/Info.plist" || true
        VERBATIM
    )
endif()

# Ensure the packed port assets (spaghetti.o2r) are generated before the app
# links, so the POST_BUILD step below has them to copy into the bundle.
# (mk64.o2r is created at runtime from the user's ROM into SHIP_HOME, so it is
# intentionally not a build dependency.)
add_dependencies(${PROJECT_NAME} GenerateO2R)

# ---------------------------------------------------------------------------
# Copy runtime resources into Contents/Resources after the app links.
# ---------------------------------------------------------------------------
set(RES_DIR "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/Resources")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RES_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/config.yml" "${RES_DIR}/config.yml"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/yamls" "${RES_DIR}/yamls"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/meta" "${RES_DIR}/meta"
    # spaghetti.o2r is produced by the GenerateO2R target; copy if present (CI
    # builds it in a separate job and inserts it into the bundle afterwards).
    COMMAND bash -c "[ -f '${CMAKE_BINARY_DIR}/spaghetti.o2r' ] && cp '${CMAKE_BINARY_DIR}/spaghetti.o2r' '${RES_DIR}/spaghetti.o2r' || ([ -f '${CMAKE_SOURCE_DIR}/spaghetti.o2r' ] && cp '${CMAKE_SOURCE_DIR}/spaghetti.o2r' '${RES_DIR}/spaghetti.o2r' || echo 'note: spaghetti.o2r not found - build the GenerateO2R target, then rebuild')"
    COMMENT "Bundling SpaghettiKart resources into the .app"
    VERBATIM
)

# ---------------------------------------------------------------------------
# Relink dylibs into Contents/Frameworks (portable .app) and codesign.
# With static vcpkg dependencies (CI) this is a near no-op; it matters for
# local builds against Homebrew/MacPorts dylibs.
# ---------------------------------------------------------------------------
if (SPAGHETTI_BUNDLE_DEPS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            -DAPP_BUNDLE=$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>
            -DEXECUTABLE_NAME=SpaghettiKart
            -P ${MACOS_DIR}/fixup_bundle.cmake
        COMMAND bash -c "install_name_tool -add_rpath '@executable_path/../Frameworks/' '$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/MacOS/SpaghettiKart' 2>/dev/null || true"
        # Homebrew's "sdl2" is sdl2-compat, a shim that dlopen()s libSDL3.dylib
        # from @loader_path at runtime. fixup_bundle can't follow a dlopen, so if
        # a dynamic libSDL2 was bundled, copy SDL3 in next to it (= @loader_path)
        # or the app aborts with "Failed loading SDL3 library." SDL3 itself only
        # links system frameworks. Static SDL2 builds (vcpkg) skip this entirely.
        COMMAND bash -c "FRAMEWORKS='$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/Frameworks'; if ls \"$FRAMEWORKS\"/libSDL2*.dylib >/dev/null 2>&1; then SDL3_LIB=$(brew --prefix sdl3 2>/dev/null)/lib/libSDL3.0.dylib; if [ -f \"$SDL3_LIB\" ]; then cp \"$SDL3_LIB\" \"$FRAMEWORKS/libSDL3.dylib\" && chmod u+w \"$FRAMEWORKS/libSDL3.dylib\"; else echo 'warning: bundled libSDL2 is sdl2-compat but Homebrew sdl3 was not found'; fi; fi"
        COMMENT "Relinking dylibs into the .app bundle"
        VERBATIM
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND codesign --force --deep --sign - --options runtime --entitlements ${ENTITLEMENTS_FILE} "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>"
    COMMENT "Ad-hoc codesigning SpaghettiKart.app"
    VERBATIM
)
