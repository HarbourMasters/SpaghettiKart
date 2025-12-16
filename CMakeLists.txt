cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

# Set the project version and language
project(Spaghettify VERSION 0.1.0 LANGUAGES C CXX ASM)
include(FetchContent)

set(NATO_PHONETIC_ALPHABET
  "Alfa" "Bravo" "Charlie" "Delta" "Echo" "Foxtrot" "Golf" "Hotel"
  "India" "Juliett" "Kilo" "Lima" "Mike" "November" "Oscar" "Papa"
  "Quebec" "Romeo" "Sierra" "Tango" "Uniform" "Victor" "Whiskey"
  "Xray" "Yankee" "Zulu"
)

execute_process(
  COMMAND git config --global --add safe.directory ${CMAKE_SOURCE_DIR}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_QUIET
)

execute_process(
  COMMAND git describe --tags
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE PROJECT_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
if (DEFINED PROJECT_VERSION AND NOT "${PROJECT_VERSION}" STREQUAL "")
  string(REPLACE "-" ";" PROJECT_VERSION_LIST "${PROJECT_VERSION}")
  list(GET PROJECT_VERSION_LIST 0 PROJECT_VERSION_PATCH)
  string(REPLACE "." ";" PROJECT_VERSION_LIST "${PROJECT_VERSION_PATCH}")
  list(LENGTH PROJECT_VERSION_LIST PROJECT_VERSION_LIST_LENGTH)
  if (${PROJECT_VERSION_LIST_LENGTH} LESS 3)
    set(PROJECT_VERSION_PATCH 0)
  else()
    list(GET PROJECT_VERSION_LIST 2 PROJECT_VERSION_PATCH)
  endif()
else()
  set(PROJECT_VERSION "Unknown")
  execute_process(
    COMMAND git log --pretty=format:%h -1
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE PROJECT_VERSION_PATCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  if (DEFINED PROJECT_VERSION_PATCH AND NOT "${PROJECT_VERSION_PATCH}" STREQUAL "")
    set(PROJECT_VERSION "${PROJECT_VERSION}-${PROJECT_VERSION_PATCH}")
  endif()
  set(PROJECT_VERSION_PATCH 0)
endif()

# Get the patch version number from the project version
math(EXPR PATCH_INDEX "${PROJECT_VERSION_PATCH}")

# Use the patch number to select the correct word
list(GET NATO_PHONETIC_ALPHABET ${PATCH_INDEX} PROJECT_PATCH_WORD)

set(PROJECT_BUILD_NAME "Alfredo ${PROJECT_PATCH_WORD}" CACHE STRING "" FORCE)
set(PROJECT_TEAM "github.com/harbourmasters" CACHE STRING "" FORCE)

message("Spaghetti Kart version: ${PROJECT_VERSION} Alfredo ${PROJECT_PATCH_WORD}")

if(APPLE)
  enable_language(OBJCXX)
endif()

# Set the minimum version of CMake and the deployment target for macOS
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")

# Set CMake to build as Universal Binary for macOS
set(CMAKE_OSX_ARCHITECTURES=arm64;x86_64)

# Set the C++ standard and enable the MSVC parallel build option
set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11 CACHE STRING "The C standard to use")
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Spaghettify)
set(PROJECT_TEAM "MegaMech")
#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

# Add a custom module path to locate additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set(YAML_CPP_STATIC_DEFINE ON)
add_compile_definitions(YAML_CPP_STATIC_DEFINE)
include(cmake/lus-cvars.cmake)

if (WIN32)
include(cmake/automate-vcpkg.cmake)
set(VCPKG_TRIPLET x64-windows-static)
set(VCPKG_TARGET_TRIPLET x64-windows-static)
vcpkg_bootstrap()
vcpkg_install_packages()

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

if (MSVC)
set(CPP "${CMAKE_C_COMPILER}" "/EP")
else()
set(CPP "${CMAKE_C_COMPILER}" "-E" "-P" "-Wno-trigraphs" "-x" "c")
endif()

if(IOS)
set(PLATFORM "OS64")
include(cmake/ios.toolchain.cmake)
add_compile_definitions(
    PLATFORM_IOS=1
)
set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "YOUR_TEAM_ID")
set(PROJECT_ID "dev.net64.game")
endif()

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(NOT CMAKE_VS_PLATFORM_NAME)
        set(CMAKE_VS_PLATFORM_NAME "x64")
    endif()
    message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

    if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"
        OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32"))
        message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
    endif()
