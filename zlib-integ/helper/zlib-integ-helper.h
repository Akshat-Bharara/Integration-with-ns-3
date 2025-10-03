#ifndef ZLIB_INTEG_HELPER_H
#define ZLIB_INTEG_HELPER_H

#include "ns3/zlib-integ.h"
#include "ns3/ptr.h"

namespace ns3 {

/**
 * @brief Helper class for the zlib integration module
 */
class ZlibIntegHelper
{
public:
  /**
   * @brief Create a ZlibInteg object
   * @return A smart pointer to the created ZlibInteg object
   */
  static Ptr<ZlibInteg> Create();
};

} // namespace ns3

#endif /* ZLIB_INTEG_HELPER_H */
