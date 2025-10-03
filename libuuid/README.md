# LibUUID Integration Guide for ns-3

- This guide provides step-by-step instructions for integrating the libuuid into ns-3.
- Libuuid is a C library that generates universally unique identifiers (UUIDs).
- The primary goal is to learn how to integrate a C library with ns-3.

## Quick Start with Patch File (Recommended)

If you want to quickly integrate libuuid without manual setup, use the provided patch file:

```bash
# 1. Install dependencies
sudo apt update
sudo apt install uuid-dev

# 2. Navigate to your ns-3 directory
cd /path/to/your/ns-3-dev

# 3. Apply the patch file
patch -p1 < libuuid-integration.patch

# 4. Build and run
./ns3 clean
./ns3 configure --enable-examples
./ns3 build
./ns3 run libuuid-integ-example
```

**Expected Output:**
```
=== Node UUID Assignment ===
Node 0 UUID: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
Node 1 UUID: yyyyyyyy-yyyy-yyyy-yyyy-yyyyyyyyyyyy
...
Simulation complete!
```

---

## Manual Integration Guide

Following the guide here - <https://www.nsnam.org/docs/manual/html/new-modules.html>

### Prerequisites

#### Install Required Dependencies

```bash
# Install libuuid development library
sudo apt update
sudo apt install uuid-dev
```

## Create the Module Directory Structure

Create a new module directory in your ns-3 root:

```bash
cd /path/to/your/ns-3-dev
python3 utils/create-module.py libuuid-integ
```

Directory structure:

```
ns-3-dev/
├── libuuid-integ/
│   ├── model/
│   ├── helper/
│   ├── examples/
│   ├── test/
│   └── doc/
├── src/
├── examples/
└── ... (other ns-3 directories)
```

### Refer to the specific files attached in the libuuid-integ folder 

### Modify Main CMakeLists.txt

Add your module to the main ns-3 CMakeLists.txt file:

```bash
# Edit /path/to/ns-3-dev/CMakeLists.txt
# Find the section with add_subdirectory commands and add:
```

```cmake
# Build NS3 library core
add_subdirectory(src)

# Build libuuid-integ module
add_subdirectory(libuuid-integ)

# Build NS library examples
add_subdirectory(examples)
```

## Build and Test

### Configure ns-3

```bash
cd /path/to/ns-3-dev
./ns3 clean
./ns3 configure --enable-examples
```

### Build the Project

```bash
./ns3 build
```

### Run the Example

```bash
./ns3 run libuuid-integ-example
```

## Key Components

* **UuidHelper class** (`helper/libuuid-integ-helper.h/.cc`)

  * Standard ns-3 helper class for UUID management
  * `AssignUuids()` → assigns UUIDs to node containers
  * `GetNodeUuid()` → retrieves UUID for specific node
  * `PrintNodeUuids()` → displays all node UUIDs
  * `GenerateUuid()` → creates UUIDs

---

## Example Program Flow

```cpp
// Create network nodes
NodeContainer nodes;
nodes.Create(2);

// Create UUID helper and assign unique identifiers
UuidHelper uuidHelper;
uuidHelper.AssignUuids(nodes);

// Display assigned UUIDs
std::cout << "=== Node UUID Assignment ===" << std::endl;
uuidHelper.PrintNodeUuids(nodes);

...

// Create UUID-based payload for network transmission
std::string nodeUuid0 = uuidHelper.GetNodeUuid(nodes.Get(0));
std::string nodeUuid1 = uuidHelper.GetNodeUuid(nodes.Get(1));

std::string uuidPayload = "Message from Node UUID: " + nodeUuid0 + 
                         " to Node UUID: " + nodeUuid1 + 
                         " - LibUUID Integration Test!";


...

// Enable packet capture for analysis
csma.EnablePcapAll("libuuid-integ", false);

// Run simulation
Simulator::Run();
```

Expected output:

```
=== Node UUID Assignment ===
Node 0 UUID: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
Node 1 UUID: yyyyyyyy-yyyy-yyyy-yyyy-yyyyyyyyyyyy

Node 0 UUID: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
Node 1 UUID: yyyyyyyy-yyyy-yyyy-yyyy-yyyyyyyyyyyy
Payload size: XXX bytes

Simulation complete!
Check libuuid-integ*.pcap files to see the UUID-based packets in Wireshark.

=== LibUUID Integration Summary ===
Successfully assigned UUIDs to 2 nodes
```

## Analyze PCAP Files (Optional)

```bash
# View packet content (requires Wireshark or tcpdump)
tcpdump -r libuuid-integ-0-1.pcap -X
```
