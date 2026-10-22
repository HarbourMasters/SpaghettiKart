cmake_minimum_required(VERSION 3.16)
project(ModMetadataTests LANGUAGES CXX)

enable_testing()
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/dependencies/semver.cmake)

set(TOML_DIR ${CMAKE_BINARY_DIR}/_deps/tomlplusplus)
file(
  DOWNLOAD
  "https://raw.githubusercontent.com/marzer/tomlplusplus/v3.4.0/toml.hpp"
  "${TOML_DIR}/toml++/toml.hpp"
  EXPECTED_HASH SHA256=6b5172ad4dd6519aec67b919181fa7a38a2234131e5b2afa232dfe444819783e
  TLS_VERIFY ON)

add_executable(mod-metadata-tests ModMetadataTests.cpp)
target_compile_features(mod-metadata-tests PRIVATE cxx_std_20)
target_include_directories(mod-metadata-tests PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../../src/engine/mods
  ${SEMVER_DIR}
  ${TOML_DIR})
add_test(NAME mod-metadata COMMAND mod-metadata-tests)
