#include "JsonWsm.h"
#include <regex>
#include <sstream>
#include <iostream>

WSM::JsonMin::JsonMin() {}

WSM::JsonMin::~JsonMin() {}

WSM::JsonMin::JsonMin(const std::string& jsonStr) {
    parseJson(jsonStr);
}

void WSM::JsonMin::parseJson(const std::string& jsonStr) {
    // Reset parsing state
    correctlyParsed = true;
    
    // Trim whitespace
    std::string trimmed = jsonStr;
    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r\f\v"));
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);
    
    // Check if the string is empty
    if (trimmed.empty()) {
        correctlyParsed = false;
        return;
    }
    
    // Validate overall structure
    if (trimmed[0] != '{' || trimmed[trimmed.length() - 1] != '}') {
        correctlyParsed = false;
        return;
    }

	// Check bracket balance
    int bracketCount = 0;
    bool inString = false;
    bool escaped = false;
    
	for (size_t i = 0; i < trimmed.length(); ++i) {
		char c = trimmed[i];
		
		if (escaped) { escaped = false; continue; }
		
		if (c == '\\') { escaped = true; continue; }
		
		if (c == '"' && !escaped) { inString = !inString; continue; }
		
		if (!inString) {
			if (c == '{') { bracketCount++; }
			else if (c == '}') { 
				bracketCount--;
				if (bracketCount < 0) { correctlyParsed = false; return; } 
			} 
			else if (c == '[') { bracketCount++;} 
			else if (c == ']') {
				bracketCount--;
				if (bracketCount < 0) { correctlyParsed = false; return; }
			}
		}
	}

    if (bracketCount != 0) {
        correctlyParsed = false;
        return;
    }

    // Remove the outer braces
    trimmed = trimmed.substr(1, trimmed.length() - 2);
    
    // Split the JSON object into key-value pairs
    auto pairs = splitJsonObject(trimmed);
    
    // Parse each key-value pair
    for (const auto& pair : pairs) {
        if (pair.first.empty() || pair.second.empty()) {
            correctlyParsed = false;
            return;
        }
        data[pair.first] = parseValue(pair.second);
    }
}

std::vector<std::pair<std::string, std::string>> WSM::JsonMin::splitJsonObject(const std::string& jsonStr) {
    std::vector<std::pair<std::string, std::string>> pairs;
    size_t pos = 0;
    
    while (pos < jsonStr.length()) {
        // Skip whitespace
        while (pos < jsonStr.length() && std::isspace(jsonStr[pos])) ++pos;
        if (pos >= jsonStr.length()) break;
        
        // Extract key
        std::string key = extractJsonKey(jsonStr, pos);
        if (key.empty()) break;
        
        // Skip whitespace and colon
        while (pos < jsonStr.length() && (std::isspace(jsonStr[pos]) || jsonStr[pos] == ':')) ++pos;
        if (pos >= jsonStr.length()) break;
        
        // Extract value
        std::string value = extractJsonValue(jsonStr, pos);
        if (!value.empty()) {
            pairs.emplace_back(key, value);
        } 
        
        // Skip comma and whitespace
        while (pos < jsonStr.length() && (std::isspace(jsonStr[pos]) || jsonStr[pos] == ',')) ++pos;
    }
    
    return pairs;
}

std::string WSM::JsonMin::extractJsonKey(const std::string& jsonStr, size_t& pos) {
    if (pos >= jsonStr.length() || jsonStr[pos] != '"') return "";
    
    ++pos; // Skip opening quote
    size_t start = pos;
    bool escaped = false;
    
    while (pos < jsonStr.length()) {
        if (escaped) {
            escaped = false;
            ++pos;
            continue;
        }
        
        if (jsonStr[pos] == '\\') {
            escaped = true;
            ++pos;
            continue;
        }
        
        if (jsonStr[pos] == '"') {
            size_t length = pos - start;
            ++pos; // Skip closing quote
            return jsonStr.substr(start, length);
        }
        
        ++pos;
    }
    
    return "";
}

