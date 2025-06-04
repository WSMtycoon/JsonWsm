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
		case JsonType::BOOL: return "bool";
		case JsonType::INT: return "int";
		case JsonType::FLOAT: return "float";
		case JsonType::DOUBLE: return "double";
		case JsonType::STRING: return "string";
		case JsonType::ARRAY: return "array";
		case JsonType::OBJECT: return "object";
		case JsonType::EMPTY: return "empty";
		default: return "null";
	}
}

bool JsonUtils::compareJsonElements(const JsonValue& a, const JsonValue& b) {
	if (a.getType() != b.getType()) return false;
	
	switch (a.getType()) {
		case JsonType::BOOL: return a.getBool() == b.getBool();
		case JsonType::INT: return a.getInt() == b.getInt();
		case JsonType::DOUBLE: return a.getDouble() == b.getDouble();
		case JsonType::STRING: return a.getString() == b.getString();
		case JsonType::NULL_TYPE: return true;
		default: return false;
	}
}


bool JsonUtils::isValidJsonString(const std::string& str) {
	if (str.empty()) return false;
	if (str[0] != '"' || str[str.length() - 1] != '"') return false;
	
	// Check for unescaped quotes
	for (size_t i = 1; i < str.length() - 1; ++i) {
		if (str[i] == '"' && str[i - 1] != '\\') return false;
	}
	return true;
}

bool JsonUtils::isValidJsonNumber(const std::string& str) {
	static const std::regex numberRegex("^-?(0|[1-9]\\d*)(\\.\\d+)?([eE][+-]?\\d+)?$");
	return std::regex_match(str, numberRegex);
}

bool JsonUtils::isValidJsonBoolean(const std::string& str) {
	return str == "true" || str == "false";
}

bool JsonUtils::isValidJsonNull(const std::string& str) {
	return str == "null";
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
	for (const auto& part : parts) {
		if (part.empty()) return false;
		if (part[0] == '[' && part[part.length() - 1] == ']') {
			std::string index = part.substr(1, part.length() - 2);
			if (!isValidJsonNumber(index)) return false;
		}
	}
	return true;
}

void JsonUtils::printJsonElement(const JsonValue& element, int indent) {
	std::string indentStr(indent * 2, ' ');
	std::cout << indentStr << element.getValueString() << std::endl;
}

void JsonUtils::traverse(const std::function<void(const std::string &path, const JsonValue &value, JsonType type)> &callback, const std::string &prefix){
	// traverseHelper(root, callback, prefix);
}

void JsonUtils::traverseHelper(const JsonValue& value,
							  const std::function<void(const std::string& path, const JsonValue& value, JsonType type)>& callback,
							  const std::string& prefix) {
	callback(prefix, value, value.getType());

	if (value.isObject()) {
		auto obj = value.getBlock();
		for (const auto& field : obj->getFields()) {
			if (auto fieldValue = obj->operator[](field)) {
				std::string newPath = prefix.empty() ? field : prefix + "." + field;
				traverseHelper(*fieldValue, callback, newPath);
			}
		}
	} else if (value.isArray()) {
		auto arr = value.getBlock();
		for (size_t i = 0; i < arr->size(); ++i) {
			std::string newPath = prefix.empty() ? std::to_string(i) : prefix + "[" + std::to_string(i) + "]";
			traverseHelper(arr->operator[](i).value(), callback, newPath);
		}
	}
}



} // namespace WSM

