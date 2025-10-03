#include "mycjson.h"
#include "ns3/log.h"
#include "ns3/type-id.h"
#include <cjson/cJSON.h>
#include <sstream>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("CJsonWrapper");

NS_OBJECT_ENSURE_REGISTERED(CJsonWrapper);

TypeId
CJsonWrapper::GetTypeId()
{
    static TypeId tid = TypeId("ns3::CJsonWrapper")
                            .SetParent<Object>()
                            .SetGroupName("MyCJson")
                            .AddConstructor<CJsonWrapper>();
    return tid;
}

CJsonWrapper::CJsonWrapper() : m_json(nullptr)
{
    NS_LOG_FUNCTION(this);
}

CJsonWrapper::~CJsonWrapper()
{
    NS_LOG_FUNCTION(this);
    Cleanup();
}

void
CJsonWrapper::Cleanup()
{
    if (m_json)
    {
        cJSON_Delete(static_cast<cJSON*>(m_json));
        m_json = nullptr;
    }
}

bool
CJsonWrapper::CreateObject()
{
    NS_LOG_FUNCTION(this);
    Cleanup();
    m_json = cJSON_CreateObject();
    return m_json != nullptr;
}

bool
CJsonWrapper::CreateArray()
{
    NS_LOG_FUNCTION(this);
    Cleanup();
    m_json = cJSON_CreateArray();
    return m_json != nullptr;
}

bool
CJsonWrapper::ParseString(const std::string& jsonString)
{
    NS_LOG_FUNCTION(this << jsonString);
    Cleanup();
    m_json = cJSON_Parse(jsonString.c_str());
    if (!m_json)
    {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != nullptr)
        {
            NS_LOG_ERROR("JSON Parse Error: " << error_ptr);
        }
        return false;
    }
    return true;
}

bool
CJsonWrapper::AddString(const std::string& key, const std::string& value)
{
    NS_LOG_FUNCTION(this << key << value);
    if (!m_json || !cJSON_IsObject(static_cast<cJSON*>(m_json)))
    {
        NS_LOG_ERROR("JSON object not initialized or not an object");
        return false;
    }
    
    cJSON* json_string = cJSON_CreateString(value.c_str());
    if (!json_string)
    {
        return false;
    }
    
    return cJSON_AddItemToObject(static_cast<cJSON*>(m_json), key.c_str(), json_string);
}

bool
CJsonWrapper::AddNumber(const std::string& key, double value)
{
    NS_LOG_FUNCTION(this << key << value);
    if (!m_json || !cJSON_IsObject(static_cast<cJSON*>(m_json)))
    {
        NS_LOG_ERROR("JSON object not initialized or not an object");
        return false;
    }
    
    cJSON* json_number = cJSON_CreateNumber(value);
    if (!json_number)
    {
        return false;
    }
    
    return cJSON_AddItemToObject(static_cast<cJSON*>(m_json), key.c_str(), json_number);
}

bool
CJsonWrapper::AddBool(const std::string& key, bool value)
{
    NS_LOG_FUNCTION(this << key << value);
    if (!m_json || !cJSON_IsObject(static_cast<cJSON*>(m_json)))
    {
        NS_LOG_ERROR("JSON object not initialized or not an object");
        return false;
    }
    
    cJSON* json_bool = cJSON_CreateBool(value);
    if (!json_bool)
    {
        return false;
    }
    
    return cJSON_AddItemToObject(static_cast<cJSON*>(m_json), key.c_str(), json_bool);
}

bool
CJsonWrapper::AddNull(const std::string& key)
{
    NS_LOG_FUNCTION(this << key);
    if (!m_json || !cJSON_IsObject(static_cast<cJSON*>(m_json)))
    {
        NS_LOG_ERROR("JSON object not initialized or not an object");
        return false;
    }
    
    cJSON* json_null = cJSON_CreateNull();
    if (!json_null)
    {
        return false;
    }
    
    return cJSON_AddItemToObject(static_cast<cJSON*>(m_json), key.c_str(), json_null);
}

