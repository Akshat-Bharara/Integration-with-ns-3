#include "crypto-sim.h"
#include "ns3/log.h"
#include <iostream>

// Crypto++ headers - using local system installation
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>

namespace ns3 {

// Define a logging component for this module
NS_LOG_COMPONENT_DEFINE("CryptoSim");

TypeId CryptoSim::GetTypeId(void)
{
  static TypeId tid = TypeId("ns3::CryptoSim")
    .SetParent<Object>()
    .SetGroupName("CryptoSim");
  return tid;
}

CryptoSim::CryptoSim() {
    NS_LOG_FUNCTION(this);
}

CryptoSim::~CryptoSim() {
    NS_LOG_FUNCTION(this);
}

std::string CryptoSim::GetVersion()
{
    return "CryptoSim v1.0 with Crypto++ Library";
}

std::vector<uint8_t> CryptoSim::Encrypt(const std::vector<uint8_t>& inputData)
{
    NS_LOG_FUNCTION(this);

    if (inputData.empty())
    {
        NS_LOG_WARN("Input data for encryption is empty.");
        return {};
    }

    try {
        using namespace CryptoPP;

        AutoSeededRandomPool prng;

        // Generate random key and IV
        SecByteBlock key(AES::DEFAULT_KEYLENGTH);
        prng.GenerateBlock(key, key.size());

        SecByteBlock iv(AES::BLOCKSIZE);
        prng.GenerateBlock(iv, iv.size());

        // Store key and IV for later decryption
        m_key.assign(key.begin(), key.end());
        m_iv.assign(iv.begin(), iv.end());

        // Convert input data to string for Crypto++ processing
        std::string plaintext(inputData.begin(), inputData.end());
        std::string ciphertext;

        // Perform AES encryption in CBC mode
        CBC_Mode<AES>::Encryption encryption;
        encryption.SetKeyWithIV(key, key.size(), iv);

        StringSource ss(plaintext, true,
            new StreamTransformationFilter(encryption,
                new StringSink(ciphertext)
            )
        );

        // Combine key + iv + ciphertext for transmission
        std::vector<uint8_t> result;
        result.reserve(key.size() + iv.size() + ciphertext.size());
        
        // Add key
        result.insert(result.end(), key.begin(), key.end());
        // Add IV
        result.insert(result.end(), iv.begin(), iv.end());
        // Add encrypted data
        result.insert(result.end(), ciphertext.begin(), ciphertext.end());

        NS_LOG_INFO("Encryption successful. Input: " << inputData.size() 
                   << " bytes, Output: " << result.size() << " bytes");
        
        return result;
    }
    catch (const CryptoPP::Exception& e)
    {
        NS_LOG_ERROR("Crypto++ encryption error: " << e.what());
        return {};
    }
    catch (const std::exception& e)
    {
        NS_LOG_ERROR("Standard encryption error: " << e.what());
        return {};
    }
}

std::vector<uint8_t> CryptoSim::Decrypt(const std::vector<uint8_t>& encryptedData)
{
    NS_LOG_FUNCTION(this);

    if (encryptedData.empty()) {
        NS_LOG_WARN("Input data for decryption is empty.");
        return {};
    }

    try {
        using namespace CryptoPP;

        // Extract key, IV, and ciphertext from the encrypted data
        const size_t keySize = AES::DEFAULT_KEYLENGTH;
        const size_t ivSize = AES::BLOCKSIZE;
        
        if (encryptedData.size() < keySize + ivSize) {
            NS_LOG_ERROR("Encrypted data too short to contain key and IV");
            return {};
        }

        // Extract key
        SecByteBlock key(encryptedData.data(), keySize);
        
        // Extract IV
        SecByteBlock iv(encryptedData.data() + keySize, ivSize);
        
        // Extract ciphertext
        std::string ciphertext(encryptedData.begin() + keySize + ivSize, encryptedData.end());

        std::string decryptedtext;

        // Perform AES decryption in CBC mode
        CBC_Mode<AES>::Decryption decryption;
        decryption.SetKeyWithIV(key, key.size(), iv);

        StringSource ss(ciphertext, true,
            new StreamTransformationFilter(decryption,
                new StringSink(decryptedtext)
            )
        );

        // Convert back to vector
        std::vector<uint8_t> result(decryptedtext.begin(), decryptedtext.end());

        NS_LOG_INFO("Decryption successful. Input: " << encryptedData.size() 
                   << " bytes, Output: " << result.size() << " bytes");

        return result;
    }
    catch (const CryptoPP::Exception& e)
    {
        NS_LOG_ERROR("Crypto++ decryption error: " << e.what());
        return {};
    }
    catch (const std::exception& e)
    {
        NS_LOG_ERROR("Standard decryption error: " << e.what());
        return {};
    }
}

} // namespace ns3