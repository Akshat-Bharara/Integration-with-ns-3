#ifndef MYCJSON_H
#define MYCJSON_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include <string>
#include <vector>
#include <map>

// Add a doxygen group for this module.
// If you have more than one file, this should be in only one of them.
/**
 * @defgroup mycjson cJSON Integration Module
 * 
 * This module provides a wrapper around the cJSON library for JSON parsing
 * and creation within ns-3 simulations.
 */

namespace ns3
{

/**
 * @ingroup mycjson
 * @brief A wrapper class for cJSON functionality in ns-3
 * 
 * This class provides convenient methods for creating, parsing, and manipulating
 * JSON objects within ns-3 simulations. It wraps the cJSON library functionality
 * in an ns-3 compatible interface.
 */
class CJsonWrapper : public Object
{
  public:
    /**
     * @brief Get the type ID
     * @return the object TypeId
     */
    static TypeId GetTypeId();

    /**
     * @brief Constructor
     */
    CJsonWrapper();

    /**
     * @brief Destructor
     */
    virtual ~CJsonWrapper();

    /**
     * @brief Create a new JSON object
     * @return true if successful, false otherwise
     */
    bool CreateObject();

    /**
     * @brief Create a new JSON array
     * @return true if successful, false otherwise
     */
    bool CreateArray();

    /**
     * @brief Parse a JSON string
     * @param jsonString the JSON string to parse
     * @return true if parsing was successful, false otherwise
     */
    bool ParseString(const std::string& jsonString);

    /**
     * @brief Add a string value to the JSON object
     * @param key the key name
     * @param value the string value
     * @return true if successful, false otherwise
     */
    bool AddString(const std::string& key, const std::string& value);

    /**
     * @brief Add a number value to the JSON object
     * @param key the key name
     * @param value the numeric value
     * @return true if successful, false otherwise
     */
    bool AddNumber(const std::string& key, double value);

    /**
     * @brief Add a boolean value to the JSON object
     * @param key the key name
     * @param value the boolean value
     * @return true if successful, false otherwise
     */
    bool AddBool(const std::string& key, bool value);

    /**
     * @brief Add a null value to the JSON object
     * @param key the key name
     * @return true if successful, false otherwise
     */
    bool AddNull(const std::string& key);

    /**
     * @brief Add an array to the JSON object
     * @param key the key name
     * @param values vector of string values for the array
     * @return true if successful, false otherwise
     */
    bool AddStringArray(const std::string& key, const std::vector<std::string>& values);

    /**
     * @brief Add a number array to the JSON object
     * @param key the key name
     * @param values vector of numeric values for the array
     * @return true if successful, false otherwise
     */
    bool AddNumberArray(const std::string& key, const std::vector<double>& values);

    /**
     * @brief Get a string value from the JSON object
     * @param key the key name
     * @param value reference to store the retrieved value
     * @return true if successful, false otherwise
     */
    bool GetString(const std::string& key, std::string& value) const;

    /**
     * @brief Get a number value from the JSON object
     * @param key the key name
     * @param value reference to store the retrieved value
     * @return true if successful, false otherwise
     */
    bool GetNumber(const std::string& key, double& value) const;

    /**
     * @brief Get a boolean value from the JSON object
     * @param key the key name
     * @param value reference to store the retrieved value
     * @return true if successful, false otherwise
     */
    bool GetBool(const std::string& key, bool& value) const;

    /**
     * @brief Get a string array from the JSON object
     * @param key the key name
     * @param values reference to vector to store the retrieved values
     * @return true if successful, false otherwise
     */
    bool GetStringArray(const std::string& key, std::vector<std::string>& values) const;

    /**
     * @brief Get a number array from the JSON object
     * @param key the key name
     * @param values reference to vector to store the retrieved values
     * @return true if successful, false otherwise
     */
    bool GetNumberArray(const std::string& key, std::vector<double>& values) const;

    /**
     * @brief Check if a key exists in the JSON object
     * @param key the key name to check
     * @return true if key exists, false otherwise
     */
    bool HasKey(const std::string& key) const;

    /**
     * @brief Get all keys in the JSON object
     * @return vector of all key names
     */
    std::vector<std::string> GetKeys() const;

    /**
     * @brief Convert the JSON object to a formatted string
     * @param formatted whether to format the output (default: true)
     * @return JSON string representation
     */
    std::string ToString(bool formatted = true) const;

    /**
     * @brief Get the size of the JSON object or array
     * @return number of items
     */
    int GetSize() const;

    /**
     * @brief Clear the JSON object/array
     */
    void Clear();

    /**
     * @brief Check if the JSON object is valid
     * @return true if valid, false otherwise
     */
    bool IsValid() const;

    /**
     * @brief Remove a key from the JSON object
     * @param key the key to remove
     * @return true if successful, false otherwise
     */
    bool RemoveKey(const std::string& key);

  private:
    void* m_json; ///< Pointer to the cJSON object (void* to avoid including cjson.h in header)
    
    /**
     * @brief Clean up the internal cJSON object
     */
    void Cleanup();
};

/**
 * @ingroup mycjson
 * @brief Utility functions for JSON operations
 */
class JsonUtils
{
  public:
    /**
     * @brief Validate a JSON string
     * @param jsonString the JSON string to validate
     * @return true if valid, false otherwise
     */
    static bool ValidateJsonString(const std::string& jsonString);

    /**
     * @brief Pretty print a JSON string
     * @param jsonString the JSON string to format
     * @return formatted JSON string
     */
    static std::string PrettyPrint(const std::string& jsonString);

    /**
     * @brief Minify a JSON string (remove whitespace)
     * @param jsonString the JSON string to minify
     * @return minified JSON string
     */
    static std::string Minify(const std::string& jsonString);

    /**
     * @brief Convert a map to JSON string
     * @param data the map to convert
     * @return JSON string representation
     */
    static std::string MapToJson(const std::map<std::string, std::string>& data);

    /**
     * @brief Convert a JSON string to map
     * @param jsonString the JSON string
     * @param data reference to map to store the data
     * @return true if successful, false otherwise
     */
    static bool JsonToMap(const std::string& jsonString, std::map<std::string, std::string>& data);
};

} // namespace ns3

#endif // MYCJSON_H
