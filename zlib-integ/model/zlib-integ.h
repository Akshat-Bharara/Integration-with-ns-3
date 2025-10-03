#ifndef ZLIB_INTEG_H
#define ZLIB_INTEG_H

#include "ns3/object.h"
#include <string>
#include <vector>
#include <cstdint> // Required for uint8_t

namespace ns3 {

/**
 * @brief An ns-3 object that integrates zlib functionality.
 */
class ZlibInteg : public Object
{
public:
  static TypeId GetTypeId(void);
  ZlibInteg();
  ~ZlibInteg();

  /**
   * @brief Gets the version of the linked zlib library.
   */
  std::string GetVersion();

  /**
   * @brief Compresses data using zlib's deflate algorithm.
   */
  std::vector<uint8_t> Deflate(const std::vector<uint8_t>& inputData);

  /**
   * @brief Decompresses data using zlib's inflate algorithm.
   *
   * @param compressedData A vector of bytes to be decompressed.
   * @return A vector of bytes containing the original, decompressed data.
   * Returns an empty vector on failure.
   */
  std::vector<uint8_t> Inflate(const std::vector<uint8_t>& compressedData);
};

} // namespace ns3

#endif /* ZLIB_INTEG_H */

