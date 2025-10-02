# Find the libuuid library
find_library(UUID_LIBRARY uuid)
if(NOT UUID_LIBRARY)
    message(FATAL_ERROR "libuuid library not found. Please install libuuid-dev (Ubuntu/Debian) or libuuid-devel (Red Hat/CentOS)")
endif()

check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources
        #test/libuuid-integ-examples-test-suite.cc
        )
endif()

build_lib(
    LIBNAME libuuid-integ
    SOURCE_FILES model/libuuid-integ.cc
                 helper/libuuid-integ-helper.cc
    HEADER_FILES model/libuuid-integ.h
                 helper/libuuid-integ-helper.h
    LIBRARIES_TO_LINK ${libcore}
                      ${UUID_LIBRARY}
    TEST_SOURCES test/libuuid-integ-test-suite.cc
                 ${examples_as_tests_sources}
)
