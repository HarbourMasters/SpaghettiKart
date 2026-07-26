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
# Preferred path (Xcode 26+): compile the Icon Composer package
# (cmake/macos/SpaghettiKartIcon.icon) with actool at configure time. This
# produces:
#   - Assets.car             : compiled asset catalog with the Liquid Glass icon,
#                              used on macOS 26 (Tahoe) and later via CFBundleIconName
#                              ("SpaghettiKartIcon" — must match the .icon basename).
#   - SpaghettiKartIcon.icns : a flattened fallback rendered by actool, renamed to
#                              SpaghettiKart.icns to match CFBundleIconFile, used by
#                              macOS versions that predate Liquid Glass.
#
# Fallback path (older Xcode without .icon support): generate the flat
# SpaghettiKart.icns from icon.png with sips/iconutil as before, and strip
# CFBundleIconName from the bundled Info.plist after the build so macOS 26
# doesn't look for an asset catalog that isn't there.
# ---------------------------------------------------------------------------
set(ICON_COMPOSER_SRC ${MACOS_DIR}/SpaghettiKartIcon.icon)
set(ICON_COMPILE_DIR ${CMAKE_BINARY_DIR}/macosx/AppIconAssets)
set(ICNS_FILE ${CMAKE_BINARY_DIR}/macosx/SpaghettiKart.icns)

# CMAKE_OSX_DEPLOYMENT_TARGET can be empty here (the project's set(... CACHE)
# doesn't override a pre-existing empty cache entry); an empty value would eat
# actool's argument parsing, so fall back explicitly.
if(CMAKE_OSX_DEPLOYMENT_TARGET)
    set(ICON_MIN_TARGET ${CMAKE_OSX_DEPLOYMENT_TARGET})
else()
    set(ICON_MIN_TARGET "11.0")
endif()

file(MAKE_DIRECTORY ${ICON_COMPILE_DIR})
execute_process(
    COMMAND xcrun actool ${ICON_COMPOSER_SRC}
            --compile ${ICON_COMPILE_DIR}
            --app-icon SpaghettiKartIcon
            --output-partial-info-plist ${ICON_COMPILE_DIR}/icon-partial-info.plist
            --platform macosx
            --target-device mac
            --minimum-deployment-target ${ICON_MIN_TARGET}
    RESULT_VARIABLE ACTOOL_RESULT
    OUTPUT_QUIET ERROR_QUIET
)

if(ACTOOL_RESULT EQUAL 0 AND EXISTS ${ICON_COMPILE_DIR}/Assets.car)
    message(STATUS "App icon: Liquid Glass (actool) + flattened icns fallback")
    file(COPY_FILE ${ICON_COMPILE_DIR}/SpaghettiKartIcon.icns ${ICNS_FILE})
    set(ICON_BUNDLE_FILES ${CMAKE_BINARY_DIR}/macosx/Assets.car ${ICNS_FILE})
    file(COPY_FILE ${ICON_COMPILE_DIR}/Assets.car ${CMAKE_BINARY_DIR}/macosx/Assets.car)
    set(SPAGHETTI_ICON_HAS_GLASS TRUE)
else()
    message(STATUS "App icon: actool unavailable or lacks .icon support; flat icns from icon.png")
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
