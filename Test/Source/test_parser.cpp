#include "../../Source/JsonWSM.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

void printValue(const std::any& value) {
    if (value.type() == typeid(bool)) {
        std::cout << std::any_cast<bool>(value);
    } else if (value.type() == typeid(int)) {
        std::cout << std::any_cast<int>(value);
    } else if (value.type() == typeid(double)) {
        std::cout << std::any_cast<double>(value);
    } else if (value.type() == typeid(std::string)) {
        std::cout << "\"" << std::any_cast<std::string>(value) << "\"";
    } else if (value.type() == typeid(std::vector<std::any>)) {
        const auto& arr = std::any_cast<const std::vector<std::any>&>(value);
        std::cout << "[";
        for (size_t i = 0; i < arr.size(); ++i) {
            if (i > 0) std::cout << ", ";
            printValue(arr[i]);
        }
        std::cout << "]";
    } else if (value.type() == typeid(std::map<std::string, std::any>)) {
        const auto& obj = std::any_cast<const std::map<std::string, std::any>&>(value);
        std::cout << "{";
        bool first = true;
        for (const auto& [key, val] : obj) {
            if (!first) std::cout << ", ";
            std::cout << "\"" << key << "\": ";
            printValue(val);
            first = false;
        }
        std::cout << "}";
    } else {
        std::cout << "null";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <json_file_path>" << std::endl;
        return 1;
    }
    
    std::string filePath = argv[1];
    
    // Читаем файл в бинарном режиме, чтобы избежать проблем с символами конца строки
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filePath << std::endl;
        return 1;
    }
    
    // Читаем содержимое файла
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonStr = buffer.str();
    
    // Выводим исходный JSON
    std::cout << "Input JSON:\n" << jsonStr << "\n\n";
    
    // Парсим JSON
    WSM::JsonMin json(jsonStr);
    
    // Выводим все поля с помощью traverse
    std::cout << "Parsed JSON structure:\n";
    json.traverse([&json](const std::string& path, const std::any& value, WSM::JsonType type) {
        std::cout << std::setw(30) << std::left << path 
                  << " (" << json.getTypeName(type) << "): ";
        printValue(value);
        std::cout << "\n";
    });
    
    // Выводим дополнительную информацию
    std::cout << "\nAdditional Information:\n";
    std::cout << "Root object size: " << json.getSize() << "\n";
    std::cout << "Root object empty: " << (json.empty() ? "yes" : "no") << "\n";
    
    // Выводим поля корневого объекта
    std::cout << "Root object fields: ";
    auto fields = json.getFields();
    for (size_t i = 0; i < fields.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << fields[i];
    }
    std::cout << "\n";
    
    // Проверяем наличие определенных полей
    std::cout << "Has 'dimensions' field: " << (json.hasField("dimensions") ? "yes" : "no") << "\n";
    std::cout << "Has 'nonexistent' field: " << (json.hasField("nonexistent") ? "yes" : "no") << "\n";
    
    // Проверяем null значения
    std::cout << "Is 'nullValue' null: " << (json.isNull("nullValue") ? "yes" : "no") << "\n";
    
    // Проверяем размеры массивов
    if (json.hasField("tags")) {
        std::cout << "Size of 'tags' array: " << json.getSize("tags") << "\n";
    }
    
    if (json.hasField("items")) {
        std::cout << "Size of 'items' array: " << json.getSize("items") << "\n";
    }
    
    return 0;
} 