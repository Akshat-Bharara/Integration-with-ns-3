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
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/crypto-sim.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("CryptoSimExample");

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

    // Set up original data to encrypt and send
    std::string originalData = "Hello crypto world from NITK! This is a test string for encryption. "
                              "This message demonstrates AES encryption in NS-3 simulation. "
                              "Secure communication is essential for network protocols.";

    std::cout << "Original data: " << originalData << std::endl;
    std::cout << "Original data size: " << originalData.size() << " bytes" << std::endl;

    // Encrypt the data using CryptoSim
    Ptr<CryptoSim> encryptor = CreateObject<CryptoSim>();
    std::vector<uint8_t> inputVector(originalData.begin(), originalData.end());
    std::vector<uint8_t> encryptedData = encryptor->Encrypt(inputVector);
    
    std::cout << "Encrypted data size: " << encryptedData.size() << " bytes" << std::endl;

    // Create UDP client with encrypted data as payload
    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), port);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(encryptedData.size()));
    
    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
    
    // Set encrypted data as the packet content
    echoClient.SetFill(clientApps.Get(0), encryptedData.data(), encryptedData.size(), encryptedData.size());

    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    // Enable PCAP tracing to see encrypted packet content
    pointToPoint.EnablePcapAll("crypto-sim", false);

    // Run simulation
    Simulator::Run();
    
    std::cout << "\nSimulation complete!" << std::endl;
    std::cout << "Check crypto-sim*.pcap files to see the encrypted packet content in Wireshark." << std::endl;
    
    // Now demonstrate decryption of the same data
    std::cout << "\n=== Demonstrating Decryption ===" << std::endl;
    std::vector<uint8_t> decryptedData = encryptor->Decrypt(encryptedData);
    std::string decryptedString(decryptedData.begin(), decryptedData.end());
    
    if (originalData == decryptedString)
    {
        std::cout << "SUCCESS: Decrypted data matches original!" << std::endl;
        std::cout << "Original:     \"" << originalData << "\"" << std::endl;
        std::cout << "Decrypted:    \"" << decryptedString << "\"" << std::endl;
        std::cout << "Data secured with AES encryption!" << std::endl;
    }
    else
    {
        std::cout << "FAILURE: Data mismatch after decryption!" << std::endl;
    }
    
    Simulator::Destroy();
    return 0;
}