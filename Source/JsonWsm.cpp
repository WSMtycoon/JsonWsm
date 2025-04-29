#include "JsonWSM.h"
#include <regex>
#include <sstream>
#include <algorithm>
#include <iostream>


WSM::JsonMin::JsonMin() {}

WSM::JsonMin::~JsonMin() {}

WSM::JsonMin::JsonMin(const std::string& jsonStr) {
    parseJson(jsonStr);
}

void WSM::JsonMin::parseJson(const std::string& jsonStr) {
    // Удаляем внешние фигурные скобки, если они есть
    std::string str = jsonStr;
    if (str.front() == '{' && str.back() == '}') {
        str = str.substr(1, str.length() - 2);
    }
    
    // Разбиваем JSON объект на пары ключ-значение
    auto pairs = splitJsonObject(str);
    
    // Обрабатываем каждую пару
    for (const auto& pair : pairs) {
        const std::string& key = pair.first;
        const std::string& value = pair.second;
        
        // Парсим значение и сохраняем его в карте данных
        data[key] = parseValue(value);
    }
}

std::vector<std::pair<std::string, std::string>> WSM::JsonMin::splitJsonObject(const std::string& jsonStr) {
    std::vector<std::pair<std::string, std::string>> result;
    size_t pos = 0;
    
    // Пропускаем начальные пробелы
    while (pos < jsonStr.length() && std::isspace(jsonStr[pos])) ++pos;
    
    while (pos < jsonStr.length()) {
        // Пропускаем пробелы
        while (pos < jsonStr.length() && std::isspace(jsonStr[pos])) ++pos;
        if (pos >= jsonStr.length()) break;
        
        // Извлекаем ключ
        std::string key = extractJsonKey(jsonStr, pos);
        if (key.empty()) {
            // Если ключ пустой, пропускаем до следующей запятой или конца
            while (pos < jsonStr.length() && jsonStr[pos] != ',') ++pos;
            if (pos < jsonStr.length()) ++pos; // Пропускаем запятую
            continue;
        }
        
        // Пропускаем разделитель
        while (pos < jsonStr.length() && (std::isspace(jsonStr[pos]) || jsonStr[pos] == ':' || jsonStr[pos] == ';')) ++pos;
        if (pos >= jsonStr.length()) break;
        
        // Извлекаем значение
        std::string value = extractJsonValue(jsonStr, pos);
        
        // Добавляем пару в результат
        result.push_back({key, value});
        
        // Пропускаем запятую
        while (pos < jsonStr.length() && (std::isspace(jsonStr[pos]) || jsonStr[pos] == ',')) ++pos;
    }
    
    return result;
}

std::string WSM::JsonMin::extractJsonKey(const std::string& jsonStr, size_t& pos) {
    // Key should start with a quote
    if (jsonStr[pos] != '"') return "";
    
    // Find the end of the key
    size_t start = ++pos;
    while (pos < jsonStr.length() && jsonStr[pos] != '"') ++pos;
    if (pos >= jsonStr.length()) return "";
    
    // Extract the key
    std::string key = jsonStr.substr(start, pos - start);
    ++pos; // Skip the closing quote
    
    return key;
}

std::string WSM::JsonMin::extractJsonValue(const std::string& jsonStr, size_t& pos) {
    // Пропускаем начальные пробелы
    while (pos < jsonStr.length() && std::isspace(jsonStr[pos])) ++pos;
    if (pos >= jsonStr.length()) return "";
    
    size_t start = pos;
    int braceCount = 0;
    int bracketCount = 0;
    bool inString = false;
    bool escaped = false;
    
    // Обрабатываем строковые значения
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
                ++pos; // Включаем закрывающую кавычку
                return jsonStr.substr(start, pos - start);
            }
            
            ++pos;
        }
        return jsonStr.substr(start, pos - start);
    }
    
    // Обрабатываем null
    if (pos + 4 <= jsonStr.length() && jsonStr.substr(pos, 4) == "null") {
        pos += 4;
        return "null";
    }
    
    // Обрабатываем объекты и массивы
    if (jsonStr[pos] == '{' || jsonStr[pos] == '[') {
        char openChar = jsonStr[pos];
        char closeChar = (openChar == '{') ? '}' : ']';
        
        ++pos; // Пропускаем открывающую скобку
        int count = 1; // Счетчик открытых/закрытых скобок
        
        while (pos < jsonStr.length() && count > 0) {
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
                if (jsonStr[pos] == openChar) ++count;
                else if (jsonStr[pos] == closeChar) --count;
            }
            
            ++pos;
        }
        
        // Возвращаем значение с открывающей и закрывающей скобками
        return jsonStr.substr(start, pos - start);
    }
    
    // Обрабатываем простые значения (числа, булевы значения)
    while (pos < jsonStr.length()) {
        if (jsonStr[pos] == ',' || jsonStr[pos] == '}' || jsonStr[pos] == ']') {
            break;
        }
        ++pos;
    }
    
    return jsonStr.substr(start, pos - start);
}