bool
CJsonWrapper::AddStringArray(const std::string& key, const std::vector<std::string>& values)
{
    NS_LOG_FUNCTION(this << key);
    if (!m_json || !cJSON_IsObject(static_cast<cJSON*>(m_json)))
    {
        NS_LOG_ERROR("JSON object not initialized or not an object");
        return false;
    }
    
    cJSON* json_array = cJSON_CreateArray();
    if (!json_array)
    {
        return false;
    }
    
    for (const auto& value : values)
    {
        cJSON* json_string = cJSON_CreateString(value.c_str());
        if (!json_string)
        {
            cJSON_Delete(json_array);
            return false;
        }
        cJSON_AddItemToArray(json_array, json_string);
    }
    
    return cJSON_AddItemToObject(static_cast<cJSON*>(m_json), key.c_str(), json_array);
}

bool
CJsonWrapper::AddNumberArray(const std::string& key, const std::vector<double>& values)
{
    NS_LOG_FUNCTION(this << key);
    if (!m_json || !cJSON_IsObject(static_cast<cJSON*>(m_json)))
    {
        NS_LOG_ERROR("JSON object not initialized or not an object");
        return false;
    }
    
    cJSON* json_array = cJSON_CreateArray();
    if (!json_array)
    {
        return false;
    }
    
    for (const auto& value : values)
    {
        cJSON* json_number = cJSON_CreateNumber(value);
        if (!json_number)
        {
            cJSON_Delete(json_array);
            return false;
        }
        cJSON_AddItemToArray(json_array, json_number);
    }
    
    return cJSON_AddItemToObject(static_cast<cJSON*>(m_json), key.c_str(), json_array);
}

bool
CJsonWrapper::GetString(const std::string& key, std::string& value) const
{
    NS_LOG_FUNCTION(this << key);
    if (!m_json)
    {
        return false;
    }
    
    cJSON* json_item = cJSON_GetObjectItem(static_cast<cJSON*>(m_json), key.c_str());
    if (!json_item || !cJSON_IsString(json_item))
    {
        return false;
    }
    
    value = cJSON_GetStringValue(json_item);
    return true;
}

bool
CJsonWrapper::GetNumber(const std::string& key, double& value) const
{
    NS_LOG_FUNCTION(this << key);
    if (!m_json)
    {
        return false;
    }
    
    cJSON* json_item = cJSON_GetObjectItem(static_cast<cJSON*>(m_json), key.c_str());
    if (!json_item || !cJSON_IsNumber(json_item))
    {
        return false;
    }
    
    value = cJSON_GetNumberValue(json_item);
    return true;
}

bool
CJsonWrapper::GetBool(const std::string& key, bool& value) const
{
    NS_LOG_FUNCTION(this << key);
    if (!m_json)
    {
        return false;
    }
    
    cJSON* json_item = cJSON_GetObjectItem(static_cast<cJSON*>(m_json), key.c_str());
    if (!json_item || !cJSON_IsBool(json_item))
    {
        return false;
    }
    
    value = cJSON_IsTrue(json_item);
    return true;
}

bool
CJsonWrapper::GetStringArray(const std::string& key, std::vector<std::string>& values) const
{
    NS_LOG_FUNCTION(this << key);
    if (!m_json)
    {
        return false;
    }
    
    cJSON* json_array = cJSON_GetObjectItem(static_cast<cJSON*>(m_json), key.c_str());
    if (!json_array || !cJSON_IsArray(json_array))
    {
        return false;
    }
    
    values.clear();
    cJSON* json_item = nullptr;
    cJSON_ArrayForEach(json_item, json_array)
    {
        if (cJSON_IsString(json_item))
        {
            values.push_back(cJSON_GetStringValue(json_item));
        }
    }
    
    return true;
}

bool
CJsonWrapper::GetNumberArray(const std::string& key, std::vector<double>& values) const
{
    NS_LOG_FUNCTION(this << key);
    if (!m_json)
    {
        return false;
    }
    
    cJSON* json_array = cJSON_GetObjectItem(static_cast<cJSON*>(m_json), key.c_str());
    if (!json_array || !cJSON_IsArray(json_array))
    {
        return false;
    }
    
    values.clear();
    cJSON* json_item = nullptr;
    cJSON_ArrayForEach(json_item, json_array)
    {
        if (cJSON_IsNumber(json_item))
        {
            values.push_back(cJSON_GetNumberValue(json_item));
        }
    }
    
    return true;
}

bool
CJsonWrapper::HasKey(const std::string& key) const
{
    NS_LOG_FUNCTION(this << key);
    if (!m_json)
    {
        return false;
    }
    
    return cJSON_HasObjectItem(static_cast<cJSON*>(m_json), key.c_str());
}

