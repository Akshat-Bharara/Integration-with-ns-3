check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

# Find or configure uthash (header-only library)
# Check if system has uthash installed
find_path(UTHASH_INCLUDE_DIR NAMES uthash.h)
if(NOT UTHASH_INCLUDE_DIR)
    # If not found, we'll use our bundled version
    set(UTHASH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
    
    # Create include directory if it doesn't exist
    file(MAKE_DIRECTORY ${UTHASH_INCLUDE_DIR})
    
    # Download uthash header if not already present
    if(NOT EXISTS "${UTHASH_INCLUDE_DIR}/uthash.h")
        message(STATUS "Downloading uthash.h to ${UTHASH_INCLUDE_DIR}")
        file(DOWNLOAD 
             "https://raw.githubusercontent.com/troydhanson/uthash/master/src/uthash.h"
             "${UTHASH_INCLUDE_DIR}/uthash.h"
             STATUS download_status)
        list(GET download_status 0 status_code)
        if(NOT status_code EQUAL 0)
            message(FATAL_ERROR "Failed to download uthash.h")
        endif()
    endif()
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources
        #test/uthash-integ-examples-test-suite.cc
        )
endif()

build_lib(
    LIBNAME uthash-integ
    SOURCE_FILES model/uthash-integ.cc
                 helper/uthash-integ-helper.cc
    HEADER_FILES model/uthash-integ.h
                 helper/uthash-integ-helper.h
    LIBRARIES_TO_LINK ${libcore}
                      ${libnetwork}
    TEST_SOURCES test/uthash-integ-test-suite.cc
                 ${examples_as_tests_sources}
)

# Add uthash include directories
target_include_directories(${libuthash-integ} PRIVATE ${UTHASH_INCLUDE_DIR})

# Build examples
build_lib_example(
    NAME uthash-point-to-point
    SOURCE_FILES uthash-point-to-point.cc
    LIBRARIES_TO_LINK ${libuthash-integ}
                      ${libcore}
                      ${libnetwork}
                      ${libinternet}
                      ${libapplications}
                      ${libpoint-to-point}
)
