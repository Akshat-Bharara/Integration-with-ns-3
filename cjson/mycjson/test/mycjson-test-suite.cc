// Include header files from the module to test
#include "ns3/mycjson.h"
#include "ns3/mycjson-helper.h"

// An essential include is test.h
#include "ns3/test.h"

// Do not put your test classes in namespace ns3.  You may find it useful
// to use the using directive to access the ns3 namespace directly
using namespace ns3;

// Add a doxygen group for tests.
// If you have more than one test, this should be in only one of them.
/**
 * @defgroup mycjson-tests Tests for mycjson
 * @ingroup mycjson
 * @ingroup tests
 */

/**
 * @ingroup mycjson-tests
 * Test case for basic JSON operations
 */
class CJsonBasicTestCase : public TestCase
{
  public:
    CJsonBasicTestCase();
    ~CJsonBasicTestCase() override;

  private:
    void DoRun() override;
};

CJsonBasicTestCase::CJsonBasicTestCase()
    : TestCase("CJson basic operations test")
{
}

CJsonBasicTestCase::~CJsonBasicTestCase()
{
}

void
CJsonBasicTestCase::DoRun()
{
    // Test object creation
    Ptr<CJsonWrapper> json = CreateObject<CJsonWrapper>();
    NS_TEST_ASSERT_MSG_NE(json, nullptr, "Failed to create CJsonWrapper object");
    
    // Test object initialization
    NS_TEST_ASSERT_MSG_EQ(json->CreateObject(), true, "Failed to initialize JSON object");
    NS_TEST_ASSERT_MSG_EQ(json->IsValid(), true, "JSON object should be valid after creation");
    
    // Test adding string
    NS_TEST_ASSERT_MSG_EQ(json->AddString("name", "test"), true, "Failed to add string");
    
    // Test adding number
    NS_TEST_ASSERT_MSG_EQ(json->AddNumber("value", 42.5), true, "Failed to add number");
    
    // Test adding boolean
    NS_TEST_ASSERT_MSG_EQ(json->AddBool("enabled", true), true, "Failed to add boolean");
    
    // Test reading values back
    std::string name;
    NS_TEST_ASSERT_MSG_EQ(json->GetString("name", name), true, "Failed to get string");
    NS_TEST_ASSERT_MSG_EQ(name, "test", "String value mismatch");
    
    double value;
    NS_TEST_ASSERT_MSG_EQ(json->GetNumber("value", value), true, "Failed to get number");
    NS_TEST_ASSERT_MSG_EQ_TOL(value, 42.5, 0.001, "Number value mismatch");
    
    bool enabled;
    NS_TEST_ASSERT_MSG_EQ(json->GetBool("enabled", enabled), true, "Failed to get boolean");
    NS_TEST_ASSERT_MSG_EQ(enabled, true, "Boolean value mismatch");
    
    // Test key existence
    NS_TEST_ASSERT_MSG_EQ(json->HasKey("name"), true, "Key 'name' should exist");
    NS_TEST_ASSERT_MSG_EQ(json->HasKey("nonexistent"), false, "Key 'nonexistent' should not exist");
}

/**
 * @ingroup mycjson-tests
 * Test case for JSON array operations
 */
class CJsonArrayTestCase : public TestCase
{
  public:
    CJsonArrayTestCase();
    ~CJsonArrayTestCase() override;

  private:
    void DoRun() override;
};

CJsonArrayTestCase::CJsonArrayTestCase()
    : TestCase("CJson array operations test")
{
}

CJsonArrayTestCase::~CJsonArrayTestCase()
{
}