endif()

################################################################################
# Compiler specific configuration types
################################################################################
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU|Clang|AppleClang")
    set(COMMON_FLAGS "-Wall -Wextra -Wno-error \
            -Wno-return-type \
            -Wno-unused-parameter \
            -Wno-unused-function \
            -Wno-unused-variable \
            -Wno-missing-field-initializers \
            -Wno-parentheses \
            -Wno-narrowing \
            -Wno-missing-braces \
            -ffast-math \
            -flto=auto \
            -pipe"
    )

    set(COMMON_C_FLAGS "${COMMON_FLAGS} -Wno-int-conversion -Werror-implicit-function-declaration -Wno-incompatible-pointer-types")
    set(COMMON_CXX_FLAGS "${COMMON_FLAGS} -Wno-c++11-narrowing -Wno-deprecated-enum-enum-conversion -fpermissive -fomit-frame-pointer")
elseif (MSVC)
    set(COMMON_FLAGS "/fp:fast")
    set(COMMON_C_FLAGS "${COMMON_FLAGS}")
    set(COMMON_CXX_FLAGS "${COMMON_FLAGS}")
else()
    set(COMMON_C_FLAGS "")
    set(COMMON_CXX_FLAGS "")
endif()

# fix LTO breaks debug information on Apple/Ninja https://gitlab.kitware.com/cmake/cmake/-/issues/25202
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "AppleClang" AND "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_link_options("-fno-lto")
    add_compile_options("-fno-lto")
endif()

################################################################################
# Global configuration types
################################################################################
if (MSVC)
    set(CMAKE_C_FLAGS_DEBUG "/Od /Zi /DNDEBUG ${COMMON_C_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi /DNDEBUG ${COMMON_CXX_FLAGS}")
    set(CMAKE_C_FLAGS_RELEASE "/O2 /DNDEBUG ${COMMON_C_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG ${COMMON_CXX_FLAGS}")
else()
    set(CMAKE_C_FLAGS_DEBUG "-g -DNDEBUG ${COMMON_C_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -DNDEBUG ${COMMON_CXX_FLAGS}")
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG ${COMMON_C_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG ${COMMON_CXX_FLAGS}")
    set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG ${COMMON_C_FLAGS}")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG ${COMMON_CXX_FLAGS}")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG ${COMMON_C_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG ${COMMON_CXX_FLAGS}")
endif()

# Set game compilation version
set(VERSION us)
set(USE_NETWORKING OFF)
set(SKIP_XCODE_VERSION_CHECK ON)

# TODO: Sorry i broke this
set(GFX_DEBUG_DISASSEMBLER OFF)

# Add compile definitions for the target
add_compile_definitions(
    VERSION_US=1
    ENABLE_RUMBLE=1
    F3DEX_GBI=1
    _LANGUAGE_C
    _USE_MATH_DEFINES
    CIMGUI_DEFINE_ENUMS_AND_STRUCTS
    NON_MATCHING=1
    NON_EQUIVALENT=1
	AVOID_UB=1
	SPAGHETTI_VERSION="${PROJECT_VERSION}"
)

# Find necessary libraries
if (UNIX AND NOT APPLE)
find_package(OpenGL REQUIRED)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/properties.h.in ${CMAKE_CURRENT_SOURCE_DIR}/properties.h @ONLY)
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/assets
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/racing
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ending
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    ${CMAKE_CURRENT_SOURCE_DIR}/src/buffers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/audio
    ${CMAKE_CURRENT_SOURCE_DIR}/src/debug
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/tracks
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/objects
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/particles
    ${CMAKE_CURRENT_SOURCE_DIR}/src/enhancements
    ${CMAKE_CURRENT_SOURCE_DIR}/src/enhancements/freecam
)

# Collect source files to build the executable
file(GLOB ALL_FILES_ROOT RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "src/*.c"
    "src/*.h"
    "src/os/guLookAtF.c"
    "src/os/guPerspectiveF.c"
    "src/os/guMtxF2L.c"
    "src/os/guRotateF.c"
    "src/os/guOrthoF.c"
    "src/os/guScaleF.c"
    "src/os/guTranslateF.c"
    "src/os/guMtxCatF.c"
    "src/os/guMtxCatL.c"
    "src/os/guNormalize.c"
)

