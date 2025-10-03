# UTHash Integration Guide for ns-3

- This guide provides step-by-step instructions for integrating the UTHash into ns-3.
- UTHash is a C library that implements hash tables for C structures using macros.
- The primary goal is to learn how to integrate a C library with ns-3.

## Quick Start with Patch File (Recommended)

If you want to quickly integrate UTHash without manual setup, use the provided patch file:

```bash
# 1. Install dependencies
sudo apt update
sudo apt install uthash-dev

# 2. Navigate to your ns-3 directory
cd /path/to/your/ns-3-dev

# 3. Apply the patch file
patch -p1 < uthash-integration.patch

# 4. Build and run
./ns3 clean
./ns3 configure --enable-examples
./ns3 build
./ns3 run uthash-integ-example
```

**Expected Output:**
```
=== Routing Table Creation ===
Node 0 Routes: 10.1.1.2 via 10.1.1.1 (interface: 0, metric: 10)
Node 1 Routes: 10.1.1.1 via 10.1.1.1 (interface: 0, metric: 10)
...
Simulation complete!
```

---

## Manual Integration Guide

Following the guide here - <https://www.nsnam.org/docs/manual/html/new-modules.html>

### Prerequisites

#### Install Required Dependencies

```bash
# Install uthash development library
sudo apt update
sudo apt install uthash-dev
```

## Create the Module Directory Structure

Create a new module directory in your ns-3 root:

```bash
cd /path/to/your/ns-3-dev
python3 utils/create-module.py uthash-integ
```

Directory structure:

```
ns-3-dev/
├── uthash-integ/
│   ├── model/
│   ├── helper/
│   ├── examples/
│   ├── test/
│   └── doc/
├── src/
├── examples/
└── ... (other ns-3 directories)
```

### Refer to the specific files attached in the uthash-integ folder 

### Modify Main CMakeLists.txt

Add your module to the main ns-3 CMakeLists.txt file:

```bash
# Edit /path/to/ns-3-dev/CMakeLists.txt
# Find the section with add_subdirectory commands and add:
```

```cmake
# Build NS3 library core
add_subdirectory(src)

# Build uthash-integ module
add_subdirectory(uthash-integ)

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
./ns3 run uthash-integ-example
```

## Key Components

* **HashTableWrapper class** (`model/uthash-integ.h/.cc`)

  * Standard ns-3 class for hash table management
  * `AddRouteEntry()` → adds routes to the routing table
  * `FindRoute()` → finds route by destination
  * `DeleteRoute()` → removes routes from table
  * `GetStats()` → returns hash table statistics

* **HashTableHelper class** (`helper/uthash-integ-helper.h/.cc`)

  * Standard ns-3 helper class for hash table management
  * `CreateRoutingTable()` → creates routing table for nodes
  * `AddRoute()` → adds routes to a node's table
  * `FindNextHop()` → finds next hop for a destination
  * `PrintRoutingTable()` → displays routing table contents

---

## Example Program Flow

```cpp
// Create network nodes
NodeContainer nodes;
nodes.Create(2);

// Create hash table helper and routing tables
HashTableHelper hashHelper;
Ptr<HashTableWrapper> routeTable = hashHelper.CreateRoutingTable(nodes.Get(0));

// Add routes
hashHelper.AddRoute(routeTable, "10.1.1.2", "10.1.1.1", 0, 10);

// Display routing tables
std::cout << "=== Routing Table Creation ===" << std::endl;
hashHelper.PrintRoutingTable(routeTable);

...

// Create routing payload for network transmission
std::string routingPayload = "Route from " + interfaces.GetAddress(0).Get() +
                           " to " + interfaces.GetAddress(1).Get() +
                           " via " + interfaces.GetAddress(1).Get();

...

// Enable packet capture for analysis
pointToPoint.EnablePcapAll("uthash-integ", false);

// Run simulation
Simulator::Run();
```

Expected output:

```
=== Routing Table Creation ===
Node 0 Routes:
  Destination: 10.1.1.2
  Next hop: 10.1.1.1
  Interface: 0
  Metric: 10

Routing payload size: XXX bytes

Simulation complete!
Check uthash-integ*.pcap files to see the routing packets in Wireshark.

=== UTHash Integration Summary ===
Routes: 1
Lookups: 1
Average lookup time: XX μs
```

## Analyze PCAP Files (Optional)

```bash
# View packet content (requires Wireshark or tcpdump)
tcpdump -r uthash-integ-0-1.pcap -X
```