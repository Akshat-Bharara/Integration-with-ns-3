// Include header files from the module to test
#include "ns3/uthash-integ.h"
#include "ns3/uthash-integ-helper.h"

// An essential include is test.h
#include "ns3/test.h"
#include "ns3/ipv4-address.h"

// Do not put your test classes in namespace ns3. You may find it useful
// to use the using directive to access the ns3 namespace directly
using namespace ns3;

// Add a doxygen group for tests.
/**
 * @defgroup uthash-integ-tests Tests for uthash-integ
 * @ingroup uthash-integ
 * @ingroup tests
 */

/**
 * @ingroup uthash-integ-tests
 * Test case for basic hash table operations
 */
class UthashBasicTestCase : public TestCase
{
public:
    UthashBasicTestCase();
    ~UthashBasicTestCase() override;

private:
    void DoRun() override;
};

UthashBasicTestCase::UthashBasicTestCase()
    : TestCase("Uthash basic operations test")
{
}

UthashBasicTestCase::~UthashBasicTestCase()
{
}

void
UthashBasicTestCase::DoRun()
{
    // Test object creation
    Ptr<HashTableWrapper> hashTable = CreateObject<HashTableWrapper>();
    NS_TEST_ASSERT_MSG_NE(hashTable, nullptr, "Failed to create HashTableWrapper object");
    
    // Test adding route entries
    Ipv4Address dest1("192.168.1.1");
    Ipv4Address nextHop1("10.0.0.1");
    NS_TEST_ASSERT_MSG_EQ(hashTable->AddRouteEntry(dest1.Get(), nextHop1.Get(), 0, 10), true, 
                          "Failed to add route entry");
    
    Ipv4Address dest2("192.168.1.2");
    Ipv4Address nextHop2("10.0.0.2");
    NS_TEST_ASSERT_MSG_EQ(hashTable->AddRouteEntry(dest2.Get(), nextHop2.Get(), 1, 20), true, 
                          "Failed to add second route entry");
    
    // Test route lookup
    RouteEntry* route = hashTable->FindRoute(dest1.Get());
    NS_TEST_ASSERT_MSG_NE(route, nullptr, "Failed to find route entry");
    
    if (route) {
        NS_TEST_ASSERT_MSG_EQ(route->nextHop, nextHop1.Get(), "Next hop mismatch");
        NS_TEST_ASSERT_MSG_EQ(route->interface, 0, "Interface mismatch");
        NS_TEST_ASSERT_MSG_EQ(route->metric, 10, "Metric mismatch");
        delete route;
    }
    
    // Test route count
    NS_TEST_ASSERT_MSG_EQ(hashTable->GetRouteCount(), 2, "Route count should be 2");
    
    // Test route deletion
    NS_TEST_ASSERT_MSG_EQ(hashTable->DeleteRoute(dest1.Get()), true, "Failed to delete route entry");
    NS_TEST_ASSERT_MSG_EQ(hashTable->GetRouteCount(), 1, "Route count should be 1 after deletion");
    
    // Test looking up a deleted route
    RouteEntry* deletedRoute = hashTable->FindRoute(dest1.Get());
    NS_TEST_ASSERT_MSG_EQ(deletedRoute, nullptr, "Deleted route should not be found");
    
    // Clear all routes
    hashTable->Clear();
    NS_TEST_ASSERT_MSG_EQ(hashTable->GetRouteCount(), 0, "Route count should be 0 after clearing");
}

/**
 * @ingroup uthash-integ-tests
 * Test case for connection tracking operations
 */
class UthashConnectionTrackingTestCase : public TestCase
{
public:
    UthashConnectionTrackingTestCase();
    ~UthashConnectionTrackingTestCase() override;

private:
    void DoRun() override;
};

UthashConnectionTrackingTestCase::UthashConnectionTrackingTestCase()
    : TestCase("Uthash connection tracking test")
{
}

UthashConnectionTrackingTestCase::~UthashConnectionTrackingTestCase()
{
}

void
UthashConnectionTrackingTestCase::DoRun()
{
    // Test object creation
    Ptr<HashTableWrapper> hashTable = CreateObject<HashTableWrapper>();
    NS_TEST_ASSERT_MSG_NE(hashTable, nullptr, "Failed to create HashTableWrapper object");
    
    // Initialize connection tracking
    hashTable->InitConnectionTracking();
    
    // Test adding connections
    NS_TEST_ASSERT_MSG_EQ(hashTable->AddConnection("192.168.1.1", "10.0.0.1", 12345, 80, "TCP"), true, 
                         "Failed to add connection");
    NS_TEST_ASSERT_MSG_EQ(hashTable->AddConnection("192.168.1.2", "10.0.0.2", 23456, 443, "TCP"), true, 
                         "Failed to add second connection");
    
    // Test connection count
    NS_TEST_ASSERT_MSG_EQ(hashTable->GetConnectionCount(), 2, "Connection count should be 2");
    
    // Test connection lookup
    ConnectionEntry* conn = hashTable->FindConnection("192.168.1.1", "10.0.0.1", 12345, 80);
    NS_TEST_ASSERT_MSG_NE(conn, nullptr, "Failed to find connection");
    
    if (conn) {
        NS_TEST_ASSERT_MSG_EQ(conn->sourceIP, "192.168.1.1", "Source IP mismatch");
        NS_TEST_ASSERT_MSG_EQ(conn->destIP, "10.0.0.1", "Destination IP mismatch");
        NS_TEST_ASSERT_MSG_EQ(conn->sourcePort, 12345, "Source port mismatch");
        NS_TEST_ASSERT_MSG_EQ(conn->destPort, 80, "Destination port mismatch");
        NS_TEST_ASSERT_MSG_EQ(conn->protocol, "TCP", "Protocol mismatch");
        NS_TEST_ASSERT_MSG_EQ(conn->packetCount, 1, "Packet count mismatch");
        delete conn;
    }
    
    // Test connection update
    NS_TEST_ASSERT_MSG_EQ(hashTable->UpdateConnection("192.168.1.1", "10.0.0.1", 12345, 80), true, 
                         "Failed to update connection");
    
    // Verify packet count incremented
    ConnectionEntry* updatedConn = hashTable->FindConnection("192.168.1.1", "10.0.0.1", 12345, 80);
    if (updatedConn) {
        NS_TEST_ASSERT_MSG_EQ(updatedConn->packetCount, 2, "Packet count should be 2 after update");
        delete updatedConn;
    }
    
    // Test connection deletion
    NS_TEST_ASSERT_MSG_EQ(hashTable->DeleteConnection("192.168.1.1", "10.0.0.1", 12345, 80), true, 
                         "Failed to delete connection");
    NS_TEST_ASSERT_MSG_EQ(hashTable->GetConnectionCount(), 1, "Connection count should be 1 after deletion");
    
    // Clear all connections
    hashTable->Clear();
    NS_TEST_ASSERT_MSG_EQ(hashTable->GetConnectionCount(), 0, "Connection count should be 0 after clearing");
}

/**
 * @ingroup uthash-integ-tests
 * TestSuite for module uthash-integ
 */
class UthashIntegTestSuite : public TestSuite
{
public:
    UthashIntegTestSuite();
};

UthashIntegTestSuite::UthashIntegTestSuite()
    : TestSuite("uthash-integ", Type::UNIT)
{
    AddTestCase(new UthashBasicTestCase, TestCase::Duration::QUICK);
    AddTestCase(new UthashConnectionTrackingTestCase, TestCase::Duration::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
/**
 * @ingroup uthash-integ-tests
 * Static variable for test initialization
 */
static UthashIntegTestSuite sUthashIntegTestSuite;