std::string WSM::JsonMin::extractJsonValue(const std::string& jsonStr, size_t& pos) {
    // Skip whitespace
    while (pos < jsonStr.length() && std::isspace(jsonStr[pos])) ++pos;
	if (pos >= jsonStr.length()) {
        correctlyParsed = false;
        return "";
    }
    
    size_t start = pos;
    bool inString = false;
    bool escaped = false;
    
    // Handle string values
    if (jsonStr[pos] == '"') {
        inString = true;
        ++pos;
        while (pos < jsonStr.length()) {
            if (escaped) {
                escaped = false;
                ++pos;
                continue;
            }
            
            if (jsonStr[pos] == '\\') {
                escaped = true;
                ++pos;
                continue;
            }
            
            if (jsonStr[pos] == '"' && !escaped) {
                ++pos; // Include closing quote
                return jsonStr.substr(start, pos - start);
            }
            
            ++pos;
        }
        return jsonStr.substr(start, pos - start);
    }
    
    // Handle null
    if (pos + 4 <= jsonStr.length() && jsonStr.substr(pos, 4) == "null") {
        pos += 4;
        return "null";
    }
    
    // Handle objects and arrays
    if (jsonStr[pos] == '{' || jsonStr[pos] == '[') {
        char openChar = jsonStr[pos];
        char closeChar = (openChar == '{') ? '}' : ']';
        
        ++pos; // Skip opening bracket
        int nestLevel = 1; // Counter for nested brackets
        
        while (pos < jsonStr.length() && nestLevel > 0) {
            if (escaped) {
                escaped = false;
                ++pos;
                continue;
            }
            
            if (jsonStr[pos] == '\\') {
                escaped = true;
                ++pos;
                continue;
            }
            
            if (jsonStr[pos] == '"' && !escaped) {
                inString = !inString;
                ++pos;
                continue;
            }
            
            if (!inString) {
                if (jsonStr[pos] == openChar) ++nestLevel;
                else if (jsonStr[pos] == closeChar) --nestLevel;
            }
            ++pos;
        }
        // Return value with opening and closing brackets
        return jsonStr.substr(start, pos - start);
    }
    
    // Handle simple values (numbers, booleans, null)
    while (pos < jsonStr.length()) {
        if (jsonStr[pos] == ',' || jsonStr[pos] == '}' || jsonStr[pos] == ']') {
            break;
        }
        ++pos;
    }
    
    return jsonStr.substr(start, pos - start);
}

std::vector<std::any> WSM::JsonMin::parseArray(const std::string& value) {
    std::vector<std::any> arr;
    
    // Remove outer brackets
    std::string inner = value;
    if (inner.empty() || inner[0] != '[' || inner[inner.length() - 1] != ']') {
        return arr;
    }
    
    inner = inner.substr(1, inner.length() - 2);
    
    // If array is empty, return empty vector
    if (inner.empty()) {
        return arr;
    }
    
    size_t pos = 0;
    while (pos < inner.length()) {
        // Skip whitespace
        while (pos < inner.length() && std::isspace(inner[pos])) ++pos;
        if (pos >= inner.length()) break;
        
        // Extract array element
        size_t start = pos;
        bool inString = false;
        bool escaped = false;
        int nestLevel = 0;  // Level counter for brackets [] and {}
        
        while (pos < inner.length()) {
            if (escaped) {
                escaped = false;
                ++pos;
                continue;
            }
            
            if (inner[pos] == '\\') {
                escaped = true;
                ++pos;
                continue;
            }
            
            if (inner[pos] == '"' && !escaped) {
                inString = !inString;
                ++pos;
                continue;
            }
            
            if (!inString) {
                if (inner[pos] == '[' || inner[pos] == '{') {
                    ++nestLevel;
                }
                else if (inner[pos] == ']' || inner[pos] == '}') {
                    --nestLevel;
                }
                else if (inner[pos] == ',' && nestLevel == 0) {
                    break;
                }
            }
            
            ++pos;
        }
        
        std::string element = inner.substr(start, pos - start);
        if (!element.empty()) {
            // Trim whitespace from the element
            element.erase(0, element.find_first_not_of(" \t\n\r\f\v"));
            element.erase(element.find_last_not_of(" \t\n\r\f\v") + 1);
            
            // Check if the element is a nested array
            if (element[0] == '[' && element[element.length() - 1] == ']') {
                // Parse the nested array
                std::vector<std::any> nestedArr = parseArray(element);
                arr.push_back(nestedArr);
            }
            else {
                // Parse the element
                std::any parsedValue = parseValue(element);
                if (parsedValue.has_value()) {
                    arr.push_back(parsedValue);
                } else if (element == "null") {
                    // Only push null if the element is explicitly "null"
                    arr.push_back(std::any());
                } else {
                    // If the value couldn't be parsed and isn't explicitly null,
                    // treat it as a string
                    arr.push_back(element);
                }
            }
        }
        
        // Skip comma and whitespace
        while (pos < inner.length() && (std::isspace(inner[pos]) || inner[pos] == ',')) ++pos;
    }
    
    return arr;
}

