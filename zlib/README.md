# Documentation for integrating zlib library in ns-3

This document provides an overview of integrating the **zlib** compression library into **ns-3**.  
zlib can be used to compress packet payloads (deflate), reducing transmission delay and latency.

---

## What is zlib?

zlib is a free, general-purpose, lossless data-compression C library, portable across platforms and not covered by patents.  
It uses the DEFLATE algorithm which avoids data expansion (unlike LZW, which can enlarge data in extreme cases).  
Its memory footprint is independent of input data and tunable for performance vs. compression ratio.

---

## Installation

### Linux/Unix
```bash
sudo apt install zlib1g-dev
````

### macOS

```bash
brew install zlib
```

---

## Module Structure

```
contrib/zlib-integ
├── CMakeLists.txt
├── README.md
├── doc
│   └── zlib-integ.rst
├── examples
│   ├── CMakeLists.txt
│   └── zlib-integ-example.cc
├── helper
│   ├── zlib-integ-helper.cc
│   └── zlib-integ-helper.h
├── model
    ├── zlib-integ.cc
    └── zlib-integ.h
```

---

## Key Components

* **ZlibInteg class** (`model/zlib-integ.h/.cc`)

  * Inherits from `ns3::Object`
  * `Deflate()` → compression
  * `Inflate()` → decompression
  * `GetVersion()` → returns zlib version

* **ZlibIntegHelper class** (`helper/zlib-integ-helper.h/.cc`)

  * Standard ns-3 helper wrapper

* **Example program** (`examples/zlib-integ-example.cc`)

  * Demonstrates compression and decompression in a client-server setup

---

## Example Program Flow

```cpp
NodeContainer nodes;
nodes.Create(2);

UdpEchoServerHelper echoServer(port);
ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
serverApps.Start(Seconds(1.0));
serverApps.Stop(Seconds(10.0));

Ptr<ZlibInteg> compressor = CreateObject<ZlibInteg>();
std::vector<uint8_t> inputVector(originalData.begin(), originalData.end());
std::vector<uint8_t> compressedData = compressor->Deflate(inputVector);

std::cout << "Compressed size: " << compressedData.size() << " bytes" << std::endl;

UdpEchoClientHelper echoClient(interfaces.GetAddress(1), port);
echoClient.SetAttribute("PacketSize", UintegerValue(compressedData.size()));

csma.EnablePcapAll("zlib-integ", false);
Simulator::Run();
```

---

## Running the Example

### Build

```bash
./ns3 build zlib-integ-example
```

### Run

```bash
./ns3 run zlib-integ-example
```

The simulation logs compression ratio, saves PCAP traces (`zlib-integ*.pcap`), and validates decompression correctness.

---

## Reference Code

See [`examples/zlib-integ-example.cc`](examples/zlib-point-to-point.cc) for the complete working code:

* Compresses data
* Sends it over a UDP client-server application
* Writes compressed packets to PCAP
* Demonstrates decompression and checks integrity

```cpp
if (originalData == decompressedString)
{
    std::cout << "SUCCESS: Decompressed data matches original!" << std::endl;
}
else
{
    std::cout << "FAILURE: Data mismatch!" << std::endl;
}
```
