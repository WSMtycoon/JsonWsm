#pragma once

#include <string>
#include <map>
#include <any>
#include <vector>
#include <memory>

enum class JsonType {
    NULL_TYPE,
    BOOL,
    INT,
    DOUBLE,
    ARRAY,
    OBJECT
};

class JsonWSM {
public:
    JsonWSM();
    ~JsonWSM();

    // Constructor with string input
    JsonWSM(const std::string& jsonStr);
    
    // Value access methods
    std::any operator[](const std::string& key) const;
    std::any getValue(const std::string& path) const;
    
    // Type checking methods
    JsonType getType(const std::string& key) const;
    bool isBool(const std::string& key) const;
    bool isInt(const std::string& key) const;
    bool isDouble(const std::string& key) const;
    bool isArray(const std::string& key) const;
    bool isObject(const std::string& key) const;

private:
    std::map<std::string, std::any> data;
    
    // Helper methods
    void parseJson(const std::string& jsonStr);
    std::any parseValue(const std::string& value);
    JsonType determineType(const std::string& value) const;
    std::any getNestedValue(const std::string& path) const;
    std::vector<std::string> splitPath(const std::string& path) const;
};