void
CJsonArrayTestCase::DoRun()
{
    Ptr<CJsonWrapper> json = CreateObject<CJsonWrapper>();
    NS_TEST_ASSERT_MSG_EQ(json->CreateObject(), true, "Failed to initialize JSON object");
    
    // Test string array
    std::vector<std::string> stringArray = {"item1", "item2", "item3"};
    NS_TEST_ASSERT_MSG_EQ(json->AddStringArray("strings", stringArray), true, "Failed to add string array");
    
    std::vector<std::string> retrievedStrings;
    NS_TEST_ASSERT_MSG_EQ(json->GetStringArray("strings", retrievedStrings), true, "Failed to get string array");
    NS_TEST_ASSERT_MSG_EQ(retrievedStrings.size(), 3, "String array size mismatch");
    NS_TEST_ASSERT_MSG_EQ(retrievedStrings[0], "item1", "String array item 0 mismatch");
    NS_TEST_ASSERT_MSG_EQ(retrievedStrings[1], "item2", "String array item 1 mismatch");
    NS_TEST_ASSERT_MSG_EQ(retrievedStrings[2], "item3", "String array item 2 mismatch");
    
    // Test number array
    std::vector<double> numberArray = {1.1, 2.2, 3.3};
    NS_TEST_ASSERT_MSG_EQ(json->AddNumberArray("numbers", numberArray), true, "Failed to add number array");
    
    std::vector<double> retrievedNumbers;
    NS_TEST_ASSERT_MSG_EQ(json->GetNumberArray("numbers", retrievedNumbers), true, "Failed to get number array");
    NS_TEST_ASSERT_MSG_EQ(retrievedNumbers.size(), 3, "Number array size mismatch");
    NS_TEST_ASSERT_MSG_EQ_TOL(retrievedNumbers[0], 1.1, 0.001, "Number array item 0 mismatch");
    NS_TEST_ASSERT_MSG_EQ_TOL(retrievedNumbers[1], 2.2, 0.001, "Number array item 1 mismatch");
    NS_TEST_ASSERT_MSG_EQ_TOL(retrievedNumbers[2], 3.3, 0.001, "Number array item 2 mismatch");
}

/**
 * @ingroup mycjson-tests
 * Test case for JSON parsing
 */
class CJsonParsingTestCase : public TestCase
{
  public:
    CJsonParsingTestCase();
    ~CJsonParsingTestCase() override;

  private:
    void DoRun() override;
};

CJsonParsingTestCase::CJsonParsingTestCase()
    : TestCase("CJson parsing test")
{
}

CJsonParsingTestCase::~CJsonParsingTestCase()
{
}

void
CJsonParsingTestCase::DoRun()
{
    // Test valid JSON parsing
    std::string validJson = R"({"name": "test", "value": 42, "enabled": true})";
    
    Ptr<CJsonWrapper> json = CreateObject<CJsonWrapper>();
    NS_TEST_ASSERT_MSG_EQ(json->ParseString(validJson), true, "Failed to parse valid JSON");
    NS_TEST_ASSERT_MSG_EQ(json->IsValid(), true, "Parsed JSON should be valid");
    
    // Test reading parsed values
    std::string name;
    NS_TEST_ASSERT_MSG_EQ(json->GetString("name", name), true, "Failed to get parsed string");
    NS_TEST_ASSERT_MSG_EQ(name, "test", "Parsed string value mismatch");
    
    double value;
    NS_TEST_ASSERT_MSG_EQ(json->GetNumber("value", value), true, "Failed to get parsed number");
    NS_TEST_ASSERT_MSG_EQ_TOL(value, 42.0, 0.001, "Parsed number value mismatch");
    
    // Test invalid JSON parsing
    std::string invalidJson = R"({"name": "test", "value": })"; // Invalid JSON
    Ptr<CJsonWrapper> json2 = CreateObject<CJsonWrapper>();
    NS_TEST_ASSERT_MSG_EQ(json2->ParseString(invalidJson), false, "Should fail to parse invalid JSON");
}

/**
 * @ingroup mycjson-tests
 * Test case for JSON utilities
 */
class JsonUtilsTestCase : public TestCase
{
  public:
    JsonUtilsTestCase();
    ~JsonUtilsTestCase() override;

  private:
    void DoRun() override;
};

JsonUtilsTestCase::JsonUtilsTestCase()
    : TestCase("JsonUtils test")
{
}

JsonUtilsTestCase::~JsonUtilsTestCase()
{
}

