# JsonWSM - Simple JSON Parser Cpp

A lightweight JSON parser that supports UTF-8 encoding and provides easy access to JSON data through a simple interface.

## Features

- UTF-8 encoding support
- Flexible value parsing (supports both `:` and `;` as separators)
- Automatic type detection for values
- Nested object access using dot notation
- Support for multiple data types:
  - Null
  - Empty
  - Boolean
  - Integer
  - Long
  - Float
  - Double
  - String
  - Arrays
  - Objects

## Building
#include "Module\JsonParser.h"

## Usage

### Работа с вложенными структурами
```cpp
JsonParser parser(R"({
    "user": {
        "profile": {
            "name": "John",
            "age": 30
        },
        "items": [
            {"id": 1, "price": 19.99f},
            {"id": 2, "price": 29.99f}
        ]
    }
})");

if (parser.isCorrectlyParsed()) {
    
	std::string name = "";
	int age = 0;
	float price = 0.;
	
	if(parser.hasField("user")){
		if(parser["user"].hasField("profile")){
			if(parser["user"]["profile"].hasField("name"))
				{name = parser["user"]["profile"]["name"].getString();}
			if(parser["user"]["profile"].hasField("age"))
				{age = parser["user"]["profile"]["age"].getInt();}
			if(parser["user"]["profile"].hasField("price"))
				{price = parser["user"]["items"][0]["price"].getFloat();;}
		}
	}

	if (parser["user"]["items"].isArray()) {
		// Обработка массива
	}
}
```

## Requirements

- C++17 compatible compiler
- Make

## License

This project is open source and available under the MIT License.