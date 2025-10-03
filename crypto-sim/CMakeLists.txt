# Check for Crypto++ availability
find_path(CRYPTOPP_INCLUDE_DIR cryptopp/aes.h
    PATHS /usr/include /usr/local/include /opt/homebrew/include
)

find_library(CRYPTOPP_LIBRARY cryptopp
    PATHS /usr/lib /usr/local/lib /opt/homebrew/lib
)

if (NOT CRYPTOPP_INCLUDE_DIR OR NOT CRYPTOPP_LIBRARY)
  message(STATUS "Crypto++ not found, skipping crypto-sim module")
  return()
endif()

# Create a variable to hold libraries to link against
set(libraries_to_link ${CRYPTOPP_LIBRARY})

# Build the crypto-sim module
build_lib(
  LIBNAME crypto-sim
  SOURCE_FILES
    helper/crypto-sim-helper.cc
    model/crypto-sim.cc
  HEADER_FILES
    helper/crypto-sim-helper.h
    model/crypto-sim.h
  TEST_SOURCES
    test/crypto-sim-test-suite.cc
  LIBRARIES_TO_LINK
    ${libraries_to_link}
    core
    network
    internet
    point-to-point
    applications
)

# Add include directory for Crypto++
target_include_directories(crypto-sim PRIVATE ${CRYPTOPP_INCLUDE_DIR})