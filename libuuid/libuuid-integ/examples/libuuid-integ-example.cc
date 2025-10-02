/*
 * Copyright (c) 2025-28 NITK Surathkal
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Authors: Akshat Bharara <akshat.bharara05@gmail.com>
 *          Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 */

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/libuuid-integ-helper.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE("LibuuidIntegExample");

int main(int argc, char* argv[])
{
    // Create nodes
    NodeContainer nodes;
    nodes.Create(2);

    // Create UUID helper and assign UUIDs to nodes
    UuidHelper uuidHelper;
    uuidHelper.AssignUuids(nodes);


    // Display node UUIDs
    std::cout << "=== Node UUID Assignment ===" << std::endl;
    uuidHelper.PrintNodeUuids(nodes);

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

    // Create payload containing node UUIDs for identification
    std::string nodeUuid0 = uuidHelper.GetNodeUuid(nodes.Get(0));
    std::string nodeUuid1 = uuidHelper.GetNodeUuid(nodes.Get(1));
    
    std::string uuidPayload = "Message from Node UUID: " + nodeUuid0 + 
                             " to Node UUID: " + nodeUuid1 + 
                             " - LibUUID Integration Test from NITK!";

    std::cout << "\nNode 0 UUID: " << nodeUuid0 << std::endl;
    std::cout << "Node 1 UUID: " << nodeUuid1 << std::endl;
    std::cout << "Payload size: " << uuidPayload.size() << " bytes" << std::endl;

    // Create UDP client with UUID-based payload
    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), port);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(uuidPayload.size()));
    
    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
    
    // Set UUID payload as the packet content
    echoClient.SetFill(clientApps.Get(0), (uint8_t*)uuidPayload.c_str(), uuidPayload.size(), uuidPayload.size());

    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    // Enable PCAP tracing to see UUID-based packets
    csma.EnablePcapAll("libuuid-integ", false);
    
    // Run simulation
    Simulator::Run();
    
    std::cout << "\nSimulation complete!" << std::endl;
    std::cout << "Check libuuid-integ*.pcap files to see the UUID-based packets in Wireshark." << std::endl;
    
    std::cout << "\n=== LibUUID Integration Summary ===" << std::endl;
    std::cout << "Successfully assigned UUIDs to " << nodes.GetN() << " nodes" << std::endl;
    
    Simulator::Destroy();
    return 0;
}
