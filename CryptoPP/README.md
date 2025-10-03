````markdown
# Documentation for integrating Crypto++ library in ns-3

This document gives an overview of **Crypto++ (CryptoPP)** integration into **ns-3**.  
Crypto++ can be used to perform encryption (AES, RSA, etc.) on packet payloads to enable secure communication between nodes in a simulation.

---

## What is Crypto++?

Crypto++ is a free C++ class library of cryptographic schemes.  
It provides implementations for encryption, hashing, key exchange, and random number generation.  
Crypto++ is portable across platforms and has a modular design, making it usable in research and simulation projects.  

---

## Installation

### Linux/Unix
```bash
sudo apt install libcrypto++-dev libcrypto++-doc libcrypto++-utils
````

### macOS

```bash
brew install cryptopp
```

---

## Module Structure

```
contrib/crypto-sim
├── CMakeLists.txt
├── README.md
├── doc
│   └── crypto-sim.rst
├── examples
│   ├── CMakeLists.txt
│   └── crypto-sim-example.cc
├── helper
│   ├── crypto-sim-helper.cc
│   └── crypto-sim-helper.h
├── model
    ├── crypto-sim.cc
    └── crypto-sim.h
```

---

## Key Components

* **CryptoSim class** (`model/crypto-sim.h/.cc`)

  * Inherits from `ns3::Object`
  * `Encrypt()` → performs AES encryption
  * `Decrypt()` → performs AES decryption
  * Returns library version using `GetVersion()` (planned, not implemented yet)

* **CryptoSimHelper class** (`helper/crypto-sim-helper.h/.cc`)

  * Standard ns-3 helper

* **Example program** (`examples/crypto-sim-example.cc`)

  * Shows AES encryption + decryption with UDP echo applications

---

## Example Program Flow

```cpp
NodeContainer nodes;
nodes.Create(2);

PointToPointHelper pointToPoint;
pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

UdpEchoServerHelper echoServer(port);
ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));

Ptr<CryptoSim> encryptor = CreateObject<CryptoSim>();
std::vector<uint8_t> encryptedData = encryptor->Encrypt(inputVector);

UdpEchoClientHelper echoClient(interfaces.GetAddress(1), port);
echoClient.SetAttribute("PacketSize", UintegerValue(encryptedData.size()));

pointToPoint.EnablePcapAll("crypto-sim", false);
Simulator::Run();
```

---

## Running the Example

### Build

```bash
./ns3 build crypto-sim-example
```

### Run

```bash
./ns3 run crypto-sim-example
```

The program encrypts a string using AES, transmits it over a UDP client-server setup, saves encrypted packets in PCAP format, and then demonstrates decryption.

---

## Reference Code

The complete example (`examples/crypto-sim-example.cc`) demonstrates:

* Encrypting a payload
* Sending encrypted packets between nodes
* Viewing encrypted packets in Wireshark (`crypto-sim*.pcap`)
* Decrypting packets to validate correctness

```cpp
if (originalData == decryptedString)
{
    std::cout << "SUCCESS: Decrypted data matches original!" << std::endl;
}
else
{
    std::cout << "FAILURE: Data mismatch!" << std::endl;
}
```

---

```
```
