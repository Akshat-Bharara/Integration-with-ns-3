build_lib_example(
    NAME libuuid-integ-example
    SOURCE_FILES libuuid-integ-example.cc
    LIBRARIES_TO_LINK ${liblibuuid-integ}
                      ${libapplications}
                      ${libcsma}
                      ${libinternet}
)
