if(ENABLE_TESTING)
  # Include the test subdirectory.
  message("-- Adding p4smith nic test suite")
  include(test/P4Tests.cmake)
endif()

# Source files for smith.
set(
  SMITH_SOURCES
  ${SMITH_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/expressions.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/target.cpp
  PARENT_SCOPE
)