std::any WSM::JsonMin::parseValue(const std::string& value) {
    // Trim whitespace
    std::string trimmed = value;
    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r\f\v"));
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);
    
    // Check for null
    if (trimmed == "null" || trimmed.empty()) {
        return std::any();
    }
    
    // Remove quotes if present
    bool hasQuotes = false;
    if (trimmed.front() == '"' && trimmed.back() == '"') {
        trimmed = trimmed.substr(1, trimmed.length() - 2);
        hasQuotes = true;
    }
    
    // Если значение было в кавычках и после удаления кавычек стало пустым,
    // возвращаем пустую строку
    if (hasQuotes && trimmed.empty()) {
        return std::string("");
    }
    
    // Check for boolean (with or without quotes)
    if (trimmed == "true" || trimmed == "false") {
        return trimmed == "true";
    }
    
    // Check for integer (with or without quotes)
    if (std::regex_match(trimmed, std::regex(R"(-?\d+)"))) {
        return std::stoi(trimmed);
    }
    
    // Check for double (with or without quotes)
    if (std::regex_match(trimmed, std::regex(R"(-?\d+\.\d+)"))) {
        return std::stod(trimmed);
    }
    
    // Check for array
    if (trimmed.front() == '[' && trimmed.back() == ']') {
        std::vector<std::any> arr;
        std::string inner = trimmed.substr(1, trimmed.length() - 2);
        size_t pos = 0;
        
        while (pos < inner.length()) {
            // Skip whitespace
            while (pos < inner.length() && std::isspace(inner[pos])) ++pos;
            if (pos >= inner.length()) break;
            
            // Extract array element
            size_t start = pos;
            int bracketCount = 0;
            int braceCount = 0;
            bool inString = false;
            bool escaped = false;
            
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
                    if (inner[pos] == '[') ++bracketCount;
                    else if (inner[pos] == ']') --bracketCount;
                    else if (inner[pos] == '{') ++braceCount;
                    else if (inner[pos] == '}') --braceCount;
                    else if (inner[pos] == ',' && bracketCount == 0 && braceCount == 0) break;
                }
                
                ++pos;
            }
            
            std::string element = inner.substr(start, pos - start);
            if (!element.empty()) {
                arr.push_back(parseValue(element));
            }
            
            // Skip comma
            while (pos < inner.length() && (std::isspace(inner[pos]) || inner[pos] == ',')) ++pos;
        }
        
        return arr;
    }
    
    // Check for object
    if (trimmed.front() == '{' && trimmed.back() == '}') {
        JsonMin obj(trimmed);
        return obj.data;
    }
    
    // If it was a quoted string, return it as is
    if (hasQuotes) {
        return trimmed;
    }
    
    // Default to string
    return trimmed;
}

std::any WSM::JsonMin::operator[](const std::string& key) const {
    auto it = data.find(key);
    return (it != data.end()) ? it->second : std::any();
}

std::any WSM::JsonMin::getValue(const std::string& path) const {
    return getNestedValue(path);
}

WSM::JsonType WSM::JsonMin::getType(const std::string& key) const {
    auto it = data.find(key);
    if (it == data.end()) return JsonType::NULL_TYPE;
    if (it->second.type() == typeid(bool)) return JsonType::BOOL;
    if (it->second.type() == typeid(int)) return JsonType::INT;
    if (it->second.type() == typeid(double)) return JsonType::DOUBLE;
    if (it->second.type() == typeid(std::string)) return JsonType::STRING;
    if (it->second.type() == typeid(std::vector<std::any>)) return JsonType::ARRAY;
    if (it->second.type() == typeid(std::map<std::string, std::any>)) return JsonType::OBJECT;
    
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
    auto it = data.find(key);
    if (it == data.end()) return true;
    return !it->second.has_value();
}

std::any WSM::JsonMin::getNestedValue(const std::string& path) const {
    auto parts = splitPath(path);
    if (parts.empty()) return std::any();
    
    const JsonMin* current = this;
    std::any value = (*current)[parts[0]];
    
    for (size_t i = 1; i < parts.size(); ++i) {
        if (value.type() == typeid(std::map<std::string, std::any>)) {
            const auto& map = std::any_cast<const std::map<std::string, std::any>&>(value);
            auto it = map.find(parts[i]);
            if (it == map.end()) return std::any();
            value = it->second;
        } else {
            return std::any();
        }
    }
    
    return value;
}

std::vector<std::string> WSM::JsonMin::splitPath(const std::string& path) const {
    std::vector<std::string> result;
    std::stringstream ss(path);
    std::string item;
    
    while (std::getline(ss, item, '.')) {
        if (!item.empty()) {
            result.push_back(item);
        }
    }
    
    return result;
}

