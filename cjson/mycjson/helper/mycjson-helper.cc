#include "mycjson-helper.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/simulator.h"
#include <fstream>
#include <sstream>
#include <ctime>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("CJsonHelper");

// Initialize static member
Ptr<CJsonWrapper> JsonConfigHelper::s_config = nullptr;

CJsonHelper::CJsonHelper()
{
    NS_LOG_FUNCTION(this);
    m_factory.SetTypeId("ns3::CJsonWrapper");
}

CJsonHelper::~CJsonHelper()
{
    NS_LOG_FUNCTION(this);
}

Ptr<CJsonWrapper>
CJsonHelper::Create()
{
    NS_LOG_FUNCTION(this);
    Ptr<CJsonWrapper> wrapper = m_factory.Create<CJsonWrapper>();
    wrapper->CreateObject();
    return wrapper;
}

Ptr<CJsonWrapper>
CJsonHelper::CreateFromString(const std::string& jsonConfig)
{
    NS_LOG_FUNCTION(this << jsonConfig);
    Ptr<CJsonWrapper> wrapper = m_factory.Create<CJsonWrapper>();
    if (wrapper->ParseString(jsonConfig))
    {
        return wrapper;
    }
    return nullptr;
}

Ptr<CJsonWrapper>
CJsonHelper::CreateSimulationConfig(double simulationTime, 
                                    uint32_t nodes,
                                    const std::map<std::string, std::string>& additionalParams)
{
    NS_LOG_FUNCTION(this << simulationTime << nodes);
    Ptr<CJsonWrapper> wrapper = Create();
    if (!wrapper)
    {
        return nullptr;
    }
    
    // Add basic simulation parameters
    wrapper->AddNumber("simulationTime", simulationTime);
    wrapper->AddNumber("nodeCount", static_cast<double>(nodes));
    wrapper->AddString("timestamp", std::to_string(std::time(nullptr)));
    
    // Add additional parameters
    for (const auto& param : additionalParams)
    {
        wrapper->AddString(param.first, param.second);
    }
    
    return wrapper;
}

Ptr<CJsonWrapper>
CJsonHelper::CreateTopologyInfo(const NodeContainer& nodes)
{
    NS_LOG_FUNCTION(this);
    Ptr<CJsonWrapper> wrapper = Create();
    if (!wrapper)
    {
        return nullptr;
    }
    
    wrapper->AddNumber("nodeCount", static_cast<double>(nodes.GetN()));
    
    // Add node IDs
    std::vector<double> nodeIds;
    for (uint32_t i = 0; i < nodes.GetN(); ++i)
    {
        nodeIds.push_back(static_cast<double>(nodes.Get(i)->GetId()));
    }
    wrapper->AddNumberArray("nodeIds", nodeIds);
    
    return wrapper;
}

bool
CJsonHelper::SaveToFile(Ptr<CJsonWrapper> jsonWrapper, 
                       const std::string& filename, 
                       bool formatted)
{
    NS_LOG_FUNCTION(filename << formatted);
    if (!jsonWrapper || !jsonWrapper->IsValid())
    {
        NS_LOG_ERROR("Invalid JSON wrapper");
        return false;
    }
    
    std::ofstream file(filename);
    if (!file.is_open())
    {
        NS_LOG_ERROR("Cannot open file for writing: " << filename);
        return false;
    }
    
    file << jsonWrapper->ToString(formatted);
    file.close();
    return true;
}

