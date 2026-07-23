if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /fp:fast)

  if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_options(${PROJECT_NAME} PRIVATE
      "$<$<CONFIG:Debug>:/w;/Od;/ZI>"
      "$<$<CONFIG:Release>:/O2;/Oi;/Gy;/W3;/Zi>" /permissive- /MP
      ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT} ${DEFAULT_CXX_EXCEPTION_HANDLING})
    target_link_options(${PROJECT_NAME} PRIVATE
      "$<$<CONFIG:Debug>:/INCREMENTAL>"
      "$<$<CONFIG:Release>:/OPT:REF;/OPT:ICF;/INCREMENTAL:NO;/FORCE:MULTIPLE>"
      /MANIFEST:NO /DEBUG /SUBSYSTEM:WINDOWS)
  elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    target_compile_options(${PROJECT_NAME} PRIVATE
      "$<$<CONFIG:Release>:/O2;/Oi;/Gy>"
      /permissive-
      /MP
      /w
      ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
      ${DEFAULT_CXX_EXCEPTION_HANDLING})
    target_link_options(${PROJECT_NAME} PRIVATE
      "$<$<CONFIG:Debug>:/STACK:8777216>"
      "$<$<CONFIG:Release>:/OPT:REF;/OPT:ICF;/INCREMENTAL:NO;/FORCE:MULTIPLE>"
      /MANIFEST:NO /DEBUG /SUBSYSTEM:WINDOWS)
  endif()

  # Remove /RTC from msvc flags
  foreach(fentry CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
    string(REGEX REPLACE "/RTC(su|[1su])" "" ${fentry} "${${fentry}}")
  endforeach()
else()
  target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wno-error
    -Wno-missing-field-initializers
    -Wno-parentheses
    -Wno-missing-braces
    -ffast-math
    -flto=auto
    -pipe)
  target_link_options(${PROJECT_NAME} PRIVATE -flto=auto)

  set(C_FLAGS -Werror-implicit-function-declaration
              -Wno-incompatible-pointer-types)
  target_compile_options(${PROJECT_NAME} PRIVATE
                         "$<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>")

  set(CXX_FLAGS -fpermissive -fomit-frame-pointer)
  target_compile_options(${PROJECT_NAME} PRIVATE
                         "$<$<COMPILE_LANGUAGE:CXX>:${CXX_FLAGS}>")

  include(CheckCCompilerFlag)
  include(CheckCXXCompilerFlag)

  check_c_compiler_flag("-Wno-error=int-conversion"
                        HAS_WNO_ERROR_INT_CONVERSION)
  if(HAS_WNO_ERROR_INT_CONVERSION)
    target_compile_options(${PROJECT_NAME} PRIVATE
      "$<$<COMPILE_LANGUAGE:C>:-Wno-error=int-conversion>")
  endif()

  check_cxx_compiler_flag("-Wno-error=narrowing" HAS_WNO_ERROR_NARROWING)
  if(HAS_WNO_ERROR_NARROWING)
    target_compile_options(
      ${PROJECT_NAME} PRIVATE
      "$<$<COMPILE_LANGUAGE:CXX>:-Wno-error=narrowing>")
  endif()

  check_cxx_compiler_flag("-Wno-error=changes-meaning"
                          HAS_WNO_ERROR_CHANGES_MEANING)
  if(HAS_WNO_ERROR_CHANGES_MEANING)
    target_compile_options(${PROJECT_NAME} PRIVATE
      "$<$<COMPILE_LANGUAGE:CXX>:-Wno-error=changes-meaning>")
  endif()

  target_compile_options(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:-g>" "$<$<CONFIG:Release>:-O3>"
    "$<$<CONFIG:MinSizeRel>:-Os>" "$<$<CONFIG:RelWithDebInfo>:-O2;-g>")

  check_cxx_compiler_flag("-pthread" HAS_PTHREAD)
  if(HAS_PTHREAD AND NOT CMAKE_SYSTEM_NAME STREQUAL "CafeOS")
    target_compile_options(${PROJECT_NAME} PRIVATE -pthread)
    target_link_options(${PROJECT_NAME} PRIVATE -pthread)
  endif()

  if(NOT APPLE
     AND NOT CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch"
     AND NOT CMAKE_SYSTEM_NAME STREQUAL "CafeOS")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
      check_cxx_compiler_flag("-msse2 -mfpmath=sse" HAS_SSE2)
      if(HAS_SSE2)
        target_compile_options(${PROJECT_NAME} PRIVATE -msse2 -mfpmath=sse)
      endif()
    endif()

    include(CheckLinkerFlag)
    check_linker_flag("CXX" "-Wl,-export-dynamic" HAS_EXPORT_DYNAMIC)
    if(HAS_EXPORT_DYNAMIC)
      target_link_options(${PROJECT_NAME} PRIVATE -Wl,-export-dynamic)
    endif()
  endif()

  # LTO breaks debug information with AppleClang and Ninja. Keep LTO for all
  # other configurations and explicitly override it for macOS Debug builds.
  if(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    target_compile_options(${PROJECT_NAME} PRIVATE
                           "$<$<CONFIG:Debug>:-fno-lto>")
    target_link_options(${PROJECT_NAME} PRIVATE
                        "$<$<CONFIG:Debug>:-fno-lto>")
  endif()
endif()

# Add compile definitions for the target
target_compile_definitions(${PROJECT_NAME} PRIVATE
  NDEBUG
  VERSION_US=1
  "$<$<BOOL:${USE_OPENGLES}>:USE_OPENGLES>"
  ENABLE_RUMBLE=1
  F3DEX_GBI=1
  _LANGUAGE_C
  _USE_MATH_DEFINES
  CIMGUI_DEFINE_ENUMS_AND_STRUCTS
  NON_MATCHING=1
  NON_EQUIVALENT=1
  AVOID_UB=1
  SPAGHETTI_VERSION="${PROJECT_VERSION}")

target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Debug>:_DEBUG>")

if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:ENABLE_DX11>"
    INCLUDE_GAME_PRINTF
    NOMINMAX
    UNICODE
    _UNICODE
    _CRT_SECURE_NO_WARNINGS
    _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
    STORMLIB_NO_AUTO_LINK)
  set(STORMLIB_NO_AUTO_LINK ON)
elseif(CMAKE_SYSTEM_NAME STREQUAL "CafeOS")
  target_compile_definitions(
    ${PROJECT_NAME} PRIVATE SPDLOG_ACTIVE_LEVEL=3 SPDLOG_NO_THREAD_ID
                            SPDLOG_NO_TLS STBI_NO_THREAD_LOCALS)
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU|Clang|AppleClang")
  target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<BOOL:${BUILD_CROWD_CONTROL}>:ENABLE_CROWD_CONTROL>"
    _CONSOLE _CRT_SECURE_NO_WARNINGS UNICODE _UNICODE)
endif()
