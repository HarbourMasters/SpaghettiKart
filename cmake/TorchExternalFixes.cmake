# Included in the TorchExternal sub-build via CMAKE_PROJECT_INCLUDE (runs right
# after Torch's project() call).
#
# Torch's pinned spdlog bundles an fmt whose consteval format-string checking
# fails to compile under newer AppleClang (Xcode 16+):
#   error: call to consteval function 'fmt::basic_format_string<...>' is not a
#   constant expression
# Defining FMT_CONSTEVAL to empty falls back to fmt's pre-C++20 constexpr
# checking, which compiles cleanly. Scoped to AppleClang so other platforms and
# compilers are untouched. add_compile_definitions (rather than CMAKE_CXX_FLAGS)
# because Torch's CMakeLists overwrites the latter.
# TODO: Temporary hack. Delete this file (plus the CMAKE_PROJECT_INCLUDE line
# in CMakeLists.txt and the vcpkg.json fmt/spdlog pins) once the Torch pin is
# bumped past Torch's spdlog/fmt update. Note the bump also needs matching
# MK64 loader changes, see the discussion on PR #712.
if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_definitions("FMT_CONSTEVAL=")
endif()
