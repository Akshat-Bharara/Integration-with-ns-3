#ifndef UTHASH_INTEG_HELPER_H
#define UTHASH_INTEG_HELPER_H

#include "ns3/uthash-integ.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"
#include <string>

namespace ns3
{

/**
 * @ingroup uthash-integ
 * @brief Helper class for managing routing tables in ns-3 simulations
 */
class HashTableHelper
{
public:
    HashTableHelper();
    virtual ~HashTableHelper();

    /**
     * @brief Create a HashTableWrapper object
     */
    Ptr<HashTableWrapper> Create();

    /**
     * @brief Create a routing table for a node
     */
    Ptr<HashTableWrapper> CreateRoutingTable(Ptr<Node> node = nullptr);

    /**
     * @brief Add a route to a node's routing table
     */
    bool AddRoute(Ptr<HashTableWrapper> routingTable,
                 Ipv4Address destination,
                 Ipv4Address nextHop,
                 uint32_t interface,
                 uint32_t metric);

    /**
     * @brief Print a node's routing table
     */
    void PrintRoutingTable(Ptr<HashTableWrapper> routingTable);

private:
    ObjectFactory m_factory; ///< Object factory for creating HashTableWrapper instances
};

} // namespace ns3

#endif // UTHASH_INTEG_HELPER_H