file(GLOB_RECURSE ALL_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "src/buffers/*.c"
    "src/buffers/*.h"
    "src/data/*.c"
    "src/data/*.h"
    "src/debug/*.c"
    "src/debug/*.h"
    "src/ending/*.c"
    "src/ending/*.h"
    "src/racing/*.c"
    "src/racing/*.h"
    "src/audio/*.c"
    "src/audio/*.h"
    "src/port/*.h"
    "src/port/*.c"
    "src/port/*.cpp"
    "src/mods/*.h"
    "src/mods/*.c"
    "src/mods/*.cpp"
    "assets/code/*.c"
    "courses/*.c"
    "courses/*.h"
    "src/engine/*.cpp"
    "src/engine/*.h"
    "src/engine/tracks/*.c"
    "src/engine/tracks/*.h"
    "src/engine/objects/*.cpp"
    "src/engine/objects/*.h"
    "src/engine/particles/*.cpp"
    "src/engine/particles/*.h"
    "src/enhancements/*.c"
    "src/enhancements/*.h"
    "src/enhancements/freecam/*.c"
    "src/enhancements/freecam/*.cpp"
    "src/enhancements/freecam/*.h"
    "src/engine/*.cpp"
    "src/engine/*.h"
    "Resource.rc"
)

list(APPEND ALL_FILES ${ALL_FILES_ROOT})

# Exclude specific files from the ALL_FILES list
list(FILTER ALL_FILES EXCLUDE REGEX ".*.inc.c")
list(FILTER ALL_FILES EXCLUDE REGEX "./src/debug/crash_screen_enhancement.c")

if (CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(IOS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libultraship/ios)

    set(STORYBOARD_FILE ${IOS_DIR}/Launch.storyboard)
    set(IMAGE_FILES ${IOS_DIR}/PoweredBy.png)
    set(ICON_FILES ${IOS_DIR}/Icon.png)

    list(APPEND ALL_FILES ${STORYBOARD_FILE} ${IMAGE_FILES} ${ICON_FILES})

    add_executable(${PROJECT_NAME} ${ALL_FILES})
    set_xcode_property(${PROJECT_NAME} PRODUCT_BUNDLE_IDENTIFIER ${PROJECT_ID} All)
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${IOS_DIR}/plist.in
        RESOURCE "${IMAGE_FILES};${STORYBOARD_FILE};${ICON_FILES}"
    )
else()
    add_executable(${PROJECT_NAME} ${ALL_FILES})
endif()


################################################################################
# MSVC runtime library
################################################################################
if (MSVC)
  if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	  get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
	  if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
		  string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
			  $<$<CONFIG:Debug>:
                MultiThreadedDebug
			  >
			  $<$<CONFIG:Release>:
                MultiThreaded
			  >
			  $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
		  )
	  elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
		  string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
			  $<$<CONFIG:Debug>:
				  MultiThreadedDebug
			  >
			  $<$<CONFIG:Release>:
				  MultiThreaded
			  >
			  $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
		  )
	  endif()
	  set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})
  endif()
endif()

#=================== SSE2NEON ===================
set(SSE2NEON_DIR ${CMAKE_BINARY_DIR}/_deps/sse2neon)
file(DOWNLOAD "https://raw.githubusercontent.com/DLTcollab/sse2neon/refs/heads/master/sse2neon.h" "${SSE2NEON_DIR}/sse2neon.h")

include_directories(${SSE2NEON_DIR})

#================== SEMVER ===================
set(SEMVER_DIR ${CMAKE_BINARY_DIR}/_deps/semver)
file(DOWNLOAD "https://raw.githubusercontent.com/Neargye/semver/refs/tags/v1.0.0-rc/include/semver.hpp" "${SEMVER_DIR}/semver.hpp")

include_directories(${SEMVER_DIR})

#=================== DRLibs ===================
FetchContent_Declare(
    dr_libs
    GIT_REPOSITORY https://github.com/mackron/dr_libs.git
	GIT_TAG da35f9d6c7374a95353fd1df1d394d44ab66cf01
)
FetchContent_MakeAvailable(dr_libs)

