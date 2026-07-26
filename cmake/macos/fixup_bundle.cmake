# Run at build time (cmake -P) to copy non-system dylibs the executable links
# against into the .app's Contents/Frameworks and rewrite the load paths, making
# the bundle self-contained / portable. Invoked from apple_bundle.cmake.
#
# Expects -D APP_BUNDLE=<abs path to SpaghettiKart.app> -D EXECUTABLE_NAME=SpaghettiKart

include(BundleUtilities)

set(APP "${APP_BUNDLE}/Contents/MacOS/${EXECUTABLE_NAME}")
file(REAL_PATH "${APP}" APP)

# Where fixup_bundle should look for the dependencies it needs to copy in.
set(SEARCH_DIRS
    "/opt/homebrew/lib"          # Homebrew (Apple Silicon)
    "/usr/local/lib"             # Homebrew (Intel) / misc
    "/opt/local/lib"             # MacPorts
    "${APP_BUNDLE}/Contents/Frameworks"
)

set(RESOLVED_DIRS "")
foreach(DIR IN LISTS SEARCH_DIRS)
    if (EXISTS "${DIR}")
        file(REAL_PATH "${DIR}" RESOLVED_DIR)
        list(APPEND RESOLVED_DIRS "${RESOLVED_DIR}")
    endif()
endforeach()

message(STATUS "fixup_bundle: ${APP}")
message(STATUS "fixup_bundle search dirs: ${RESOLVED_DIRS}")

fixup_bundle("${APP}" "" "${RESOLVED_DIRS}")
