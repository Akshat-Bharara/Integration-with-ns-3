#include "uthash-integ.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include <sstream>

// Include the UTHash header
#include "uthash.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("HashTableWrapper");

NS_OBJECT_ENSURE_REGISTERED(HashTableWrapper);

// Define the actual structure for routing entries with uthash macros
typedef struct RouteEntryInternal {
    uint32_t destination;       // Key field
    uint32_t nextHop;
    uint32_t interface;
    uint32_t metric;
    UT_hash_handle hh;         // Makes this structure hashable
} RouteEntryInternal;

TypeId
HashTableWrapper::GetTypeId()
{
    static TypeId tid = TypeId("ns3::HashTableWrapper")
                           .SetParent<Object>()
                           .SetGroupName("UthashInteg")
                           .AddConstructor<HashTableWrapper>();
    return tid;
}

HashTableWrapper::HashTableWrapper()
    : m_routeTable(nullptr)
{
    NS_LOG_FUNCTION(this);
}

HashTableWrapper::~HashTableWrapper()
{
    NS_LOG_FUNCTION(this);
    Clear();
}

bool
HashTableWrapper::AddRouteEntry(uint32_t destination, uint32_t nextHop, uint32_t interface, uint32_t metric)
{
    NS_LOG_FUNCTION(this << destination << nextHop << interface << metric);
    
    // First check if this destination already exists
    RouteEntryInternal* existing = nullptr;
    HASH_FIND_INT(static_cast<RouteEntryInternal*>(m_routeTable), &destination, existing);
    
    if (existing) {
        // Update existing entry
        existing->nextHop = nextHop;
        existing->interface = interface;
        existing->metric = metric;
    } else {
        // Create new entry
        RouteEntryInternal* entry = new RouteEntryInternal();
        entry->destination = destination;
        entry->nextHop = nextHop;
        entry->interface = interface;
        entry->metric = metric;
        
        // Add to hash table
        HASH_ADD_INT(static_cast<RouteEntryInternal*>(m_routeTable), destination, entry);
    }
    
    return true;
}

RouteEntry*
HashTableWrapper::FindRoute(uint32_t destination)
{
    NS_LOG_FUNCTION(this << destination);
    
    // Look up the entry
    RouteEntryInternal* entry = nullptr;
    HASH_FIND_INT(static_cast<RouteEntryInternal*>(m_routeTable), &destination, entry);
    
    // Return as the public structure if found
    if (entry) {
        RouteEntry* result = new RouteEntry();
        result->destination = entry->destination;
        result->nextHop = entry->nextHop;
        result->interface = entry->interface;
        result->metric = entry->metric;
        return result;
    }
    
    return nullptr;
}

bool
HashTableWrapper::DeleteRoute(uint32_t destination)
{
    NS_LOG_FUNCTION(this << destination);
    
    RouteEntryInternal* entry = nullptr;
    HASH_FIND_INT(static_cast<RouteEntryInternal*>(m_routeTable), &destination, entry);
    
    if (entry) {
        HASH_DEL(static_cast<RouteEntryInternal*>(m_routeTable), entry);
        delete entry;
        return true;
    }
    
    return false;
}

std::vector<RouteEntry*>
HashTableWrapper::GetAllRoutes()
{
    NS_LOG_FUNCTION(this);
    
    std::vector<RouteEntry*> routes;
    RouteEntryInternal* entry, *tmp;
    
    HASH_ITER(hh, static_cast<RouteEntryInternal*>(m_routeTable), entry, tmp) {
        RouteEntry* route = new RouteEntry();
        route->destination = entry->destination;
        route->nextHop = entry->nextHop;
        route->interface = entry->interface;
        route->metric = entry->metric;
        routes.push_back(route);
    }
    
    return routes;
}

void
HashTableWrapper::Clear()
{
    NS_LOG_FUNCTION(this);
    
    // Clean up routing table
    RouteEntryInternal* route, *tmp;
    HASH_ITER(hh, static_cast<RouteEntryInternal*>(m_routeTable), route, tmp) {
        HASH_DEL(static_cast<RouteEntryInternal*>(m_routeTable), route);
        delete route;
    }
    m_routeTable = nullptr;
}

} // namespace ns3