# cJSON Integration with ns-3

This document provides a comprehensive guide for integrating the cJSON library into ns-3 network simulations. The cJSON library enables creation, parsing, and manipulation of JSON (JavaScript Object Notation) data structures for structured communication, configuration management, and data exchange in network simulations.

## What is cJSON Library?

cJSON is a lightweight, portable JSON parser and generator library written in C. It provides a simple API for creating and parsing JSON data structures. JSON is a human-readable, text-based data interchange format that is widely used for configuration files, web APIs, and structured data exchange. 

Key features:
- Fast and memory-efficient
- Easy integration into C/C++ projects
- Supports all JSON data types (objects, arrays, strings, numbers, booleans, null)
- Ideal for creating self-describing network protocols and configuration systems

## Installation

### Linux/Unix Systems
```bash
sudo apt install libcjson-dev
```

### macOS Systems
```bash
brew install cjson
```

## Module Structure

```
contrib/mycjson/
├── CMakeLists.txt
├── doc/
│   └── mycjson.rst
├── examples/
│   ├── CMakeLists.txt
│   └── mycjson-example.cc
├── helper/
│   ├── mycjson-helper.cc
│   └── mycjson-helper.h
├── model/
│   ├── mycjson.cc
│   └── mycjson.h
└── test/
    └── mycjson-test-suite.cc
```

## Key Components

### CJsonWrapper Class (`model/mycjson.h/.cc`)

The main wrapper class that inherits from `ns3::Object` and provides comprehensive JSON functionality:

#### Object and Array Creation
- `CreateObject()` - Creates JSON objects
- `CreateArray()` - Creates JSON arrays

#### Parsing
- `ParseString()` - Parses JSON strings

#### Data Addition
- `AddString()` - Adds string values
- `AddNumber()` - Adds numeric values
- `AddBool()` - Adds boolean values
- `AddNull()` - Adds null values
- `AddStringArray()` - Adds string arrays
- `AddNumberArray()` - Adds numeric arrays

#### Data Retrieval
- `GetString()` - Retrieves string values
- `GetNumber()` - Retrieves numeric values
- `GetBool()` - Retrieves boolean values
- `GetStringArray()` - Retrieves string arrays
- `GetNumberArray()` - Retrieves numeric arrays

#### JSON Operations
- `ToString()` - Serializes JSON to string
- `HasKey()` - Checks if key exists
- `GetKeys()` - Gets all keys
- `RemoveKey()` - Removes a key

#### Utility Functions
- `IsValid()` - Validates JSON structure
- `Clear()` - Clears JSON data
- `GetSize()` - Gets JSON size

### JsonUtils Class (`model/mycjson.h/.cc`)

Static utility functions for advanced JSON operations:

- `ValidateJsonString()` - JSON validation
- `PrettyPrint()` - Formatted JSON output
- `Minify()` - Compressed JSON output
- `MapToJson()` - Convert std::map to JSON
- `JsonToMap()` - Convert JSON to std::map

### CJsonHelper Class (`helper/mycjson-helper.h/.cc`)

Standard ns-3 helper class providing high-level functionality:

- `Create()` - Creates new JSON wrapper
- `CreateFromString()` - Creates from JSON string
- `CreateSimulationConfig()` - Simulation configuration templates
- `CreateTopologyInfo()` - Network topology representation
- `CreateLogEntry()` - Structured logging
- `SaveToFile()` - Save JSON to file
- `LoadFromFile()` - Load JSON from file

## Example Program

The `examples/mycjson-network-example.cc` demonstrates JSON integration in network simulations with packet transmission.

### Network Example Flow

```cpp
// Create network nodes
NodeContainer nodes;
nodes.Create(2);

// Setup UDP echo server
UdpEchoServerHelper echoServer(port);
ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
serverApps.Start(Seconds(1.0));
serverApps.Stop(Seconds(10.0));

// Create JSON payload
CJsonHelper helper;
Ptr<CJsonWrapper> jsonPayload = helper.Create();

// Add data to JSON
jsonPayload->AddString("message", originalMessage);
jsonPayload->AddString("node_info", nodeInfo);
jsonPayload->AddNumber("timestamp", timestamp);
jsonPayload->AddNumber("sequence", static_cast<double>(sequenceNumber));
jsonPayload->AddString("source_ip", "10.1.1.1");
jsonPayload->AddString("dest_ip", "10.1.1.2");
jsonPayload->AddString("protocol", "UDP");

// Serialize and analyze
std::string jsonString = jsonPayload->ToString(false); // minified
std::cout << "JSON payload size: " << jsonString.size() << " bytes" << std::endl;
std::cout << "Overhead: " << (jsonString.size() - originalMessage.size()) << " bytes" << std::endl;

// Save to file
CJsonHelper::SaveToFile(jsonPayload, "network-packet-data.json", true);

// Configure UDP client with JSON data
UdpEchoClientHelper echoClient(interfaces.GetAddress(1), port);
echoClient.SetAttribute("MaxPackets", UintegerValue(1));
echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
echoClient.SetAttribute("PacketSize", UintegerValue(jsonString.size()));
echoClient.SetFill(clientApps.Get(0), 
    reinterpret_cast<uint8_t*>(const_cast<char*>(jsonString.c_str())), 
    jsonString.size(), jsonString.size());

// Run simulation
Simulator::Run();

// Verify data integrity
Ptr<CJsonWrapper> parsedJson = helper.CreateFromString(jsonString);
if (parsedJson && parsedJson->IsValid()) {
    std::string extractedMessage;
    parsedJson->GetString("message", extractedMessage);
    std::cout << "Data Integrity: " << (originalMessage == extractedMessage ? "VERIFIED" : "FAILED") << std::endl;
}
```

## Building and Running

### Build the Module
```bash
./ns3 build
```

### Run the Network Example
```bash
./ns3 run mycjson-network-example
```

## Use Cases

- **Structured Communication**: Create self-describing network protocols
- **Configuration Management**: Store and exchange simulation parameters
- **Data Logging**: Structure simulation results and network events
- **Protocol Testing**: Validate data integrity in network transmissions
- **Topology Description**: Represent complex network topologies

## Benefits

- **Human Readable**: JSON format is easy to read and debug
- **Lightweight**: Minimal overhead for network simulations
- **Interoperable**: Compatible with web services and APIs
- **Flexible**: Supports complex nested data structures
- **Validated**: Built-in validation ensures data integrity

## Contributing

When contributing to this module, please ensure:
- All new functions include appropriate documentation
- Test cases are provided for new functionality
- Examples demonstrate practical usage
- Code follows ns-3 coding standards

## License

This module follows the same license as ns-3.
