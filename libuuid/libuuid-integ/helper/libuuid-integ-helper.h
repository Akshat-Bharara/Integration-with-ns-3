#ifndef LIBUUID_INTEG_HELPER_H
#define LIBUUID_INTEG_HELPER_H

#include "ns3/node-container.h"
#include "ns3/node.h"
#include <string>
#include <map>

namespace ns3
{

/**
 * @ingroup libuuid-integ
 * @brief Helper class for managing UUIDs in ns-3 nodes
 * 
 * This helper class provides functionality to assign unique UUIDs to nodes
 * and retrieve them when needed. It uses the libuuid library to generate
 * RFC 4122 compliant UUIDs.
 */
class UuidHelper
{
public:
    /**
     * Constructor
     */
    UuidHelper();

    /**
     * Destructor
     */
    ~UuidHelper();

    /**
     * Assign UUIDs to all nodes in the container
     * @param nodes The container of nodes to assign UUIDs to
     */
    void AssignUuids(const NodeContainer& nodes);

    /**
     * Get the UUID for a specific node
     * @param node The node to get the UUID for
     * @return The UUID string for the node
     */
    std::string GetNodeUuid(Ptr<Node> node);

    /**
     * Print the UUIDs of all nodes in the container
     * @param nodes The container of nodes to print UUIDs for
     */
    void PrintNodeUuids(const NodeContainer& nodes);

private:
    std::map<uint32_t, std::string> m_nodeUuids; ///< Map of node IDs to UUIDs

    /**
     * Generate a new UUID string
     * @return A new UUID string
     */
    std::string GenerateUuid();
};

} // namespace ns3

#endif // LIBUUID_INTEG_HELPER_H
