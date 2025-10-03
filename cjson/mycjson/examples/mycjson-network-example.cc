/*
 * Copyright (c) 2025-28 NITK Surathkal
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Authors: Sanjay S Bhat <sanjay95bhat@gmail.com>
 *          Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 */
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/mycjson-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("CJsonIntegExample");

int main(int argc, char* argv[])
{
    // Create nodes
    NodeContainer nodes;
    nodes.Create(2);

    // Install internet stack
    InternetStackHelper internet;
    internet.Install(nodes);

    // Create CSMA channel
    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", DataRateValue(DataRate(5000000)));
    csma.SetChannelAttribute("Delay", TimeValue(MilliSeconds(2)));
    NetDeviceContainer devices = csma.Install(nodes);

    // Assign IP addresses
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = ipv4.Assign(devices);

    uint16_t port = 9;

    // Create UDP server on node 1
    UdpEchoServerHelper echoServer(port);
    ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    // Set up original data to create JSON and send
    std::string originalMessage = "Hello cJSON world from NITK! This is a test message for JSON serialization.";
    std::string nodeInfo = "Node-0-Client";
    double timestamp = 2.0; // When client starts
    uint32_t sequenceNumber = 1;

    std::cout << "Original message: " << originalMessage << std::endl;

    // Create JSON payload using cJSON integration
    CJsonHelper helper;
    Ptr<CJsonWrapper> jsonPayload = helper.Create();

    // Add network simulation data to JSON
    jsonPayload->AddString("message", originalMessage);
    jsonPayload->AddString("node_info", nodeInfo);
    jsonPayload->AddNumber("timestamp", timestamp);
    jsonPayload->AddNumber("sequence", static_cast<double>(sequenceNumber));
    jsonPayload->AddString("source_ip", "10.1.1.1");
    jsonPayload->AddString("dest_ip", "10.1.1.2");
    jsonPayload->AddString("protocol", "UDP");

    // Convert JSON to string for transmission
    std::string jsonString = jsonPayload->ToString(false); // minified

    std::cout << "JSON payload: " << jsonString << std::endl;
    std::cout << "Original message size: " << originalMessage.size() << " bytes" << std::endl;
    std::cout << "JSON payload size: " << jsonString.size() << " bytes" << std::endl;
    std::cout << "Overhead: " << (jsonString.size() - originalMessage.size()) << " bytes" << std::endl;

    // Save JSON to file (pretty formatted version)
    std::string prettyJsonString = jsonPayload->ToString(true); // formatted
    std::string filename = "network-packet-data.json";
    if (CJsonHelper::SaveToFile(jsonPayload, filename, true))
    {
        std::cout << "JSON data saved to file: " << filename << std::endl;
    }
    else
    {
        std::cout << "Failed to save JSON to file!" << std::endl;
    }

    // Create UDP client with JSON data as payload
    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), port);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(jsonString.size()));

    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));

    // Set JSON data as the packet content
    echoClient.SetFill(clientApps.Get(0), reinterpret_cast<uint8_t*>(const_cast<char*>(jsonString.c_str())), jsonString.size(), jsonString.size());

    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    // Enable PCAP tracing to see JSON packet contents
    //csma.EnablePcapAll("cjson-integ", false);


    // Run simulation
    Simulator::Run();

    std::cout << "\nSimulation complete!" << std::endl;
    std::cout << "Check cjson-integ*.pcap files to see the JSON packet contents in Wireshark." << std::endl;

    // Now demonstrate JSON parsing of the same data
    std::cout << "\n=== Demonstrating JSON Parsing ===" << std::endl;
    Ptr<CJsonWrapper> parsedJson = helper.CreateFromString(jsonString);

    if (parsedJson && parsedJson->IsValid())
    {
        std::string extractedMessage;
        std::string extractedNodeInfo;
        double extractedTimestamp;
        double extractedSequence;
        std::string extractedSourceIp;

        if (parsedJson->GetString("message", extractedMessage) &&
            parsedJson->GetString("node_info", extractedNodeInfo) &&
            parsedJson->GetNumber("timestamp", extractedTimestamp) &&
            parsedJson->GetNumber("sequence", extractedSequence) &&
            parsedJson->GetString("source_ip", extractedSourceIp))
        {
            std::cout << "SUCCESS: JSON data parsed successfully!" << std::endl;
            std::cout << "Extracted Message:    " << extractedMessage << std::endl;
            std::cout << "Extracted Node Info:  " << extractedNodeInfo << std::endl;
            std::cout << "Extracted Timestamp:  " << extractedTimestamp << std::endl;
            std::cout << "Extracted Sequence:   " << static_cast<uint32_t>(extractedSequence) << std::endl;
            std::cout << "Extracted Source IP:  " << extractedSourceIp << std::endl;
            std::cout << "Data Integrity:       " << (originalMessage == extractedMessage ? "VERIFIED" : "FAILED") << std::endl;
        }
        else
        {
            std::cout << "FAILURE: Could not extract all JSON fields!" << std::endl;
        }
    }
    else
    {
        std::cout << "FAILURE: JSON parsing failed!" << std::endl;
    }

    Simulator::Destroy();
    return 0;
}