std::any WSM::JsonMin::parseValue(const std::string& value) {
    // Trim whitespace
    std::string trimmed = value;
    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r\f\v"));
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);
    
    // Check for null
    if (trimmed == "null") {
        return std::any();
    }
    
    // Check for boolean (case insensitive)
    std::string lowerTrimmed = trimmed;
    std::transform(lowerTrimmed.begin(), lowerTrimmed.end(), lowerTrimmed.begin(), ::tolower);
    
    if (lowerTrimmed == "true" || lowerTrimmed == "false" || 
        lowerTrimmed == "\"true\"" || lowerTrimmed == "\"false\"") {
        return lowerTrimmed == "true" || lowerTrimmed == "\"true\"";
    }
    
    // Check for string (with or without quotes)
    if (trimmed[0] == '"' && trimmed[trimmed.length() - 1] == '"') {
        return trimmed.substr(1, trimmed.length() - 2);
    }
    
    // Check for array
    if (trimmed[0] == '[' && trimmed[trimmed.length() - 1] == ']') {
        return parseArray(trimmed);
    }
    
    // Check for object
    if (trimmed[0] == '{' && trimmed[trimmed.length() - 1] == '}') {
        JsonMin obj(trimmed);
        return obj.getData();
    }
    
    // Check for float (with f/F suffix)
    if (trimmed.back() == 'f' || trimmed.back() == 'F') {
        std::string numStr = trimmed.substr(0, trimmed.length() - 1);
        // Check for scientific notation
        if (numStr.find('e') != std::string::npos || numStr.find('E') != std::string::npos) {
            return std::stof(numStr);
        }
        // Count decimal places for non-scientific notation
        size_t decimalPos = numStr.find('.');
        if (decimalPos != std::string::npos) {
            size_t decimalPlaces = numStr.length() - decimalPos - 1;
            if (decimalPlaces <= 6) { // Standard float precision
                return std::stof(numStr);
            }
        }
        // If more than 6 decimal places or no decimal point, treat as float
        return std::stof(numStr);
    }
    
    // Check for number
    std::regex intPattern(R"(-?\d+)");
    std::regex doublePattern(R"(-?\d+\.\d+)");
    std::regex scientificPattern1(R"(-?\d+[eE][+-]?\d+)");
    std::regex scientificPattern2(R"(-?\d+\.\d+[eE][+-]?\d+)");
    
    if (std::regex_match(trimmed, scientificPattern1) || 
        std::regex_match(trimmed, scientificPattern2)) {
        return std::stod(trimmed);
    }
    else if (std::regex_match(trimmed, doublePattern)) {
        return std::stod(trimmed);
    }
    else if (std::regex_match(trimmed, intPattern)) {
        return std::stoi(trimmed);
    }
    
    // If none of the above, treat as string
    return trimmed;
}

WSM::JsonType WSM::JsonMin::determineType(const std::string& value) const {
    // Trim whitespace
    std::string trimmed = value;
    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r\f\v"));
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);
    
    if (trimmed == "null") return JsonType::NULL_TYPE;
    if (trimmed == "true" || trimmed == "false") return JsonType::BOOL;
    
    if (trimmed[0] == '"' && trimmed[trimmed.length() - 1] == '"') {
        return JsonType::STRING;
    }
    
    if (trimmed[0] == '[' && trimmed[trimmed.length() - 1] == ']') {
        return JsonType::ARRAY;
    }
    
    if (trimmed[0] == '{' && trimmed[trimmed.length() - 1] == '}') {
        return JsonType::OBJECT;
    }
    
    // Check for float (with f/F suffix)
    if (trimmed.back() == 'f' || trimmed.back() == 'F') {
        std::string numStr = trimmed.substr(0, trimmed.length() - 1);
        size_t decimalPos = numStr.find('.');
        if (decimalPos != std::string::npos) {
            size_t decimalPlaces = numStr.length() - decimalPos - 1;
            if (decimalPlaces <= 7) {
                return JsonType::FLOAT;
            }
        }
        return JsonType::DOUBLE;
    }
    
    // Check for number
    std::regex intPattern(R"(-?\d+)");
    std::regex doublePattern(R"(-?\d+\.\d+)");
    std::regex scientificPattern(R"(-?\d+(\.\d+)?[eE][+-]?\d+)");
    
    if (std::regex_match(trimmed, doublePattern) || 
        std::regex_match(trimmed, scientificPattern)) {
        return JsonType::DOUBLE;
    }
    if (std::regex_match(trimmed, intPattern)) {
        return JsonType::INT;
    }
    
    return JsonType::STRING;
}

