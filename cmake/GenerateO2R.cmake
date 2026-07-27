foreach(REQUIRED_VARIABLE TORCH_EXECUTABLE SOURCE_DIR BINARY_DIR)
  if(NOT DEFINED ${REQUIRED_VARIABLE})
    message(FATAL_ERROR "${REQUIRED_VARIABLE} is required")
  endif()
endforeach()

file(LOCK "${BINARY_DIR}/spaghetti-o2r.lock" GUARD PROCESS TIMEOUT 300)
execute_process(
  COMMAND "${TORCH_EXECUTABLE}" pack assets spaghetti.o2r o2r
  WORKING_DIRECTORY "${SOURCE_DIR}"
  RESULT_VARIABLE GENERATE_RESULT)
if(NOT GENERATE_RESULT EQUAL 0)
  message(FATAL_ERROR "Torch failed to generate spaghetti.o2r")
endif()

execute_process(
  COMMAND "${CMAKE_COMMAND}" -E copy_if_different
          "${SOURCE_DIR}/spaghetti.o2r" "${BINARY_DIR}/spaghetti.o2r"
  RESULT_VARIABLE COPY_RESULT)
if(NOT COPY_RESULT EQUAL 0)
  message(FATAL_ERROR "Failed to stage spaghetti.o2r")
endif()
