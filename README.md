# JsonWSM - Simple JSON Parser

A lightweight JSON parser that supports UTF-8 encoding and provides easy access to JSON data through a simple interface.

## Features

- UTF-8 encoding support
- Flexible value parsing (supports both `:` and `;` as separators)
- Automatic type detection for values
- Nested object access using dot notation
- Support for multiple data types:
  - Boolean
  - Integer
  - Double
  - String
  - Arrays
  - Nested Objects

## Usage

```cpp
#include "JsonWSM.h"

// Create a JSON object from a string
JsonWSM json("{\"name\": \"John\", \"age\": 30}");

// Access values
std::any name = json["name"];
std::any age = json["age"];

## Building

The project uses Make for building. To build and run the tests:

# Build the project
make clean
make all

## Requirements

- C++17 compatible compiler
- Make
- PowerShell (for build script)

## License

This project is open source and available under the MIT License.