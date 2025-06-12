/**************************************************************************/
/**                  Created by WsmTycoon on 18.05.2025                  **/
/**************************************************************************/

/** You are solely responsible for determining the appropriateness of using
 * or redistributing the Work and assume any risks associated with Your
 * exercise of permissions under this License. **/
/** Software distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. **/

#include "JsonUtils.h"

namespace WSM {

std::string JsonUtils::getJsonTypeString(const JsonType& value) {
	switch (value) {
		case JsonType::BOOL: return "Bool";
		case JsonType::INT: return "Int";
		case JsonType::LONG: return "Long";
		case JsonType::FLOAT: return "Float";
		case JsonType::DOUBLE: return "Double";
		case JsonType::STRING: return "String";
		case JsonType::ARRAY: return "Array";
		case JsonType::OBJECT: return "Object";
		case JsonType::EMPTY: return "Empty";
		case JsonType::NULL_TYPE: return "Null";
		default: return "Unknown";
	}
}

bool JsonUtils::compareJsonElements(const JsonValue& a, const JsonValue& b) {
	if (a.getType() != b.getType()) return false;
	
	switch (a.getType()) {
		case JsonType::BOOL: return a.getBool() == b.getBool();
		case JsonType::INT: return a.getInt() == b.getInt();
		case JsonType::FLOAT: return a.getFloat() == b.getFloat();
		case JsonType::DOUBLE: return a.getDouble() == b.getDouble();
		case JsonType::STRING: return a.getString() == b.getString();
		case JsonType::ARRAY: {
			auto arrA = a.getArray();
			auto arrB = b.getArray();
			if (!arrA || !arrB) return false;
			return compareJsonArrays(*arrA, *arrB);
		}
		case JsonType::OBJECT: {
			auto objA = a.getObject();
			auto objB = b.getObject();
			if (!objA || !objB) return false;
			return compareJsonObjects(*objA, *objB);
		}
		case JsonType::NULL_TYPE: return true;
		default: return false;
	}
}

bool JsonUtils::compareJsonObjects(const JsonObject& a, const JsonObject& b) {
	if (a.size() != b.size()) return false;
	
	auto fieldsA = a.getFields();
	auto fieldsB = b.getFields();
	
	if (fieldsA.size() != fieldsB.size()) return false;
	
	for (const auto& field : fieldsA) {
		if (!a.hasField(field) || !b.hasField(field)) return false;
		if (!compareJsonElements(a[field], b[field])) return false;
	}
	
	return true;
}

bool JsonUtils::compareJsonArrays(const JsonArray& a, const JsonArray& b) {
	if (a.size() != b.size()) return false;
	
	for (size_t i = 0; i < a.size(); ++i) {
		if (!a.hasField(i) || !b.hasField(i)) return false;
		if (!compareJsonElements(a[i], b[i])) return false;
	}
	
	return true;
}

bool JsonUtils::isValidJsonString(const std::string& str) {
	if (str.empty()) return false;
	if (str[0] != '"' || str[str.length() - 1] != '"') return false;
	
	bool escaped = false;
	for (size_t i = 1; i < str.length() - 1; ++i) {
		if (escaped) {
			escaped = false;
			continue;
		}
		if (str[i] == '\\') {
			escaped = true;
			continue;
		}
		if (str[i] == '"') return false;
	}
	
	return !escaped;
}

bool JsonUtils::isValidJsonNumber(const std::string& str) {
	if (str.empty()) return false;
	
	// Check for float suffix
	if (str.back() == 'f' || str.back() == 'F') {
		return std::regex_match(str.substr(0, str.length() - 1), 
			std::regex(R"(-?\d+\.\d+([eE][+-]?\d+)?)"));
	}
	
	// Check for integer
	if (std::regex_match(str, std::regex(R"(-?\d+)"))) return true;
	
	// Check for double
	return std::regex_match(str, 
		std::regex(R"(-?\d+\.\d+([eE][+-]?\d+)?)"));
}

bool JsonUtils::isValidJsonBoolean(const std::string& str) {
	std::string lower = str;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	return lower == "true" || lower == "false";
}

bool JsonUtils::isValidJsonNull(const std::string& str) {
	std::string lower = str;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	return lower == "null" || lower == "nullptr";
}

std::vector<std::string> JsonUtils::splitPath(const std::string& path) {
	std::vector<std::string> result;
	std::string current;
	bool inBrackets = false;
	
	for (char c : path) {
		if (c == '[') {
			inBrackets = true;
			if (!current.empty()) {
				result.push_back(current);
				current.clear();
			}
		} else if (c == ']') {
			inBrackets = false;
			if (!current.empty()) {
				result.push_back(current);
				current.clear();
			}
		} else if (c == '.' && !inBrackets) {
			if (!current.empty()) {
				result.push_back(current);
				current.clear();
			}
		} else {
			current += c;
		}
	}
	
	if (!current.empty()) {
		result.push_back(current);
	}
	
	return result;
}

bool JsonUtils::isValidPath(const std::string& path) {
	if (path.empty()) return false;
	
	std::vector<std::string> parts = splitPath(path);
	if (parts.empty()) return false;
	
	for (const auto& part : parts) {
		if (part.empty()) return false;
		if (std::regex_match(part, std::regex(R"(\d+)"))) continue;
		if (!std::regex_match(part, std::regex(R"([a-zA-Z_][a-zA-Z0-9_]*)"))) return false;
	}
	
	return true;
}

void JsonUtils::printJsonElement(const JsonValue& element, int indent) {
	std::string indentStr(indent * 2, ' ');
	std::cout << indentStr << element.getValueString() << std::endl;
}

void JsonUtils::printJsonObject(const JsonObject& object, int indent) {
	std::string indentStr(indent * 2, ' ');
	std::cout << indentStr << "{\n";
	
	for (const auto& field : object.getFields()) {
		std::cout << indentStr << "  \"" << field << "\": ";
		if (object.hasField(field)) {
			const JsonValue& value = object[field];
			if (value.isObject()) {
				std::cout << "\n";
				printJsonObject(*value.getObject(), indent + 2);
			} else if (value.isArray()) {
				std::cout << "\n";
				printJsonArray(*value.getArray(), indent + 2);
			} else {
				std::cout << value.getValueString() << "\n";
			}
		} else {
			std::cout << "null\n";
		}
	}
	
	std::cout << indentStr << "}\n";
}

void JsonUtils::printJsonArray(const JsonArray& array, int indent) {
	std::string indentStr(indent * 2, ' ');
	std::cout << indentStr << "[\n";
	
	for (size_t i = 0; i < array.size(); ++i) {
		if (array.hasField(i)) {
			const JsonValue& value = array[i];
			if (value.isObject()) {
				printJsonObject(*value.getObject(), indent + 2);
			} else if (value.isArray()) {
				printJsonArray(*value.getArray(), indent + 2);
			} else {
				std::cout << indentStr << "  " << value.getValueString() << "\n";
			}
		} else {
			std::cout << indentStr << "  null\n";
		}
	}
	
	std::cout << indentStr << "]\n";
}

void JsonUtils::traverse(const std::function<void(const std::string& path, const JsonValue& value, JsonType type)>& callback,
						const std::string& prefix) {
	// This method is now handled by JsonParser's methods
}

void JsonUtils::traverseHelper(const JsonValue& value,
							  const std::function<void(const std::string& path, const JsonValue& value, JsonType type)>& callback,
							  const std::string& prefix) {
	// This method is now handled by JsonParser's methods
}

} // namespace WSM

