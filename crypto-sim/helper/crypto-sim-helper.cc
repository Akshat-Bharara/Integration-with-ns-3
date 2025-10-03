#include "crypto-sim-helper.h"
#include "ns3/object-factory.h"

namespace ns3 {

Ptr<CryptoSim> 
CryptoSimHelper::Create()
{
  return CreateObject<CryptoSim>();
}

} // namespace ns3