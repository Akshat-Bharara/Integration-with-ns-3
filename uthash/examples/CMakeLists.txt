build_lib_example(
    NAME uthash-integ-example
    SOURCE_FILES uthash-integ-example.cc
    LIBRARIES_TO_LINK ${libuthash-integ}
                      ${libcore}
                      ${libnetwork}
                      ${libinternet}
                      ${libapplications}
                      ${libpoint-to-point}
)
