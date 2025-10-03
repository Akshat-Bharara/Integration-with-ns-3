#ifndef MYCJSON_HELPER_H
#define MYCJSON_HELPER_H

#include "ns3/mycjson.h"
#include "ns3/node-container.h"
#include "ns3/application-container.h"
#include "ns3/object-factory.h"
#include <string>

namespace ns3
{

/**
 * @ingroup mycjson
 * @brief Helper class for creating and managing JSON objects in ns-3 simulations
 * 
 * This helper provides convenient methods for setting up JSON objects and
 * integrating them with ns-3 applications and nodes.
 */
class CJsonHelper
{
  public:
    /**
     * @brief Constructor
     */
    CJsonHelper();

    /**
     * @brief Destructor
     */
    virtual ~CJsonHelper();

    /**
     * @brief Create a CJsonWrapper object
     * @return Pointer to the created CJsonWrapper object
     */
    Ptr<CJsonWrapper> Create();

    /**
     * @brief Create a JSON object from a configuration string
     * @param jsonConfig JSON configuration string
     * @return Pointer to the created and configured CJsonWrapper object
     */
    Ptr<CJsonWrapper> CreateFromString(const std::string& jsonConfig);

    /**
     * @brief Create a JSON object with simulation parameters
     * @param simulationTime simulation time in seconds
     * @param nodes number of nodes in the simulation
     * @param additionalParams additional parameters as key-value pairs
     * @return Pointer to the created CJsonWrapper object with simulation parameters
     */
    Ptr<CJsonWrapper> CreateSimulationConfig(double simulationTime, 
                                             uint32_t nodes,
                                             const std::map<std::string, std::string>& additionalParams = {});

    /**
     * @brief Create a JSON object with network topology information
     * @param nodes NodeContainer with the network nodes
     * @return Pointer to the created CJsonWrapper object with topology information
     */
    Ptr<CJsonWrapper> CreateTopologyInfo(const NodeContainer& nodes);

    /**
     * @brief Save JSON object to file
     * @param jsonWrapper the JSON wrapper object
     * @param filename the output filename
     * @param formatted whether to format the output (default: true)
     * @return true if successful, false otherwise
     */
    static bool SaveToFile(Ptr<CJsonWrapper> jsonWrapper, 
                          const std::string& filename, 
                          bool formatted = true);

    /**
     * @brief Load JSON object from file
     * @param filename the input filename
     * @return Pointer to the loaded CJsonWrapper object, or nullptr if failed
     */
    static Ptr<CJsonWrapper> LoadFromFile(const std::string& filename);

    /**
     * @brief Create a JSON log entry
     * @param timestamp simulation timestamp
     * @param nodeId node identifier
     * @param event event description
     * @param data additional data as key-value pairs
     * @return Pointer to the created CJsonWrapper object with log entry
     */
    static Ptr<CJsonWrapper> CreateLogEntry(double timestamp,
                                           uint32_t nodeId,
                                           const std::string& event,
                                           const std::map<std::string, std::string>& data = {});

    /**
     * @brief Merge two JSON objects
     * @param target the target JSON object to merge into
     * @param source the source JSON object to merge from
     * @return true if successful, false otherwise
     */
    static bool MergeJsonObjects(Ptr<CJsonWrapper> target, Ptr<CJsonWrapper> source);

  private:
    ObjectFactory m_factory; ///< Object factory for creating CJsonWrapper instances
};

/**
 * @ingroup mycjson
 * @brief Configuration helper for JSON-based simulation configuration
 */
class JsonConfigHelper
{
  public:
    /**
     * @brief Load simulation configuration from JSON file
     * @param configFile path to the JSON configuration file
     * @return true if configuration was loaded successfully
     */
    static bool LoadConfig(const std::string& configFile);

    /**
     * @brief Get a configuration value as string
     * @param path dot-separated path to the configuration value (e.g., "network.nodes")
     * @param defaultValue default value if path is not found
     * @return the configuration value or default value
     */
    static std::string GetStringValue(const std::string& path, const std::string& defaultValue = "");

    /**
     * @brief Get a configuration value as double
     * @param path dot-separated path to the configuration value
     * @param defaultValue default value if path is not found
     * @return the configuration value or default value
     */
    static double GetDoubleValue(const std::string& path, double defaultValue = 0.0);

    /**
     * @brief Get a configuration value as integer
     * @param path dot-separated path to the configuration value
     * @param defaultValue default value if path is not found
     * @return the configuration value or default value
     */
    static int GetIntValue(const std::string& path, int defaultValue = 0);

    /**
     * @brief Get a configuration value as boolean
     * @param path dot-separated path to the configuration value
     * @param defaultValue default value if path is not found
     * @return the configuration value or default value
     */
    static bool GetBoolValue(const std::string& path, bool defaultValue = false);

    /**
     * @brief Check if a configuration path exists
     * @param path dot-separated path to check
     * @return true if path exists, false otherwise
     */
    static bool HasPath(const std::string& path);

  private:
    static Ptr<CJsonWrapper> s_config; ///< Global configuration object
};

} // namespace ns3

#endif // MYCJSON_HELPER_H