void WSM::JsonMin::traverse(const std::function<void(const std::string& path, const std::any& value, JsonType type)>& callback, 
                      const std::string& prefix) const {
    traverseHelper(data, callback, prefix);
}

void WSM::JsonMin::traverseHelper(const std::map<std::string, std::any>& obj, 
                           const std::function<void(const std::string& path, const std::any& value, JsonType type)>& callback,
                           const std::string& prefix) const {
    for (const auto& [key, value] : obj) {
        std::string path = prefix.empty() ? key : prefix + "." + key;
        
        // Determine type
        JsonType type = JsonType::NULL_TYPE;
        if (value.type() == typeid(bool)) type = JsonType::BOOL;
        else if (value.type() == typeid(int)) type = JsonType::INT;
        else if (value.type() == typeid(double)) type = JsonType::DOUBLE;
        else if (value.type() == typeid(std::string)) type = JsonType::STRING;
        else if (value.type() == typeid(std::vector<std::any>)) type = JsonType::ARRAY;
        else if (value.type() == typeid(std::map<std::string, std::any>)) type = JsonType::OBJECT;
        
        // Call the callback
        callback(path, value, type);
        
        // Recursively traverse objects and arrays
        if (type == JsonType::OBJECT) {
            const auto& map = std::any_cast<const std::map<std::string, std::any>&>(value);
            traverseHelper(map, callback, path);
        } else if (type == JsonType::ARRAY) {
            const auto& arr = std::any_cast<const std::vector<std::any>&>(value);
            for (size_t i = 0; i < arr.size(); ++i) {
                if (arr[i].type() == typeid(std::map<std::string, std::any>)) {
                    const auto& map = std::any_cast<const std::map<std::string, std::any>&>(arr[i]);
                    std::string arrayPath = path + "[" + std::to_string(i) + "]";
                    traverseHelper(map, callback, arrayPath);
                }
            }
        }
    }
}

// New utility methods implementation
int WSM::JsonMin::getSize(const std::string& key) const {
    if (key.empty()) {
        // If key is empty, return the size of the root object
        return data.size();
    }
    
    auto value = getValue(key);
    if (value.type() == typeid(std::vector<std::any>)) {
        return std::any_cast<const std::vector<std::any>&>(value).size();
    } else if (value.type() == typeid(std::map<std::string, std::any>)) {
        return std::any_cast<const std::map<std::string, std::any>&>(value).size();
    }
    
    return -1; // Not a container type
}

bool WSM::JsonMin::empty(const std::string& key) const {
    if (key.empty()) {
        // If key is empty, check if the root object is empty
        return data.empty();
    }
    
    auto value = getValue(key);
    if (value.type() == typeid(std::vector<std::any>)) {
        return std::any_cast<const std::vector<std::any>&>(value).empty();
    } else if (value.type() == typeid(std::map<std::string, std::any>)) {
        return std::any_cast<const std::map<std::string, std::any>&>(value).empty();
    }
    
    return !value.has_value(); // For non-container types, check if it's null
}

bool WSM::JsonMin::hasField(const std::string& key) const {
    if (key.empty()) return false;
    
    auto parts = splitPath(key);
    if (parts.empty()) return false;
    
    auto it = data.find(parts[0]);
    if (it == data.end()) return false;
    
    if (parts.size() == 1) return true;
    
    // For nested fields
    std::any value = it->second;
    for (size_t i = 1; i < parts.size(); ++i) {
        if (value.type() == typeid(std::map<std::string, std::any>)) {
            const auto& map = std::any_cast<const std::map<std::string, std::any>&>(value);
            auto nestedIt = map.find(parts[i]);
            if (nestedIt == map.end()) return false;
            value = nestedIt->second;
        } else {
            return false;
        }
    }
    
    return true;
}

std::vector<std::string> WSM::JsonMin::getFields(const std::string& key) const {
    std::vector<std::string> fields;
    
    if (key.empty()) {
        // If key is empty, return fields of the root object
        for (const auto& [field, _] : data) {
            fields.push_back(field);
        }
        return fields;
    }
    
    auto value = getValue(key);
    if (value.type() == typeid(std::map<std::string, std::any>)) {
        const auto& map = std::any_cast<const std::map<std::string, std::any>&>(value);
        for (const auto& [field, _] : map) {
            fields.push_back(field);
        }
    }
    
    return fields;
}

std::string WSM::JsonMin::getTypeName(JsonType type) const {
    switch (type) {
        case JsonType::BOOL: return "bool";
        case JsonType::INT: return "int";
        case JsonType::DOUBLE: return "double";
        case JsonType::STRING: return "string";
        case JsonType::ARRAY: return "array";
        case JsonType::OBJECT: return "object";
        default: return "null";
    }
}
