# Build examples for zlib-integ module

build_lib_example(
  NAME zlib-integ-example
  SOURCE_FILES zlib-integ-example.cc
  LIBRARIES_TO_LINK
    zlib-integ
    core
)