Ptr<CJsonWrapper>
CJsonHelper::LoadFromFile(const std::string& filename)
{
    NS_LOG_FUNCTION(filename);
    std::ifstream file(filename);
    if (!file.is_open())
    {
        NS_LOG_ERROR("Cannot open file for reading: " << filename);
        return nullptr;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    CJsonHelper helper;
    return helper.CreateFromString(buffer.str());
}

Ptr<CJsonWrapper>
CJsonHelper::CreateLogEntry(double timestamp,
                           uint32_t nodeId,
                           const std::string& event,
                           const std::map<std::string, std::string>& data)
{
    NS_LOG_FUNCTION(timestamp << nodeId << event);
    CJsonHelper helper;
    Ptr<CJsonWrapper> wrapper = helper.Create();
    if (!wrapper)
    {
        return nullptr;
    }
    
    wrapper->AddNumber("timestamp", timestamp);
    wrapper->AddNumber("nodeId", static_cast<double>(nodeId));
    wrapper->AddString("event", event);
    
    // Add additional data
    for (const auto& item : data)
    {
        wrapper->AddString(item.first, item.second);
    }
    
    return wrapper;
}

bool
CJsonHelper::MergeJsonObjects(Ptr<CJsonWrapper> target, Ptr<CJsonWrapper> source)
{
    NS_LOG_FUNCTION(target << source);
    if (!target || !source || !target->IsValid() || !source->IsValid())
    {
        return false;
    }
    
    // Get all keys from source
    std::vector<std::string> keys = source->GetKeys();
    
    for (const auto& key : keys)
    {
        // Try to get different types of values and add them to target
        std::string stringValue;
        double numberValue;
        bool boolValue;
        std::vector<std::string> stringArrayValue;
        std::vector<double> numberArrayValue;
        
        if (source->GetString(key, stringValue))
        {
            target->AddString(key, stringValue);
        }
        else if (source->GetNumber(key, numberValue))
        {
            target->AddNumber(key, numberValue);
        }
        else if (source->GetBool(key, boolValue))
        {
            target->AddBool(key, boolValue);
        }
        else if (source->GetStringArray(key, stringArrayValue))
        {
            target->AddStringArray(key, stringArrayValue);
        }
        else if (source->GetNumberArray(key, numberArrayValue))
        {
            target->AddNumberArray(key, numberArrayValue);
        }
    }
    
    return true;
}

// JsonConfigHelper implementation
bool
JsonConfigHelper::LoadConfig(const std::string& configFile)
{
    NS_LOG_FUNCTION(configFile);
    s_config = CJsonHelper::LoadFromFile(configFile);
    return s_config != nullptr && s_config->IsValid();
}

std::string
JsonConfigHelper::GetStringValue(const std::string& path, const std::string& defaultValue)
{
    NS_LOG_FUNCTION(path << defaultValue);
    if (!s_config || !s_config->IsValid())
    {
        return defaultValue;
    }
    
    // For now, we'll implement a simple single-level path lookup
    // A more sophisticated implementation would handle dot-separated paths
    std::string value;
    if (s_config->GetString(path, value))
    {
        return value;
    }
    return defaultValue;
}

double
JsonConfigHelper::GetDoubleValue(const std::string& path, double defaultValue)
{
    NS_LOG_FUNCTION(path << defaultValue);
    if (!s_config || !s_config->IsValid())
    {
        return defaultValue;
    }
    
    double value;
    if (s_config->GetNumber(path, value))
    {
        return value;
    }
    return defaultValue;
}

int
JsonConfigHelper::GetIntValue(const std::string& path, int defaultValue)
{
    NS_LOG_FUNCTION(path << defaultValue);
    if (!s_config || !s_config->IsValid())
    {
        return defaultValue;
    }
    
    double value;
    if (s_config->GetNumber(path, value))
    {
        return static_cast<int>(value);
    }
    return defaultValue;
}

bool
JsonConfigHelper::GetBoolValue(const std::string& path, bool defaultValue)
{
    NS_LOG_FUNCTION(path << defaultValue);
    if (!s_config || !s_config->IsValid())
    {
        return defaultValue;
    }
    
    bool value;
    if (s_config->GetBool(path, value))
    {
        return value;
    }
    return defaultValue;
}

bool
JsonConfigHelper::HasPath(const std::string& path)
{
    NS_LOG_FUNCTION(path);
    if (!s_config || !s_config->IsValid())
    {
        return false;
    }
    
    return s_config->HasKey(path);
}

} // namespace ns3
