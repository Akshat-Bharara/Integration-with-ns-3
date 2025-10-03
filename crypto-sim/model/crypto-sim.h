#ifndef CRYPTO_SIM_H
#define CRYPTO_SIM_H

#include "ns3/object.h"
#include <string>
#include <vector>
#include <cstdint> // Required for uint8_t

namespace ns3 {

/**
 * @brief An ns-3 object that integrates cryptographic functionality using Crypto++.
 */
class CryptoSim : public Object
{
public:
  static TypeId GetTypeId(void);
  CryptoSim();
  ~CryptoSim();

  /**
   * @brief Gets the version information of the crypto module.
   */
  std::string GetVersion();

  /**
   * @brief Encrypts data using AES encryption in CBC mode.
   *
   * @param inputData A vector of bytes to be encrypted.
   * @return A vector of bytes containing the encrypted data (key + iv + ciphertext).
   * Returns an empty vector on failure.
   */
  std::vector<uint8_t> Encrypt(const std::vector<uint8_t>& inputData);

  /**
   * @brief Decrypts data using AES decryption in CBC mode.
   *
   * @param encryptedData A vector of bytes to be decrypted (key + iv + ciphertext).
   * @return A vector of bytes containing the original, decrypted data.
   * Returns an empty vector on failure.
   */
  std::vector<uint8_t> Decrypt(const std::vector<uint8_t>& encryptedData);

private:
  std::vector<uint8_t> m_key;  // Store the last used key for decryption
  std::vector<uint8_t> m_iv;   // Store the last used IV for decryption
};

} // namespace ns3

#endif /* CRYPTO_SIM_H */