# Build examples for crypto-sim module

build_lib_example(
  NAME crypto-sim-example
  SOURCE_FILES crypto-sim-example.cc
  LIBRARIES_TO_LINK
    crypto-sim
    core
)