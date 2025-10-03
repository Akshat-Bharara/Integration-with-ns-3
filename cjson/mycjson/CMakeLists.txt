check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

# Find cJSON library
find_package(PkgConfig REQUIRED)
pkg_check_modules(CJSON REQUIRED libcjson)

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources
        #test/mycjson-examples-test-suite.cc
        )
endif()

build_lib(
    LIBNAME mycjson
    SOURCE_FILES model/mycjson.cc
                 helper/mycjson-helper.cc
    HEADER_FILES model/mycjson.h
                 helper/mycjson-helper.h
    LIBRARIES_TO_LINK ${libcore}
                      ${libnetwork}
                      ${CJSON_LIBRARIES}
    TEST_SOURCES test/mycjson-test-suite.cc
                 ${examples_as_tests_sources}
)

# Add cJSON include directories
target_include_directories(${libmycjson} PRIVATE ${CJSON_INCLUDE_DIRS})
target_compile_options(${libmycjson} PRIVATE ${CJSON_CFLAGS_OTHER})