void
JsonUtilsTestCase::DoRun()
{
    // Test JSON validation
    std::string validJson = R"({"name": "test", "value": 42})";
    std::string invalidJson = R"({"name": "test", "value": })";
    
    NS_TEST_ASSERT_MSG_EQ(JsonUtils::ValidateJsonString(validJson), true, "Valid JSON should validate");
    NS_TEST_ASSERT_MSG_EQ(JsonUtils::ValidateJsonString(invalidJson), false, "Invalid JSON should not validate");
    
    // Test map to JSON conversion
    std::map<std::string, std::string> testMap;
    testMap["key1"] = "value1";
    testMap["key2"] = "value2";
    
    std::string jsonFromMap = JsonUtils::MapToJson(testMap);
    NS_TEST_ASSERT_MSG_NE(jsonFromMap.empty(), true, "Map to JSON conversion should not be empty");
    
    // Test JSON to map conversion
    std::map<std::string, std::string> mapFromJson;
    NS_TEST_ASSERT_MSG_EQ(JsonUtils::JsonToMap(jsonFromMap, mapFromJson), true, "JSON to map conversion should succeed");
    NS_TEST_ASSERT_MSG_EQ(mapFromJson.size(), 2, "Converted map should have 2 items");
    NS_TEST_ASSERT_MSG_EQ(mapFromJson["key1"], "value1", "Map value 1 mismatch");
    NS_TEST_ASSERT_MSG_EQ(mapFromJson["key2"], "value2", "Map value 2 mismatch");
}

/**
 * @ingroup mycjson-tests
 * Test case for helper functionality
 */
class CJsonHelperTestCase : public TestCase
{
  public:
    CJsonHelperTestCase();
    ~CJsonHelperTestCase() override;

  private:
    void DoRun() override;
};

CJsonHelperTestCase::CJsonHelperTestCase()
    : TestCase("CJsonHelper test")
{
}

CJsonHelperTestCase::~CJsonHelperTestCase()
{
}

void
CJsonHelperTestCase::DoRun()
{
    CJsonHelper helper;
    
    // Test object creation
    Ptr<CJsonWrapper> json = helper.Create();
    NS_TEST_ASSERT_MSG_NE(json, nullptr, "Helper should create valid JSON object");
    NS_TEST_ASSERT_MSG_EQ(json->IsValid(), true, "Helper-created JSON should be valid");
    
    // Test simulation config creation
    std::map<std::string, std::string> params;
    params["topology"] = "star";
    
    Ptr<CJsonWrapper> config = helper.CreateSimulationConfig(10.0, 5, params);
    NS_TEST_ASSERT_MSG_NE(config, nullptr, "Should create simulation config");
    NS_TEST_ASSERT_MSG_EQ(config->IsValid(), true, "Simulation config should be valid");
    
    // Test reading simulation config values
    double simTime;
    NS_TEST_ASSERT_MSG_EQ(config->GetNumber("simulationTime", simTime), true, "Should get simulation time");
    NS_TEST_ASSERT_MSG_EQ_TOL(simTime, 10.0, 0.001, "Simulation time value mismatch");
    
    double nodeCount;
    NS_TEST_ASSERT_MSG_EQ(config->GetNumber("nodeCount", nodeCount), true, "Should get node count");
    NS_TEST_ASSERT_MSG_EQ_TOL(nodeCount, 5.0, 0.001, "Node count value mismatch");
    
    std::string topology;
    NS_TEST_ASSERT_MSG_EQ(config->GetString("topology", topology), true, "Should get topology");
    NS_TEST_ASSERT_MSG_EQ(topology, "star", "Topology value mismatch");
}

/**
 * @ingroup mycjson-tests
 * TestSuite for module mycjson
 */
class MycjsonTestSuite : public TestSuite
{
  public:
    MycjsonTestSuite();
};

MycjsonTestSuite::MycjsonTestSuite()
    : TestSuite("mycjson", Type::UNIT)
{
    AddTestCase(new CJsonBasicTestCase, TestCase::Duration::QUICK);
    AddTestCase(new CJsonArrayTestCase, TestCase::Duration::QUICK);
    AddTestCase(new CJsonParsingTestCase, TestCase::Duration::QUICK);
    AddTestCase(new JsonUtilsTestCase, TestCase::Duration::QUICK);
    AddTestCase(new CJsonHelperTestCase, TestCase::Duration::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
/**
 * @ingroup mycjson-tests
 * Static variable for test initialization
 */
static MycjsonTestSuite smycjsonTestSuite;
