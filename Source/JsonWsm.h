#pragma once

#include <string>
#include <map>
#include <any>
#include <vector>
#include <memory>
#include <functional>

namespace WSM {
	
enum class JsonType {
    NULL_TYPE,
    BOOL,
    INT,
    DOUBLE,
    STRING,
    ARRAY,
    OBJECT
};

class JsonMin {
public:
    JsonMin();
    ~JsonMin();

    // Constructor with string input
    JsonMin(const std::string& jsonStr);
    
    // Value access methods
    std::any operator[](const std::string& key) const;
    std::any getValue(const std::string& path) const;
    
    // Type checking methods
    JsonType getType(const std::string& key) const;
    bool isBool(const std::string& key) const;
    bool isInt(const std::string& key) const;
    bool isDouble(const std::string& key) const;
    bool isString(const std::string& key) const;
    bool isArray(const std::string& key) const;
    bool isObject(const std::string& key) const;
    bool isNull(const std::string& key) const;
    
    // Data access method
    const std::map<std::string, std::any>& getData() const { return data; }
    
    // Recursive traversal method
    void traverse(const std::function<void(const std::string& path, const std::any& value, JsonType type)>& callback, 
                 const std::string& prefix = "") const;
    
    // New utility methods
    int getSize(const std::string& key = "") const;
    bool empty(const std::string& key = "") const;
    bool hasField(const std::string& key) const;
    std::vector<std::string> getFields(const std::string& key = "") const;
    std::string getTypeName(JsonType type) const;

private:
    std::map<std::string, std::any> data;
    
    // Helper methods
    void parseJson(const std::string& jsonStr);
    std::any parseValue(const std::string& value);
    JsonType determineType(const std::string& value) const;
    std::any getNestedValue(const std::string& path) const;
    std::vector<std::string> splitPath(const std::string& path) const;
    
    // New helper methods for improved parsing
    std::vector<std::pair<std::string, std::string>> splitJsonObject(const std::string& jsonStr);
    std::string extractJsonValue(const std::string& jsonStr, size_t& pos);
    std::string extractJsonKey(const std::string& jsonStr, size_t& pos);
    std::vector<std::any> parseArray(const std::string& value);
    
    // Helper for recursive traversal
    void traverseHelper(const std::map<std::string, std::any>& obj, 
                       const std::function<void(const std::string& path, const std::any& value, JsonType type)>& callback,
                       const std::string& prefix) const;
};
} // namespace WSM
