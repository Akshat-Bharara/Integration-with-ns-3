# Check for zlib availability
find_package(ZLIB REQUIRED)

if(NOT ZLIB_FOUND)
  message(STATUS "ZLIB not found, skipping zlib-integ module")
  return()
endif()

# Create a variable to hold libraries to link against
set(libraries_to_link ${ZLIB_LIBRARIES})

# Build the zlib-integ module
build_lib(
  LIBNAME zlib-integ
  SOURCE_FILES
    helper/zlib-integ-helper.cc
    model/zlib-integ.cc
  HEADER_FILES
    helper/zlib-integ-helper.h
    model/zlib-integ.h
  TEST_SOURCES
    test/zlib-integ-test-suite.cc
  LIBRARIES_TO_LINK
    ${libraries_to_link}
    core
    network
    internet
    csma
    applications
)

