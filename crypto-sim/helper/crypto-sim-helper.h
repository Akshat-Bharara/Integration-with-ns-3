#ifndef CRYPTO_SIM_HELPER_H
#define CRYPTO_SIM_HELPER_H

#include "ns3/crypto-sim.h"
#include "ns3/ptr.h"

namespace ns3 {

/**
 * @brief Helper class for the crypto simulation module
 */
class CryptoSimHelper
{
public:
  /**
   * @brief Create a CryptoSim object
   * @return A smart pointer to the created CryptoSim object
   */
  static Ptr<CryptoSim> Create();
};

} // namespace ns3

#endif /* CRYPTO_SIM_HELPER_H */