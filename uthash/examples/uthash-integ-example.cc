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
#include "ns3/uthash-integ-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("UthashIntegExample");

int
main(int argc, char* argv[])
{
    bool verbose = true;
    bool tracing = false;
    
    // Parse command line arguments
    CommandLine cmd;
    cmd.AddValue("verbose", "Tell application to log if true", verbose);
    cmd.AddValue("tracing", "Enable pcap tracing", tracing);
    cmd.Parse(argc, argv);
    
    if (verbose)
    {
        LogComponentEnable("UthashIntegExample", LOG_LEVEL_INFO);
    }

    // Create nodes
    NS_LOG_INFO("Create nodes.");
    NodeContainer nodes;
    nodes.Create(3);

    // Create point-to-point links between nodes
    NS_LOG_INFO("Create channels.");
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer devices01 = pointToPoint.Install(nodes.Get(0), nodes.Get(1));
    NetDeviceContainer devices12 = pointToPoint.Install(nodes.Get(1), nodes.Get(2));

    // Install Internet stack
    InternetStackHelper stack;
    stack.Install(nodes);

    // Assign IP addresses
    NS_LOG_INFO("Assign IP Addresses.");
    Ipv4AddressHelper address;
    
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces01 = address.Assign(devices01);
    
    address.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces12 = address.Assign(devices12);

    // Set up echo server on node 2
    NS_LOG_INFO("Create Applications.");
    UdpEchoServerHelper echoServer(9);
    ApplicationContainer serverApps = echoServer.Install(nodes.Get(2));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    // Set up echo client on node 0
    UdpEchoClientHelper echoClient(interfaces12.GetAddress(1), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(10));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    // Create UTHash routing tables using the helper
    HashTableHelper hashHelper;
    
    // Node 0's routing table
    NS_LOG_INFO("Creating routing table for Node 0");
    Ptr<HashTableWrapper> routeTable0 = hashHelper.CreateRoutingTable(nodes.Get(0));
    
    // Adding routes to node 0's table (destination, next hop, interface, metric)
    hashHelper.AddRoute(routeTable0, interfaces01.GetAddress(0), interfaces01.GetAddress(1), 0, 10); // Route to self
    hashHelper.AddRoute(routeTable0, interfaces01.GetAddress(1), interfaces01.GetAddress(1), 0, 10); // Route to node 1
    hashHelper.AddRoute(routeTable0, interfaces12.GetAddress(1), interfaces01.GetAddress(1), 0, 20); // Route to node 2
    
    // Node 1's routing table
    NS_LOG_INFO("Creating routing table for Node 1");
    Ptr<HashTableWrapper> routeTable1 = hashHelper.CreateRoutingTable(nodes.Get(1));
    
    // Adding routes to node 1's table
    hashHelper.AddRoute(routeTable1, interfaces01.GetAddress(1), interfaces01.GetAddress(1), 0, 10); // Route to self
    hashHelper.AddRoute(routeTable1, interfaces01.GetAddress(0), interfaces01.GetAddress(0), 0, 10); // Route to node 0
    hashHelper.AddRoute(routeTable1, interfaces12.GetAddress(1), interfaces12.GetAddress(1), 1, 10); // Route to node 2
    
    // Print routing tables
    NS_LOG_INFO("Node 0 Routing Table:");
    hashHelper.PrintRoutingTable(routeTable0);
    
    NS_LOG_INFO("Node 1 Routing Table:");
    hashHelper.PrintRoutingTable(routeTable1);
    
    // Simulate packet routing decision
    NS_LOG_INFO("Routing packet from Node 0 to Node 2...");
    std::string nextHop = hashHelper.FindNextHop(routeTable0, interfaces12.GetAddress(1));
    if (!nextHop.empty())
    {
        NS_LOG_INFO("Next hop for packet to " << interfaces12.GetAddress(1) << " from Node 0: " << nextHop);
    }
    else
    {
        NS_LOG_INFO("No route found from Node 0 to " << interfaces12.GetAddress(1));
    }
    
    // Create a connection tracking table for Node 1
    NS_LOG_INFO("Creating connection tracking table for Node 1");
    Ptr<HashTableWrapper> connTable = hashHelper.InitConnectionTracking(nodes.Get(1));
    
    // Add some example connections to track
    hashHelper.TrackConnection(connTable, 
                             interfaces01.GetAddress(0).Get(), 
                             interfaces12.GetAddress(1).Get(), 
                             12345, 9, "UDP");
    
    // Print connection tracking table
    NS_LOG_INFO("Node 1 Connection Tracking Table:");
    hashHelper.PrintConnectionStats(connTable);
    
    // Enable pcap tracing if requested
    if (tracing)
    {
        pointToPoint.EnablePcapAll("uthash-integ-example");
    }
    
    // Run the simulation
    NS_LOG_INFO("Run Simulation.");
    Simulator::Run();
    
    // Get hash table statistics
    HashTableStats stats = routeTable0->GetStats();
    NS_LOG_INFO("Hash Table Statistics:");
    NS_LOG_INFO("Routes: " << stats.routeCount);
    NS_LOG_INFO("Lookups: " << stats.lookupCount);
    NS_LOG_INFO("Avg Lookup Time: " << stats.avgLookupTime.GetMicroSeconds() << " μs");
    
    // Clean up
    routeTable0->Clear();
    routeTable1->Clear();
    connTable->Clear();
    
    Simulator::Destroy();
    NS_LOG_INFO("Simulation complete.");
    
    return 0;
}
