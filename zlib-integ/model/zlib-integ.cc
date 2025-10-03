#include "zlib-integ.h"
#include "ns3/log.h"
#include <zlib.h> // The zlib library header
#include <iostream>

namespace ns3 {

// Define a logging component for this module
NS_LOG_COMPONENT_DEFINE("ZlibInteg");

TypeId ZlibInteg::GetTypeId(void)
{
  static TypeId tid = TypeId("ns3::ZlibInteg")
    .SetParent<Object>()
    .SetGroupName("ZlibInteg");
  return tid;
}

ZlibInteg::ZlibInteg() {
    NS_LOG_FUNCTION(this);
}

ZlibInteg::~ZlibInteg() {
    NS_LOG_FUNCTION(this);
}

std::string ZlibInteg::GetVersion()
{
    return zlibVersion();
}

// --- Implementation of Deflate (Compression) using the one-shot compress() function ---
std::vector<uint8_t> ZlibInteg::Deflate(const std::vector<uint8_t>& inputData)
{
    NS_LOG_FUNCTION(this);

    if (inputData.empty())
    {
        NS_LOG_WARN("Input data for deflate is empty.");
        return {};
    }

    // Determine the maximum possible compressed size using compressBound
    uLongf compressedSize = compressBound(inputData.size());
    
    // Create a vector to hold the compressed data
    std::vector<uint8_t> compressedData(compressedSize);

    // Perform the compression in a single call
    int result = compress(
        compressedData.data(),      // destination buffer
        &compressedSize,            // destination size (will be updated)
        inputData.data(),           // source buffer
        inputData.size()            // source size
    );

    if (result != Z_OK)
    {
        NS_LOG_ERROR("compress failed with error code: " << result);
        return {}; // Return empty vector on failure
    }

    // Resize the vector to the actual compressed size
    compressedData.resize(compressedSize);

    return compressedData;
}


// --- Implementation of Inflate (Decompression) using streaming approach ---
std::vector<uint8_t> ZlibInteg::Inflate(const std::vector<uint8_t>& compressedData)
{
    NS_LOG_FUNCTION(this);

    if (compressedData.empty()) {
        NS_LOG_WARN("Input data for inflate is empty.");
        return {};
    }

    // Initialize zlib stream structure
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;

    // Initialize the stream for decompression
    int result = inflateInit(&stream);
    if (result != Z_OK) {
        NS_LOG_ERROR("inflateInit failed with error code: " << result);
        return {};
    }

    // Set up input data
    stream.avail_in = compressedData.size();
    stream.next_in = const_cast<Bytef*>(compressedData.data());

    // Output buffer - we'll grow this as needed
    std::vector<uint8_t> decompressedData;
    const size_t CHUNK = 16384; // 16KB chunks
    std::vector<uint8_t> outBuffer(CHUNK);

    do {
        stream.avail_out = CHUNK;
        stream.next_out = outBuffer.data();

        result = inflate(&stream, Z_NO_FLUSH);
        
        if (result == Z_STREAM_ERROR || result == Z_NEED_DICT || 
            result == Z_DATA_ERROR || result == Z_MEM_ERROR) {
            NS_LOG_ERROR("inflate failed with error code: " << result);
            inflateEnd(&stream);
            return {};
        }

        size_t have = CHUNK - stream.avail_out;
        decompressedData.insert(decompressedData.end(), outBuffer.begin(), outBuffer.begin() + have);
        
    } while (stream.avail_out == 0);

    // Clean up
    inflateEnd(&stream);

    if (result != Z_STREAM_END) {
        NS_LOG_ERROR("inflate did not complete successfully, error code: " << result);
        return {};
    }

    return decompressedData;
}

} // namespace ns3

