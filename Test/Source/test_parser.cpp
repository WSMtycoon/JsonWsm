/**************************************************************************/
/**                  Created by WsmTycoon on 29.04.2025                  **/
/**************************************************************************/

/** You are solely responsible for determining the appropriateness of using
 * or redistributing the Work and assume any risks associated with Your
 * exercise of permissions under this License. **/
/** Software distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. **/
	

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "../../Utils/Source/JsonUtils.h"

void printValue(const WSM::JsonValue& value) {
    switch (value.getType()) {
        case WSM::JsonType::BOOL:
            std::cout << (value.getBool().value() ? "true" : "false");
            break;
        case WSM::JsonType::INT:
            std::cout << value.getInt().value();
            break;
        case WSM::JsonType::FLOAT:
            std::cout << value.getFloat().value() << "f";
            break;
        case WSM::JsonType::DOUBLE:
            std::cout << value.getDouble().value();
            break;
        case WSM::JsonType::STRING:
            std::cout << "\"" << value.getString().value() << "\"";
            break;
        case WSM::JsonType::ARRAY: {
            auto arr = value.getArray();
            if (arr) {
                std::cout << "[";
                for (size_t i = 0; i < arr->size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    printValue(arr->operator[](i));
                }
                std::cout << "]";
            }
            break;
        }
        case WSM::JsonType::OBJECT: {
            auto obj = value.getObject();
            if (obj) {
                std::cout << "{";
                bool first = true;
                for (const auto& field : obj->getFields()) {
                    if (!first) std::cout << ", ";
                    std::cout << "\"" << field << "\": ";
                    printValue(obj->operator[](field));
                    first = false;
                }
                std::cout << "}";
            }
            break;
        }
        default:
            std::cout << "null";
    }
}

void printJsonStructure(const WSM::JsonValue& value, const std::string& path = "", size_t indent = 0) {
    std::string indentStr;
    for (size_t i = 0; i < indent; ++i) {
        indentStr += "|   ";
    }
    indentStr += "|---";

    switch (value.getType()) {
        case WSM::JsonType::BOOL:
            std::cout << indentStr << path << " (" << WSM::JsonUtils::getJsonTypeString(value.getType()) << "): " 
                      << (value.getBool().value() ? "true" : "false") << std::endl;
            break;
        case WSM::JsonType::INT:
            std::cout << indentStr << path << " (" << WSM::JsonUtils::getJsonTypeString(value.getType()) << "): " 
                      << value.getInt().value() << std::endl;
            break;
        case WSM::JsonType::FLOAT:
            std::cout << indentStr << path << " (" << WSM::JsonUtils::getJsonTypeString(value.getType()) << "): " 
                      << value.getFloat().value() << "f" << std::endl;
            break;
        case WSM::JsonType::DOUBLE:
            std::cout << indentStr << path << " (" << WSM::JsonUtils::getJsonTypeString(value.getType()) << "): " 
                      << value.getDouble().value() << std::endl;
            break;
        case WSM::JsonType::STRING:
            std::cout << indentStr << path << " (" << WSM::JsonUtils::getJsonTypeString(value.getType()) << "): " 
                      << "\"" << value.getString().value() << "\"" << std::endl;
            break;
        case WSM::JsonType::ARRAY: {
            auto arr = value.getArray();
            if (arr) {
                std::cout << indentStr << path << " (" << WSM::JsonUtils::getJsonTypeString(value.getType()) << "): [" << std::endl;
                for (size_t i = 0; i < arr->size(); ++i) {
                    printJsonStructure(arr->operator[](i), path + "[" + std::to_string(i) + "]", indent + 1);
                }
                std::cout << indentStr << "]" << std::endl;
            }
            break;
        }
        case WSM::JsonType::OBJECT: {
            auto obj = value.getObject();
            if (obj) {
                std::cout << indentStr << path << " (" << WSM::JsonUtils::getJsonTypeString(value.getType()) << "): {" << std::endl;
                for (const auto& field : obj->getFields()) {
                    std::string newPath = path.empty() ? field : path + "." + field;
                    printJsonStructure(obj->operator[](field), newPath, indent + 1);
                }
                std::cout << indentStr << "}" << std::endl;
            }
            break;
        }
        case WSM::JsonType::EMPTY:
            std::cout << indentStr << path << " (" << WSM::JsonUtils::getJsonTypeString(value.getType()) << "): " << std::endl;
            break;
        default:
            std::cout << indentStr << path << " (" << WSM::JsonUtils::getJsonTypeString(value.getType()) << "): null" << std::endl;
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
    WSM::JsonParser parser(jsonStr);
    
    if (!parser.isCorrectlyParsed()) {
        std::cout << "Error: Failed to parse JSON" << std::endl;
        return 1;
    }
    
    // Выводим структуру JSON с путями
    std::cout << "Parsed JSON structure:\n";
    printJsonStructure(parser);
    
    // Выводим дополнительную информацию
    std::cout << "\nAdditional Information:\n";
    std::cout << "Root object size: " << parser.size() << "\n";
    std::cout << "Root object empty: " << (parser.empty() ? "yes" : "no") << "\n";
    
    // Выводим поля корневого объекта
    std::cout << "Root object fields: ";
    auto fields = parser.getFields();
    for (size_t i = 0; i < fields.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << fields[i];
    }
    std::cout << "\n";
    
    // Проверяем наличие определенных полей
    std::cout << "Parsed correctly: " << (parser.isCorrectlyParsed() ? "yes" : "no") << "\n";
    
    return 0;
} 