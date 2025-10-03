# Crypto-Sim Module

## Overview

The crypto-sim module provides cryptographic functionality for NS-3 simulations using the Crypto++ library. This module enables secure communication simulation by providing AES encryption and decryption capabilities.

## Features

- AES encryption in CBC mode
- Automatic key and IV generation
- Seamless integration with NS-3 applications
- Support for arbitrary data encryption/decryption

## Dependencies

- Crypto++ library (installed locally on the system)

## Installation

1. Ensure Crypto++ is installed on your system:
   - Ubuntu/Debian: `sudo apt-get install libcrypto++-dev`
   - macOS with Homebrew: `brew install cryptopp`
   - CentOS/RHEL: `sudo yum install cryptopp-devel`

2. The module will be automatically built if Crypto++ is detected during NS-3 configuration.

## Usage

### Basic Example

```cpp
#include "ns3/crypto-sim.h"

// Create crypto instance
Ptr<CryptoSim> crypto = CreateObject<CryptoSim>();

// Encrypt data
std::vector<uint8_t> plaintext = {'H', 'e', 'l', 'l', 'o'};
std::vector<uint8_t> encrypted = crypto->Encrypt(plaintext);

// Decrypt data
std::vector<uint8_t> decrypted = crypto->Decrypt(encrypted);
```

### Integration with Applications

The encrypted data can be used as payload in any NS-3 application, providing secure communication simulation capabilities.

## Examples

- `crypto-sim-example.cc`: Demonstrates basic encryption/decryption with UDP communication

## Testing

Run the test suite with:
```bash
./test.py --suite=crypto-sim
```

## Authors

- Samved Sajankila <samved58117@gmail.com>
- Mohit P. Tahiliani <tahiliani@nitk.edu.in>

## License

GPL-2.0-only