#=================== STB ===================
include_directories(${STB_DIR})

#=================== tomlplusplus ===================
include(FetchContent)
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)
target_link_libraries(${PROJECT_NAME} PRIVATE tomlplusplus::tomlplusplus)

#==============================================================================#
# Libultraship Integration                                                     #
#==============================================================================#

# Removes MPQ/OTR support
set(EXCLUDE_MPQ_SUPPORT TRUE CACHE BOOL "")
add_compile_definitions(EXCLUDE_MPQ_SUPPORT)

if(CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
    find_package(SDL2)
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/libultraship/include
  ${CMAKE_CURRENT_SOURCE_DIR}/libultraship/include/libultraship
  ${SDL2_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${dr_libs_SOURCE_DIR}
)

add_subdirectory(libultraship ${CMAKE_CURRENT_SOURCE_DIR}/libultraship)
add_dependencies(${PROJECT_NAME} libultraship)
target_link_libraries(${PROJECT_NAME} PRIVATE libultraship)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG;"
            "_CRT_SECURE_NO_WARNINGS;"
            "ENABLE_DX11;"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "INCLUDE_GAME_PRINTF;"
        "NOMINMAX"
        "UNICODE;"
        "_UNICODE;"
        "_CRT_SECURE_NO_WARNINGS;"
        "_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS;"
    )
    add_compile_definitions(STORMLIB_NO_AUTO_LINK)
	set(STORMLIB_NO_AUTO_LINK ON)
elseif (CMAKE_SYSTEM_NAME STREQUAL "CafeOS")
	target_compile_definitions(${PROJECT_NAME} PRIVATE
		"$<$<CONFIG:Debug>:"
			"_DEBUG"
		">"
		"$<$<CONFIG:Release>:"
			"NDEBUG"
		">"
        "SPDLOG_ACTIVE_LEVEL=3;"
		"SPDLOG_NO_THREAD_ID;"
        "SPDLOG_NO_TLS;"
		"STBI_NO_THREAD_LOCALS;"
	)
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU|Clang|AppleClang")
	target_compile_definitions(${PROJECT_NAME} PRIVATE
		"$<$<CONFIG:Debug>:"
			"_DEBUG"
		">"
		"$<$<CONFIG:Release>:"
			"NDEBUG"
		">"
        "$<$<BOOL:${BUILD_CROWD_CONTROL}>:ENABLE_CROWD_CONTROL>"
		"SPDLOG_ACTIVE_LEVEL=0;"
		"_CONSOLE;"
		"_CRT_SECURE_NO_WARNINGS;"
		"UNICODE;"
		"_UNICODE"
	)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    find_package(Ogg CONFIG REQUIRED)
    link_libraries(Ogg::ogg)

    find_package(Vorbis CONFIG REQUIRED)
    link_libraries(Vorbis::vorbisfile)
	set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "Ogg::ogg"
        "Vorbis::vorbis"
        "Vorbis::vorbisenc"
        "Vorbis::vorbisfile"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        -lglad
        SDL2::SDL2
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "CafeOS")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Debug>:-Wl,--wrap=abort>"
    )
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${DEVKITPRO}/portlibs/wiiu/include/
    )
else()
    find_package(Ogg REQUIRED)
    find_package(Vorbis REQUIRED)
 	set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "Ogg::ogg"
        "Vorbis::vorbis"
        "Vorbis::vorbisenc"
        "Vorbis::vorbisfile"
	)
endif()


if(USE_NETWORKING)
    if(MSVC)
        vcpkg_install_packages(sdl2-net)
    endif()
    include_directories(${SDL2_NET_INCLUDE_DIRS})
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_NETWORKING)
    if(CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_net)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        find_package(SDL2_net REQUIRED)
        target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_net::SDL2_net>,SDL2_net::SDL2_net,SDL2_net::SDL2_net-static>) 
    else()
        find_package(SDL2_net REQUIRED)
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_net::SDL2_net-static)
    endif()
endif()

option(USE_STANDALONE "Build as a standalone executable" OFF)
option(BUILD_STORMLIB "Build with StormLib support" OFF)

option(BUILD_SM64 "Build with Super Mario 64 support" OFF)
option(BUILD_SF64 "Build with Star Fox 64 support" OFF)
option(BUILD_FZERO "Build with F-Zero X support" OFF)