std::any WSM::JsonMin::operator[](const std::string& key) const {
    auto it = data.find(key);
    if (it != data.end()) {
        return it->second;
    }
    return std::any();
}

std::any WSM::JsonMin::getValue(const std::string& path) const {
    return getNestedValue(path);
}

std::any WSM::JsonMin::getNestedValue(const std::string& path) const {
    std::vector<std::string> parts = splitPath(path);
    
    const std::map<std::string, std::any>* currentObj = &data;
    std::any currentValue;
    
    for (size_t i = 0; i < parts.size(); ++i) {
        auto it = currentObj->find(parts[i]);
        if (it == currentObj->end()) {
            return std::any();
        }
        
        if (i == parts.size() - 1) {
            return it->second;
        }
        
        try {
            currentObj = &std::any_cast<const std::map<std::string, std::any>&>(it->second);
        }
        catch (const std::bad_any_cast&) {
            return std::any();
        }
    }
    
    return std::any();
}

std::vector<std::string> WSM::JsonMin::splitPath(const std::string& path) const {
    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string part;
    
    while (std::getline(ss, part, '.')) {
        if (!part.empty()) {
            parts.push_back(part);
        }
    }
    
    return parts;
}

void WSM::JsonMin::traverse(const std::function<void(const std::string& path, const std::any& value, JsonType type)>& callback, 
                          const std::string& prefix) const {
    traverseHelper(data, callback, prefix);
}

void WSM::JsonMin::traverseHelper(const std::map<std::string, std::any>& obj, 
                                const std::function<void(const std::string& path, const std::any& value, JsonType type)>& callback,
                                const std::string& prefix) const {
    for (const auto& pair : obj) {
        std::string currentPath = prefix.empty() ? pair.first : prefix + "." + pair.first;
        
        if (pair.second.type() == typeid(std::map<std::string, std::any>)) {
            callback(currentPath, pair.second, JsonType::OBJECT);
            traverseHelper(std::any_cast<const std::map<std::string, std::any>&>(pair.second), callback, currentPath);
        }
        else if (pair.second.type() == typeid(std::vector<std::any>)) {
            callback(currentPath, pair.second, JsonType::ARRAY);
            const auto& arr = std::any_cast<const std::vector<std::any>&>(pair.second);
            for (size_t i = 0; i < arr.size(); ++i) {
                std::string arrayPath = currentPath + "[" + std::to_string(i) + "]";
                JsonType type;
                
                if (arr[i].type() == typeid(std::map<std::string, std::any>)) {
                    type = JsonType::OBJECT;
                    callback(arrayPath, arr[i], type);
                    traverseHelper(std::any_cast<const std::map<std::string, std::any>&>(arr[i]), callback, arrayPath);
                }
                else if (arr[i].type() == typeid(std::vector<std::any>)) {
                    type = JsonType::ARRAY;
                    callback(arrayPath, arr[i], type);
                    const auto& nestedArr = std::any_cast<const std::vector<std::any>&>(arr[i]);
                    for (size_t j = 0; j < nestedArr.size(); ++j) {
                        std::string nestedArrayPath = arrayPath + "[" + std::to_string(j) + "]";
                        JsonType nestedType;
                        
                        if (nestedArr[j].type() == typeid(bool)) nestedType = JsonType::BOOL;
                        else if (nestedArr[j].type() == typeid(int)) nestedType = JsonType::INT;
                        else if (nestedArr[j].type() == typeid(double)) nestedType = JsonType::DOUBLE;
                        else if (nestedArr[j].type() == typeid(std::string)) nestedType = JsonType::STRING;
                        else if (nestedArr[j].type() == typeid(std::vector<std::any>)) nestedType = JsonType::ARRAY;
                        else if (nestedArr[j].type() == typeid(std::map<std::string, std::any>)) nestedType = JsonType::OBJECT;
                        else if (!nestedArr[j].has_value()) nestedType = JsonType::NULL_TYPE;
                        else nestedType = JsonType::STRING;
                        
                        callback(nestedArrayPath, nestedArr[j], nestedType);
                        
                        if (nestedType == JsonType::OBJECT) {
                            traverseHelper(std::any_cast<const std::map<std::string, std::any>&>(nestedArr[j]), callback, nestedArrayPath);
                        }
                    }
                }
                else {
                    if (arr[i].type() == typeid(bool)) type = JsonType::BOOL;
                    else if (arr[i].type() == typeid(int)) type = JsonType::INT;
                    else if (arr[i].type() == typeid(double)) type = JsonType::DOUBLE;
                    else if (arr[i].type() == typeid(std::string)) type = JsonType::STRING;
                    else if (!arr[i].has_value()) type = JsonType::NULL_TYPE;
                    else type = JsonType::STRING;
                    
                    callback(arrayPath, arr[i], type);
                }
            }
        }
        else {
            JsonType type;
            if (pair.second.type() == typeid(bool)) type = JsonType::BOOL;
            else if (pair.second.type() == typeid(int)) type = JsonType::INT;
            else if (pair.second.type() == typeid(double)) type = JsonType::DOUBLE;
            else if (pair.second.type() == typeid(std::string)) type = JsonType::STRING;
            else if (pair.second.type() == typeid(std::vector<std::any>)) type = JsonType::ARRAY;
            else if (!pair.second.has_value()) type = JsonType::NULL_TYPE;
            else type = JsonType::STRING;
            
            callback(currentPath, pair.second, type);
        }
    }
}

