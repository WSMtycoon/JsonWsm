# JsonWSM API Documentation

## Overview

JsonWSM is a lightweight JSON parser that provides a simple interface for working with JSON data in C++. It supports UTF-8 encoding and various data types.

## Basic Usage

```cpp
#include "JsonWSM.h"

// Create a JSON object from a string
WSM::JsonMin json("{\"name\": \"John\", \"age\": 30, \"pi\": 3.14f}");

// Access values
std::any name = json["name"];
std::any age = json["age"];
std::any pi = json["pi"];
```

## API Methods

### Constructors

- `JsonMin()` - Default constructor
- `JsonMin(const std::string& jsonStr)` - Constructor that parses JSON from a string

### Value Access

- `std::any operator[](const std::string& key)` - Access value by key
- `std::any getValue(const std::string& path)` - Access nested values using dot notation (e.g., "user.address.city")

### Type Checking

- `JsonType getType(const std::string& key)` - Get the type of a value
- `bool isBool(const std::string& key)` - Check if value is boolean
- `bool isInt(const std::string& key)` - Check if value is integer
- `bool isFloat(const std::string& key)` - Check if value is float
- `bool isDouble(const std::string& key)` - Check if value is double
- `bool isString(const std::string& key)` - Check if value is string
- `bool isArray(const std::string& key)` - Check if value is array
- `bool isObject(const std::string& key)` - Check if value is object
- `bool isNull(const std::string& key)` - Check if value is null

### Data Access

- `const std::map<std::string, std::any>& getData()` - Get the internal data map
- `void traverse(const std::function<void(const std::string& path, const std::any& value, JsonType type)>& callback, const std::string& prefix = "")` - Recursively traverse the JSON structure

### Utility Methods

- `int getSize(const std::string& key = "")` - Get the size of an object or array
- `bool empty(const std::string& key = "")` - Check if an object or array is empty
- `bool hasField(const std::string& key)` - Check if a field exists
- `std::vector<std::string> getFields(const std::string& key = "")` - Get all field names
- `std::string getTypeName(JsonType type)` - Get the string representation of a type

## Supported Data Types

- Boolean (`true`, `false`)
- Integer (e.g., `42`)
- Float (e.g., `3.14f` or `3.14F` with up to 7 decimal places)
- Double (e.g., `3.14` or `3.14159265f` with more than 7 decimal places)
- String (e.g., `"hello"`)
- Array (e.g., `[1, 2, 3]`)
- Object (e.g., `{"key": "value"}`)
- Null

### Float Type Notes

- Float values must end with 'f' or 'F' suffix
- Maximum precision is 7 decimal places
- Values with more than 7 decimal places are automatically treated as double
- Examples:
  - `3.14f` - float
  - `1.234567f` - float
  - `1.2345678f` - double (exceeds 7 decimal places)
  - `3.14` - double (no suffix)

## Example Usage

```cpp
// Parse JSON
WSM::JsonMin json("{\"name\": \"John\", \"age\": 30, \"pi\": 3.14f, \"precise\": 1.2345678f}");

// Check types
if (json.isString("name")) {
    std::string name = std::any_cast<std::string>(json["name"]);
}

if (json.isInt("age")) {
    int age = std::any_cast<int>(json["age"]);
}

if (json.isFloat("pi")) {
    float pi = std::any_cast<float>(json["pi"]);
}

if (json.isDouble("precise")) {
    double precise = std::any_cast<double>(json["precise"]);
}

// Access nested values
WSM::JsonMin nested("{\"user\": {\"name\": \"John\", \"address\": {\"city\": \"New York\"}}}");
std::any city = nested.getValue("user.address.city");

// Traverse all values
nested.traverse([](const std::string& path, const std::any& value, WSM::JsonType type) {
    std::cout << path << ": " << value << " (type: " << type << ")\n";
});
```
