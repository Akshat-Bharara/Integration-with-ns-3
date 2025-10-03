#ifndef UTHASH_INTEG_H
#define UTHASH_INTEG_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include <string>
#include <vector>

namespace ns3
{

/**
 * Structure for routing table entries
 */
typedef struct RouteEntry {
    uint32_t destination;       // Destination IP address as integer
    uint32_t nextHop;          // Next hop IP address
    uint32_t interface;        // Interface index
    uint32_t metric;           // Routing metric
    void* hh;                  // Required for UTHash (hash handle)
} RouteEntry;

/**
 * @ingroup uthash-integ
 * @brief A wrapper class for UTHash functionality in ns-3
 * 
 * This class provides methods for managing routing tables using UTHash.
 */
class HashTableWrapper : public Object
{
public:
    static TypeId GetTypeId();
    HashTableWrapper();
    virtual ~HashTableWrapper();

    /**
     * @brief Add a route entry to the routing table
     */
    bool AddRouteEntry(uint32_t destination, uint32_t nextHop, uint32_t interface, uint32_t metric);

    /**
     * @brief Find a route entry by destination
     */
    RouteEntry* FindRoute(uint32_t destination);

    /**
     * @brief Delete a route entry
     */
    bool DeleteRoute(uint32_t destination);

    /**
     * @brief Get all routes in the routing table
     */
    std::vector<RouteEntry*> GetAllRoutes();

    /**
     * @brief Clear all tables and free memory
     */
    void Clear();

private:
    void* m_routeTable;        ///< Pointer to the routing table
};

} // namespace ns3

#endif // UTHASH_INTEG_H