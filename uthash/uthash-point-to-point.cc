/*
 * Copyright (c) 2025-28 NITK Surathkal
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Authors: Swaraj Singh <swarajsingh.231cs158@nitk.edu.in>
 *          Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 */

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/uthash-integ.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("UthashIntegExample");

int
main(int argc, char* argv[])
{
    // Create nodes
    NodeContainer nodes;
    nodes.Create(2);

    // Set up point-to-point connection
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer devices = pointToPoint.Install(nodes);

    // Install internet stack
    InternetStackHelper internet;
    internet.Install(nodes);

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

    // Set up original data
    std::string originalData = "Hello UTHash world from NITK! This is a test string for routing.";
    std::cout << "Original data: " << originalData << std::endl;
    std::cout << "Original data size: " << originalData.size() << " bytes" << std::endl;

    // Create routing table and add route
    Ptr<HashTableWrapper> routingTable = CreateObject<HashTableWrapper>();
    routingTable->AddRouteEntry(interfaces.GetAddress(1).Get(), interfaces.GetAddress(1).Get(), 0, 10);
    
    // Display routing table
    std::cout << "\n=== Routing Table ===" << std::endl;
    std::cout << "Destination\tNext Hop" << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << interfaces.GetAddress(1) << "\t" << interfaces.GetAddress(1) << std::endl;
    std::cout << "------------------------" << std::endl;
    
    // Create UDP client
    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), port);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(originalData.size()));
    
    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
    
    // Set data as packet content
    uint8_t* buffer = (uint8_t*)malloc(originalData.size());
    memcpy(buffer, originalData.c_str(), originalData.size());
    echoClient.SetFill(clientApps.Get(0), buffer, originalData.size(), originalData.size());
    free(buffer);

    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    // Enable PCAP tracing
    pointToPoint.EnablePcapAll("uthash-integ", false);

    // Run simulation
    Simulator::Run();
    
    std::cout << "\nSimulation complete!" << std::endl;
    std::cout << "Check uthash-integ*.pcap files to see the packet transmissions." << std::endl;
    
    // Demonstrate route lookup
    std::cout << "\n=== Demonstrating Route Lookup ===" << std::endl;
    RouteEntry* route = routingTable->FindRoute(interfaces.GetAddress(1).Get());
    
    if (route)
    {
        std::cout << "SUCCESS: Route found!" << std::endl;
        std::cout << "Destination: " << interfaces.GetAddress(1) << std::endl;
        std::cout << "Next hop:    " << Ipv4Address(route->nextHop) << std::endl;
        delete route;
    }
    else
    {
        std::cout << "FAILURE: Route not found!" << std::endl;
    }
    
    // Cleanup
    routingTable->Clear();
    Simulator::Destroy();
    return 0;
}