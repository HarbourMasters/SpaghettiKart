if(WIN32)
  find_package(Ogg CONFIG REQUIRED)
  find_package(Vorbis CONFIG REQUIRED)
elseif(ANDROID)
  # Nothing in the game, libultraship or Torch links against Ogg/Vorbis, and the
  # NDK has no system copy, so don't drag them in. SDL2 comes from
  # libultraship/cmake/dependencies/android.cmake.
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  find_library(ANDROID_LOG_LIBRARY log REQUIRED)
  find_library(ANDROID_LIBRARY android REQUIRED)
  set(ADDITIONAL_LIBRARY_DEPENDENCIES SDL2::SDL2 Threads::Threads
                                      ${ANDROID_LIBRARY} ${ANDROID_LOG_LIBRARY})
elseif(CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
  set(ADDITIONAL_LIBRARY_DEPENDENCIES -lglad SDL2::SDL2)
elseif(CMAKE_SYSTEM_NAME STREQUAL "CafeOS")
  set(ADDITIONAL_LIBRARY_DEPENDENCIES "$<$<CONFIG:Debug>:-Wl,--wrap=abort>")
  target_include_directories(${PROJECT_NAME} PRIVATE
                             ${DEVKITPRO}/portlibs/wiiu/include/)
else()
  find_package(Ogg REQUIRED)
  find_package(Vorbis REQUIRED)
endif()

if(NOT CMAKE_SYSTEM_NAME MATCHES "NintendoSwitch|CafeOS" AND NOT ANDROID)
  set(ADDITIONAL_LIBRARY_DEPENDENCIES Ogg::ogg Vorbis::vorbis
                                      Vorbis::vorbisenc Vorbis::vorbisfile)
endif()

# Android's GL comes from libultraship, which picks GLESv3 there; the game
# itself makes no GL calls, so don't pull a second GLES version in alongside it.
if(UNIX AND NOT APPLE AND NOT ANDROID)
  if(USE_OPENGLES)
    find_library(GLESv2_LIBRARY GLESv2 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GLESv2_LIBRARY})
  else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
  endif()
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch")
  find_package(SDL2)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})

if(NOT USE_OPENGLES)
  target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIRS})
endif()

target_link_libraries(${PROJECT_NAME}
                      PRIVATE torch ${ADDITIONAL_LIBRARY_DEPENDENCIES})
