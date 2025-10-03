
build_lib_example(
    NAME mycjson-network-example
    SOURCE_FILES mycjson-network-example.cc
    LIBRARIES_TO_LINK ${libmycjson}
                      ${libcore}
                      ${libnetwork}
                      ${libinternet}
                      ${libapplications}
                      ${libcsma}
)
