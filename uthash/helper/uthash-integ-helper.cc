#include "uthash-integ-helper.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/ipv4-address.h"
#include <sstream>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("HashTableHelper");

HashTableHelper::HashTableHelper()
{
    NS_LOG_FUNCTION(this);
    m_factory.SetTypeId("ns3::HashTableWrapper");
}

HashTableHelper::~HashTableHelper()
{
    NS_LOG_FUNCTION(this);
}

Ptr<HashTableWrapper>
HashTableHelper::Create()
{
    NS_LOG_FUNCTION(this);
    return m_factory.Create<HashTableWrapper>();
}

Ptr<HashTableWrapper>
HashTableHelper::CreateRoutingTable(Ptr<Node> node)
{
    NS_LOG_FUNCTION(this << node);
    Ptr<HashTableWrapper> routingTable = Create();
    
    if (node) {
        NS_LOG_INFO("Creating routing table for node " << node->GetId());
    }
    
    return routingTable;
}

bool
HashTableHelper::AddRoute(Ptr<HashTableWrapper> routingTable,
                         Ipv4Address destination,
                         Ipv4Address nextHop,
                         uint32_t interface,
                         uint32_t metric)
{
    NS_LOG_FUNCTION(this << destination << nextHop << interface << metric);
    return routingTable->AddRouteEntry(destination.Get(), nextHop.Get(), interface, metric);
}

void
HashTableHelper::PrintRoutingTable(Ptr<HashTableWrapper> routingTable)
{
    NS_LOG_FUNCTION(this);
    
    std::vector<RouteEntry*> routes = routingTable->GetAllRoutes();
    
    std::cout << "Routing Table Contents (" << routes.size() << " entries):" << std::endl;
    std::cout << "Destination\tNext Hop" << std::endl;
    std::cout << "------------------------" << std::endl;
    
    for (auto route : routes) {
        std::cout << Ipv4Address(route->destination) << "\t"
                  << Ipv4Address(route->nextHop) << std::endl;
        delete route;
    }
    
    std::cout << "------------------------" << std::endl;
}

} // namespace ns3