add_subdirectory(torch)
target_link_libraries(${PROJECT_NAME} PRIVATE torch "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if(CMAKE_SYSTEM_NAME MATCHES "NintendoSwitch")

nx_generate_nacp(${PROJECT_NAME}.nacp
   NAME "${PROJECT_NAME}"
   AUTHOR "${PROJECT_TEAM}"
   VERSION "${PROJECT_VERSION}"
)

nx_create_nro(${PROJECT_NAME}
    NACP ${PROJECT_NAME}.nacp
    ICON ${CMAKE_CURRENT_SOURCE_DIR}/icon.png
)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.nro DESTINATION . COMPONENT ${PROJECT_NAME})
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
  if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
      target_compile_options(${PROJECT_NAME} PRIVATE
          $<$<CONFIG:Debug>:
              /w;
              /Od;
              /MTd
          >
          $<$<CONFIG:Release>:
              /Oi;
              /Gy;
              /W3;
              /MT
          >
          /permissive-;
          /MP;
          ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
          ${DEFAULT_CXX_EXCEPTION_HANDLING}
      )
      target_compile_options(${PROJECT_NAME} PRIVATE  $<$<CONFIG:Debug>:/ZI;>)
      target_compile_options(${PROJECT_NAME} PRIVATE  $<$<CONFIG:Release>:/Zi;>)
  elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
      target_compile_options(${PROJECT_NAME} PRIVATE
          $<$<CONFIG:Debug>:
              /MTd
          >
          $<$<CONFIG:Release>:
              /O2;
              /Oi;
              /Gy;
              /MT
          >
          /permissive-;
          /MP;
          /w;
          ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
          ${DEFAULT_CXX_EXCEPTION_HANDLING}
      )
  endif()
  if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
      target_link_options(${PROJECT_NAME} PRIVATE
          $<$<CONFIG:Debug>:
              /INCREMENTAL
          >
          $<$<CONFIG:Release>:
              /OPT:REF;
              /OPT:ICF;
              /INCREMENTAL:NO;
              /FORCE:MULTIPLE
          >
          /MANIFEST:NO;
          /DEBUG;
          /SUBSYSTEM:WINDOWS
      )
  elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
      target_link_options(${PROJECT_NAME} PRIVATE
          $<$<CONFIG:Debug>:
              /STACK:8777216
          >
          $<$<CONFIG:Release>:
              /OPT:REF;
              /OPT:ICF;
              /INCREMENTAL:NO;
              /FORCE:MULTIPLE
          >
          /MANIFEST:NO;
          /DEBUG;
          /SUBSYSTEM:WINDOWS
      )
  endif()

  # Remove /RTC from msvc flags
  foreach (fentry
    CMAKE_C_FLAGS
    CMAKE_CXX_FLAGS
  )
    string (REGEX REPLACE "/RTC(su|[1su])" "" ${fentry} "${${fentry}}")
  endforeach(fentry)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
        target_compile_options(${PROJECT_NAME} PRIVATE
            -pthread
        )

        target_link_options(${PROJECT_NAME} PRIVATE
            -pthread
        )
    elseif (CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
      target_compile_options(${PROJECT_NAME} PRIVATE
          -pthread
      )

      target_link_options(${PROJECT_NAME} PRIVATE
          -pthread
      )
  elseif (NOT CMAKE_SYSTEM_NAME STREQUAL "CafeOS")
      if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
          set(CPU_OPTION -msse2 -mfpmath=sse)
      endif()

      target_compile_options(${PROJECT_NAME} PRIVATE
          -pthread
          ${CPU_OPTION}
      )

      target_link_options(${PROJECT_NAME} PRIVATE
          -pthread
          -Wl,-export-dynamic
      )
  endif()
endif()

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMENT "Copying asset yamls..."
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/config.yml" "$<TARGET_FILE_DIR:Spaghettify>/config.yml"
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/yamls/" "$<TARGET_FILE_DIR:Spaghettify>/yamls/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/meta/" "$<TARGET_FILE_DIR:Spaghettify>/meta/"
)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
include(ExternalProject)
ExternalProject_Add(TorchExternal
    PREFIX TorchExternal
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/torch
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/torch
)

