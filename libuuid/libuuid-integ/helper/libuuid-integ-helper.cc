#include "libuuid-integ-helper.h"
#include "ns3/log.h"
#include <uuid/uuid.h>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("UuidHelper");

UuidHelper::UuidHelper()
{
    NS_LOG_FUNCTION(this);
}

UuidHelper::~UuidHelper()
{
    NS_LOG_FUNCTION(this);
}

void
UuidHelper::AssignUuids(const NodeContainer& nodes)
{
    NS_LOG_FUNCTION(this << &nodes);
    
    for (uint32_t i = 0; i < nodes.GetN(); ++i)
    {
        Ptr<Node> node = nodes.Get(i);
        uint32_t nodeId = node->GetId();
        
        // Generate UUID for this node if not already assigned
        if (m_nodeUuids.find(nodeId) == m_nodeUuids.end())
        {
            std::string uuid = GenerateUuid();
            m_nodeUuids[nodeId] = uuid;
            NS_LOG_INFO("Assigned UUID " << uuid << " to node " << nodeId);
        }
    }
}

std::string
UuidHelper::GetNodeUuid(Ptr<Node> node)
{
    NS_LOG_FUNCTION(this << node);
    
    uint32_t nodeId = node->GetId();
    auto it = m_nodeUuids.find(nodeId);
    
    if (it != m_nodeUuids.end())
    {
        return it->second;
    }
    else
    {
        // Generate UUID if not found
        std::string uuid = GenerateUuid();
        m_nodeUuids[nodeId] = uuid;
        NS_LOG_INFO("Generated new UUID " << uuid << " for node " << nodeId);
        return uuid;
    }
}

void
UuidHelper::PrintNodeUuids(const NodeContainer& nodes)
{
    NS_LOG_FUNCTION(this << &nodes);
    
    for (uint32_t i = 0; i < nodes.GetN(); ++i)
    {
        Ptr<Node> node = nodes.Get(i);
        uint32_t nodeId = node->GetId();
        std::string uuid = GetNodeUuid(node);
        std::cout << "Node " << nodeId << " UUID: " << uuid << std::endl;
    }
}

std::string
UuidHelper::GenerateUuid()
{
    uuid_t uuid;
    uuid_generate_random(uuid);
    
    char uuid_str[37]; // UUID string is 36 characters + null terminator
    uuid_unparse_lower(uuid, uuid_str);
    
    return std::string(uuid_str);
}

} // namespace ns3
