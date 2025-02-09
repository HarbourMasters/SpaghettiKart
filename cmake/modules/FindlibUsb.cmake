if (MSVC)
   set(WINDOWS_LIBUSB_PATH "$ENV{LIBUSB_PATH}/VS2019/MS64/static")

   # check if we're using something else than 64bit..
   if (NOT "${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
      set(WINDOWS_LIBUSB_PATH "$ENV{LIBUSB_PATH}/VS2019/MS32/static")
   endif()
endif()

if (MINGW)
   set(WINDOWS_LIBUSB_PATH "$ENV{LIBUSB_PATH}/MinGW64/static")
   # check if we're using something else than 64bit..
   if (NOT "${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
      set(WINDOWS_LIBUSB_PATH "$ENV{LIBUSB_PATH}/MinGW32/static")
   endif()
endif()

find_library (LIBUSB_LIBRARY
   NAMES libusb libusb-1.0 usb-1.0
   PATHS "/usr/lib" "/usr/local/lib/" "${WINDOWS_LIBUSB_PATH}")

find_path (LIBUSB_INCLUDEDIR
   NAMES libusb.h libusb-1.0.h
   PATHS "/usr/local/include/" "$ENV{LIBUSB_PATH}/include/libusb-1.0"
   PATH_SUFFIXES "include" "libusb" "libusb-1.0")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(libUsb DEFAULT_MSG
                                  LIBUSB_LIBRARY
                                  LIBUSB_INCLUDEDIR)

if (LIBUSB_FOUND AND NOT TARGET libUsb::libUsb)
   add_library(libUsb::libUsb STATIC IMPORTED)
   set_target_properties(
      libUsb::libUsb
      PROPERTIES
         INTERFACE_INCLUDE_DIRECTORIES "${LIBUSB_INCLUDEDIR}"
         IMPORTED_LOCATION "${LIBUSB_LIBRARY}")
   if (MSVC OR MINGW)
      set_target_properties(
         libUsb::libUsb
         PROPERTIES
            IMPORTED_IMPLIB "${LIBUSB_LIBRARY}"
      )
   endif()
endif()