std::vector<std::string>
CJsonWrapper::GetKeys() const
{
    NS_LOG_FUNCTION(this);
    std::vector<std::string> keys;
    
    if (!m_json || !cJSON_IsObject(static_cast<cJSON*>(m_json)))
    {
        return keys;
    }
    
    cJSON* json_item = nullptr;
    cJSON_ArrayForEach(json_item, static_cast<cJSON*>(m_json))
    {
        if (json_item->string)
        {
            keys.push_back(json_item->string);
        }
    }
    
    return keys;
}

std::string
CJsonWrapper::ToString(bool formatted) const
{
    NS_LOG_FUNCTION(this << formatted);
    if (!m_json)
    {
        return "";
    }
    
    char* json_string = nullptr;
    if (formatted)
    {
        json_string = cJSON_Print(static_cast<cJSON*>(m_json));
    }
    else
    {
        json_string = cJSON_PrintUnformatted(static_cast<cJSON*>(m_json));
    }
    
    if (!json_string)
    {
        return "";
    }
    
    std::string result(json_string);
    free(json_string);
    return result;
}

int
CJsonWrapper::GetSize() const
{
    NS_LOG_FUNCTION(this);
    if (!m_json)
    {
        return 0;
    }
    
    return cJSON_GetArraySize(static_cast<cJSON*>(m_json));
}

void
CJsonWrapper::Clear()
{
    NS_LOG_FUNCTION(this);
    Cleanup();
}

bool
CJsonWrapper::IsValid() const
{
    return m_json != nullptr;
}

bool
CJsonWrapper::RemoveKey(const std::string& key)
{
    NS_LOG_FUNCTION(this << key);
    if (!m_json || !cJSON_IsObject(static_cast<cJSON*>(m_json)))
    {
        return false;
    }
    
    cJSON* removed_item = cJSON_DetachItemFromObject(static_cast<cJSON*>(m_json), key.c_str());
    if (removed_item)
    {
        cJSON_Delete(removed_item);
        return true;
    }
    return false;
}

// JsonUtils implementation
bool
JsonUtils::ValidateJsonString(const std::string& jsonString)
{
    cJSON* json = cJSON_Parse(jsonString.c_str());
    if (json)
    {
        cJSON_Delete(json);
        return true;
    }
    return false;
}

std::string
JsonUtils::PrettyPrint(const std::string& jsonString)
{
    cJSON* json = cJSON_Parse(jsonString.c_str());
    if (!json)
    {
        return "";
    }
    
    char* formatted = cJSON_Print(json);
    std::string result = formatted ? formatted : "";
    
    if (formatted)
    {
        free(formatted);
    }
    cJSON_Delete(json);
    return result;
}

std::string
JsonUtils::Minify(const std::string& jsonString)
{
    cJSON* json = cJSON_Parse(jsonString.c_str());
    if (!json)
    {
        return "";
    }
    
    char* minified = cJSON_PrintUnformatted(json);
    std::string result = minified ? minified : "";
    
    if (minified)
    {
        free(minified);
    }
    cJSON_Delete(json);
    return result;
}

std::string
JsonUtils::MapToJson(const std::map<std::string, std::string>& data)
{
    cJSON* json = cJSON_CreateObject();
    if (!json)
    {
        return "";
    }
    
    for (const auto& pair : data)
    {
        cJSON* json_string = cJSON_CreateString(pair.second.c_str());
        if (json_string)
        {
            cJSON_AddItemToObject(json, pair.first.c_str(), json_string);
        }
    }
    
    char* json_string = cJSON_Print(json);
    std::string result = json_string ? json_string : "";
    
    if (json_string)
    {
        free(json_string);
    }
    cJSON_Delete(json);
    return result;
}

bool
JsonUtils::JsonToMap(const std::string& jsonString, std::map<std::string, std::string>& data)
{
    data.clear();
    
    cJSON* json = cJSON_Parse(jsonString.c_str());
    if (!json || !cJSON_IsObject(json))
    {
        if (json)
        {
            cJSON_Delete(json);
        }
        return false;
    }
    
    cJSON* json_item = nullptr;
    cJSON_ArrayForEach(json_item, json)
    {
        if (json_item->string && cJSON_IsString(json_item))
        {
            data[json_item->string] = cJSON_GetStringValue(json_item);
        }
    }
    
    cJSON_Delete(json);
    return true;
}

} // namespace ns3
