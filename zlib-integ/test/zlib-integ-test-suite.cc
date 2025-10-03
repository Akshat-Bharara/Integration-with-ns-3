#include "ns3/core-module.h"
#include "ns3/zlib-integ-module.h"
#include "ns3/test.h"

using namespace ns3;

/**
 * \brief Test suite for zlib-integ module
 */
class ZlibIntegTestSuite : public TestSuite
{
public:
    ZlibIntegTestSuite();
};

/**
 * \brief Test case for basic compression/decompression
 */
class ZlibBasicTest : public TestCase
{
public:
    ZlibBasicTest();
    virtual ~ZlibBasicTest();

private:
    virtual void DoRun(void);
};

ZlibIntegTestSuite::ZlibIntegTestSuite()
    : TestSuite("zlib-integ", TestSuite::Type::UNIT)
{
    AddTestCase(new ZlibBasicTest, TestCase::Duration::QUICK);
}

static ZlibIntegTestSuite zlibIntegTestSuite;

ZlibBasicTest::ZlibBasicTest()
    : TestCase("Basic compression and decompression test")
{
}

ZlibBasicTest::~ZlibBasicTest()
{
}

void
ZlibBasicTest::DoRun(void)
{
    std::string original = "Hello, World! This is a test string for compression.";
    
    // Create ZlibInteg instance
    Ptr<ZlibInteg> zlibInteg = CreateObject<ZlibInteg>();
    
    // Convert string to byte vector
    std::vector<uint8_t> originalData(original.begin(), original.end());
    
    // Test compression
    std::vector<uint8_t> compressed = zlibInteg->Deflate(originalData);
    NS_TEST_ASSERT_MSG_GT(compressed.size(), 0, "Compression should produce output");
    
    // Test decompression
    std::vector<uint8_t> decompressed = zlibInteg->Inflate(compressed, originalData.size());
    NS_TEST_ASSERT_MSG_EQ(decompressed.size(), originalData.size(), "Decompressed size should match original");
    
    // Convert back to string and verify data integrity
    std::string decompressedStr(decompressed.begin(), decompressed.end());
    NS_TEST_ASSERT_MSG_EQ(original, decompressedStr, "Decompressed data should match original");
    
    // Test empty data
    std::vector<uint8_t> empty;
    std::vector<uint8_t> compressedEmpty = zlibInteg->Deflate(empty);
    std::vector<uint8_t> decompressedEmpty = zlibInteg->Inflate(compressedEmpty, 0);
    NS_TEST_ASSERT_MSG_EQ(empty.size(), decompressedEmpty.size(), "Empty data should remain empty");
}