WSM::JsonType WSM::JsonMin::getType(const std::string& key) const {
    auto it = data.find(key);
    if (it == data.end()) {
        return JsonType::NULL_TYPE;
    }
    
    const std::any& value = it->second;
    
    if (value.type() == typeid(bool)) return JsonType::BOOL;
    if (value.type() == typeid(int)) return JsonType::INT;
    if (value.type() == typeid(double)) return JsonType::DOUBLE;
    if (value.type() == typeid(std::string)) return JsonType::STRING;
    if (value.type() == typeid(std::vector<std::any>)) return JsonType::ARRAY;
    if (value.type() == typeid(std::map<std::string, std::any>)) return JsonType::OBJECT;
    
    return JsonType::NULL_TYPE;
}

bool WSM::JsonMin::isBool(const std::string& key) const {
    return getType(key) == JsonType::BOOL;
}

bool WSM::JsonMin::isInt(const std::string& key) const {
    return getType(key) == JsonType::INT;
}

bool WSM::JsonMin::isDouble(const std::string& key) const {
    return getType(key) == JsonType::DOUBLE;
}

bool WSM::JsonMin::isString(const std::string& key) const {
    return getType(key) == JsonType::STRING;
}

bool WSM::JsonMin::isArray(const std::string& key) const {
    return getType(key) == JsonType::ARRAY;
}

bool WSM::JsonMin::isObject(const std::string& key) const {
    return getType(key) == JsonType::OBJECT;
}

bool WSM::JsonMin::isNull(const std::string& key) const {
    return getType(key) == JsonType::NULL_TYPE;
}

int WSM::JsonMin::getSize(const std::string& key) const {
    if (key.empty()) {
        return data.size();
    }
    
    auto value = getValue(key);
    if (!value.has_value()) {
        return 0;
    }
    
    if (value.type() == typeid(std::vector<std::any>)) {
        return std::any_cast<const std::vector<std::any>&>(value).size();
    }
    else if (value.type() == typeid(std::map<std::string, std::any>)) {
        return std::any_cast<const std::map<std::string, std::any>&>(value).size();
    }
    
    return 0;
}

bool WSM::JsonMin::empty(const std::string& key) const {
    return getSize(key) == 0;
}

bool WSM::JsonMin::hasField(const std::string& key) const {
    return data.find(key) != data.end();
}

std::vector<std::string> WSM::JsonMin::getFields(const std::string& key) const {
    std::vector<std::string> fields;
    
    if (key.empty()) {
        for (const auto& pair : data) {
            fields.push_back(pair.first);
        }
    }
    else {
        auto value = getValue(key);
        if (value.has_value() && value.type() == typeid(std::map<std::string, std::any>)) {
            const auto& obj = std::any_cast<const std::map<std::string, std::any>&>(value);
            for (const auto& pair : obj) {
                fields.push_back(pair.first);
            }
        }
    }
    
    return fields;
}

std::string WSM::JsonMin::getTypeName(JsonType type) const {
    switch (type) {
        case JsonType::NULL_TYPE: return "null";
        case JsonType::BOOL: return "boolean";
        case JsonType::INT: return "integer";
        case JsonType::FLOAT: return "float";
        case JsonType::DOUBLE: return "double";
        case JsonType::STRING: return "string";
        case JsonType::ARRAY: return "array";
        case JsonType::OBJECT: return "object";
        default: return "unknown";
    }
} 

bool WSM::JsonMin::isFloat(const std::string& key) const {
    return getType(key) == JsonType::FLOAT;
} 