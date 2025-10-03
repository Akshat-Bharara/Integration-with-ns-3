#include "ns3/core-module.h"
#include "ns3/crypto-sim-module.h"
#include "ns3/test.h"

using namespace ns3;

/**
 * \brief Test suite for crypto-sim module
 */
class CryptoSimTestSuite : public TestSuite
{
public:
    CryptoSimTestSuite();
};

/**
 * \brief Test case for basic encryption/decryption
 */
class CryptoBasicTest : public TestCase
{
public:
    CryptoBasicTest();
    virtual ~CryptoBasicTest();

private:
    virtual void DoRun(void);
};

CryptoSimTestSuite::CryptoSimTestSuite()
    : TestSuite("crypto-sim", TestSuite::Type::UNIT)
{
    AddTestCase(new CryptoBasicTest, TestCase::Duration::QUICK);
}

static CryptoSimTestSuite cryptoSimTestSuite;

CryptoBasicTest::CryptoBasicTest()
    : TestCase("Basic encryption and decryption test")
{
}

CryptoBasicTest::~CryptoBasicTest()
{
}

void
CryptoBasicTest::DoRun(void)
{
    std::string original = "Hello, World! This is a test string for encryption.";
    
    // Create CryptoSim instance
    Ptr<CryptoSim> cryptoSim = CreateObject<CryptoSim>();
    
    // Convert string to byte vector
    std::vector<uint8_t> originalData(original.begin(), original.end());
    
    // Test encryption
    std::vector<uint8_t> encrypted = cryptoSim->Encrypt(originalData);
    NS_TEST_ASSERT_MSG_GT(encrypted.size(), 0, "Encryption should produce output");
    NS_TEST_ASSERT_MSG_GT(encrypted.size(), originalData.size(), "Encrypted data should be larger than original (includes key+IV)");
    
    // Test decryption
    std::vector<uint8_t> decrypted = cryptoSim->Decrypt(encrypted);
    NS_TEST_ASSERT_MSG_EQ(decrypted.size(), originalData.size(), "Decrypted size should match original");
    
    // Convert back to string and verify data integrity
    std::string decryptedStr(decrypted.begin(), decrypted.end());
    NS_TEST_ASSERT_MSG_EQ(original, decryptedStr, "Decrypted data should match original");
    
    // Test empty data
    std::vector<uint8_t> empty;
    std::vector<uint8_t> encryptedEmpty = cryptoSim->Encrypt(empty);
    NS_TEST_ASSERT_MSG_EQ(encryptedEmpty.size(), 0, "Empty data encryption should return empty vector");
}