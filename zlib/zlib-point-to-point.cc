/*
 * Copyright (c) 2025-28 NITK Surathkal
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Authors: Samved Sajankila <samved58117@gmail.com>
 *          Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 */

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/zlib-integ.h"

using namespace ns3;    

NS_LOG_COMPONENT_DEFINE("ZlibIntegExample");

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

    // Set up original data to compress and send
    std::string originalData = "Hello zlib world from NITK! This is a test string for compression. "
                              "Repeating this sentence makes it more compressible. "
                              "Hello zlib world from NITK! This is a test string for compression. "
                              "Repeating this sentence makes it more compressible. "
                              "Hello zlib world from NITK! This is a test string for compression.";

    std::cout << "Original data size: " << originalData.size() << " bytes" << std::endl;

    // Compress the data using zlib
    Ptr<ZlibInteg> compressor = CreateObject<ZlibInteg>();
    std::vector<uint8_t> inputVector(originalData.begin(), originalData.end());
    std::vector<uint8_t> compressedData = compressor->Deflate(inputVector);
    
    std::cout << "Compressed data size: " << compressedData.size() << " bytes" << std::endl;
    std::cout << "Compression ratio: " << (100.0 - (100.0 * compressedData.size() / originalData.size())) << "%" << std::endl;

    // Create UDP client with compressed data as payload
    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), port);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(compressedData.size()));
    
    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
    
    // Set compressed data as the packet content
    echoClient.SetFill(clientApps.Get(0), compressedData.data(), compressedData.size(), compressedData.size());

    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    // Enable PCAP tracing to see compressed packet sizes
    csma.EnablePcapAll("zlib-integ", false);

    
    // Run simulation
    Simulator::Run();
    
    std::cout << "\nSimulation complete!" << std::endl;
    std::cout << "Check zlib-integ*.pcap files to see the compressed packet sizes in Wireshark." << std::endl;
    
    // Now demonstrate decompression of the same data
    std::cout << "\n=== Demonstrating Decompression ===" << std::endl;
    std::vector<uint8_t> decompressedData = compressor->Inflate(compressedData);
    std::string decompressedString(decompressedData.begin(), decompressedData.end());
    
    if (originalData == decompressedString)
    {
        std::cout << "SUCCESS: Decompressed data matches original!" << std::endl;
        std::cout << "Original Size:     " << originalData.size() << " bytes" << std::endl;
        std::cout << "Compressed Size:   " << compressedData.size() << " bytes" << std::endl;
        std::cout << "Decompressed Size: " << decompressedData.size() << " bytes" << std::endl;
        std::cout << "Bandwidth Saved:   " << (originalData.size() - compressedData.size()) << " bytes" << std::endl;
    }
    else
    {
        std::cout << "FAILURE: Data mismatch after decompression!" << std::endl;
    }
    
    Simulator::Destroy();
    return 0;
}