ExternalProject_Get_Property(TorchExternal install_dir)
if (MSVC)
  set(TORCH_EXECUTABLE ${install_dir}/src/TorchExternal-build/$<CONFIGURATION>/torch)
else()
  set(TORCH_EXECUTABLE ${install_dir}/src/TorchExternal-build/torch)
endif()

add_custom_target(
    ExtractAssets
    DEPENDS TorchExternal
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${TORCH_EXECUTABLE} header -o baserom.us.z64
    COMMAND ${TORCH_EXECUTABLE} o2r baserom.us.z64 --additional-files meta/mods.toml
    COMMAND ${TORCH_EXECUTABLE} pack assets spaghetti.o2r o2r
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/mk64.o2r" "${CMAKE_BINARY_DIR}/mk64.o2r"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/spaghetti.o2r" "${CMAKE_BINARY_DIR}/spaghetti.o2r"
)

add_custom_target(
    GenerateO2R
    DEPENDS TorchExternal
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${TORCH_EXECUTABLE} pack assets spaghetti.o2r o2r
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/spaghetti.o2r" "${CMAKE_BINARY_DIR}/spaghetti.o2r"
)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  install(FILES "${CMAKE_BINARY_DIR}/spaghetti.o2r" DESTINATION . COMPONENT ${PROJECT_NAME})
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
add_custom_target(CreateOSXIcons
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/macosx/spaghettikart.iconset
    COMMAND sips -z 16 16     icon.png --out ${CMAKE_BINARY_DIR}/macosx/spaghettikart.iconset/icon_16x16.png
    COMMAND sips -z 32 32     icon.png --out ${CMAKE_BINARY_DIR}/macosx/spaghettikart.iconset/icon_16x16@2x.png
    COMMAND sips -z 32 32     icon.png --out ${CMAKE_BINARY_DIR}/macosx/spaghettikart.iconset/icon_32x32.png
    COMMAND sips -z 64 64     icon.png --out ${CMAKE_BINARY_DIR}/macosx/spaghettikart.iconset/icon_32x32@2x.png
    COMMAND sips -z 128 128   icon.png --out ${CMAKE_BINARY_DIR}/macosx/spaghettikart.iconset/icon_128x128.png
    COMMAND sips -z 256 256   icon.png --out ${CMAKE_BINARY_DIR}/macosx/spaghettikart.iconset/icon_128x128@2x.png
    COMMAND sips -z 256 256   icon.png --out ${CMAKE_BINARY_DIR}/macosx/spaghettikart.iconset/icon_256x256.png
    COMMAND sips -z 512 512   icon.png --out ${CMAKE_BINARY_DIR}/macosx/spaghettikart.iconset/icon_256x256@2x.png
    COMMAND sips -z 512 512   icon.png --out ${CMAKE_BINARY_DIR}/macosx/spaghettikart.iconset/icon_512x512.png
    COMMAND cp                icon.png ${CMAKE_BINARY_DIR}/macosx/spaghettikart.iconset/icon_512x512@2x.png
    COMMAND iconutil -c icns -o ${CMAKE_BINARY_DIR}/macosx/spaghettikart.icns ${CMAKE_BINARY_DIR}/macosx/spaghettikart.iconset
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Creating OSX icons ..."
    )
    add_dependencies(${PROJECT_NAME} CreateOSXIcons)
    configure_file("${CMAKE_SOURCE_DIR}/Info.plist" "${CMAKE_BINARY_DIR}/Info.plist" COPYONLY)
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY APPIMAGE_DESKTOP_FILE_TERMINAL YES)
set_property(TARGET ${PROJECT_NAME} PROPERTY APPIMAGE_DESKTOP_FILE "${CMAKE_SOURCE_DIR}/SpaghettiKart.desktop")
set_property(TARGET ${PROJECT_NAME} PROPERTY APPIMAGE_ICON_FILE "${CMAKE_SOURCE_DIR}/icon.png")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CPACK_GENERATOR "External")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows|NintendoSwitch|CafeOS")
    set(CPACK_GENERATOR "ZIP")
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(CPACK_GENERATOR "Bundle")
endif()

set(CPACK_PROJECT_CONFIG_FILE ${CMAKE_SOURCE_DIR}/cmake/configure-packaging.cmake)
include(cmake/packaging.cmake)
