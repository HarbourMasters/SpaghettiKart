set(SEMVER_DIR ${CMAKE_BINARY_DIR}/_deps/semver)
file(
  DOWNLOAD
  "https://raw.githubusercontent.com/Neargye/semver/refs/tags/v1.0.1/include/semver.hpp"
  "${SEMVER_DIR}/semver.hpp"
  EXPECTED_HASH SHA256=4e55d937dc8500ecd38fc5c7b24177d57c135595b86b28ca3246cce180eca065
  TLS_VERIFY ON)
