#include "zlib-integ-helper.h"
#include "ns3/object-factory.h"

namespace ns3 {

Ptr<ZlibInteg> 
ZlibIntegHelper::Create()
{
  return CreateObject<ZlibInteg>();
}

} // namespace ns3
