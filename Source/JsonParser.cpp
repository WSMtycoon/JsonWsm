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
	
	#include <regex>
	#include <sstream>
	#include <iostream>
	#include <limits>
	#include <algorithm>
	
	#include "JsonParser.h"

namespace WSM {

JsonParser::JsonParser(const std::string& jsonStr):JsonObject() {
	parseJson(jsonStr);
}

void JsonParser::parseJson(const std::string& jsonStr) {

	correctlyParsed = false;
	
	// Trim whitespace
	std::string trimmed = jsonStr;
	trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r\f\v"));
	trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);
	
	if (trimmed.empty()) { correctlyParsed = false; return;}

	// Parse the root value
	JsonValue value = parseValue(trimmed);
	if (!value.isObject() && !value.isArray()) { correctlyParsed = false; return; }
	else{correctlyParsed = true;}

	// If root is an object, copy its contents to this object
	if (value.isObject()) {
		auto obj = value.getObject();
		if (obj) { 
			for (const auto& field : obj->getFields()) { JsonObject::push_back(obj->operator[](field), field); } 
		 }
	}
	// If root is an array, create an object with a single array field
	else if (value.isArray()) {	JsonObject::push_back(value, "root"); }
}

JsonValue JsonParser::parseValue(const std::string& value) {

	std::string trimmed = value;
	trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r\f\v"));
	trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);

	// Check for string (with or without quotes)
	if (trimmed[0] == '"' && trimmed[trimmed.length() - 1] == '"') {
		trimmed = trimmed.substr(1, trimmed.length() - 2);
	}

	if (trimmed.empty()) { return JsonValue(JsonType::EMPTY); } // Empty value

	// Check for boolean (case insensitive)
	std::string lowerTrimmed = trimmed;
	std::transform(lowerTrimmed.begin(), lowerTrimmed.end(), lowerTrimmed.begin(), ::tolower);
	// Handle Null
	if (lowerTrimmed == "null" || lowerTrimmed == "nullptr") { return JsonValue(JsonType::NULL_TYPE);  } // Null value
	// Handle boolean
	if (lowerTrimmed == "true" || lowerTrimmed == "false") return JsonValue(lowerTrimmed == "true");
	// Handle string
	if (trimmed[0] == '"' && trimmed[trimmed.length() - 1] == '"') {
		return JsonValue(trimmed.substr(1, trimmed.length() - 2));
	}

	// Handle array
	if (trimmed[0] == '[' && trimmed[trimmed.length() - 1] == ']') {
		return JsonValue(JsonArray(parseArray(trimmed)));
	}
	// Handle object
	if (trimmed[0] == '{' && trimmed[trimmed.length() - 1] == '}') {
		JsonObject obj;
		auto pairs = splitJsonObject(trimmed);
		for (const auto& pair : pairs) {
			if (!pair.first.empty() && !pair.second.empty()) {
				obj.push_back(parseValue(pair.second), pair.first);
			}
		}
		return JsonValue(obj);
	}

	// Handle numbers
	try {
		// Try parsing as integer first
		if (std::regex_match(trimmed, std::regex(R"(-?\d+)"))) {
			long long val = std::stoll(trimmed);
			if (val <= std::numeric_limits<int>::max() && val >= std::numeric_limits<int>::min()) {
				return JsonValue(static_cast<int>(val));
			}
		}

		// Try parsing as float
		if (std::regex_match(trimmed, std::regex(R"(-?\d+\.\d+)")) || 
			std::regex_match(trimmed, std::regex(R"(-?\d+[eE][+-]?\d+)")) ||
			std::regex_match(trimmed, std::regex(R"(-?\d+\.\d+[eE][+-]?\d+)"))) {
			
			// Check if it's a float (has 'f' suffix)
			if (trimmed.back() == 'f' || trimmed.back() == 'F') {
				return JsonValue(std::stof(trimmed.substr(0, trimmed.length() - 1)));
			}
			// Otherwise treat as double
			return JsonValue(std::stod(trimmed));
		}
	} catch (const std::exception&) {
		// If number parsing fails, treat as string
		return JsonValue(trimmed);
	}

	// If all else fails, treat as string
	return JsonValue(trimmed);
}

std::vector<JsonValue> JsonParser::parseArray(const std::string& value) {

	std::vector<JsonValue> arr;
	std::string inner = value.substr(1, value.length() - 2);
	
	if (inner.empty()) { return arr; }

	size_t pos = 0;
	while (pos < inner.length()) {
		// Skip whitespace
		while (pos < inner.length() && std::isspace(inner[pos])) ++pos;
		if (pos >= inner.length()) break;

		// Extract array element
		size_t start = pos;
		bool inString = false;
		bool escaped = false;
		int nestLevel = 0;

		while (pos < inner.length()) {
			if (escaped) { escaped = false; ++pos; continue; }
			if (inner[pos] == '\\') { escaped = true; ++pos; continue; }
			if (inner[pos] == '"' && !escaped) { inString = !inString; ++pos; continue; }
			if (!inString) {
				if (inner[pos] == '[' || inner[pos] == '{') ++nestLevel;
				else if (inner[pos] == ']' || inner[pos] == '}') --nestLevel;
				else if (inner[pos] == ',' && nestLevel == 0) break;
			}
			++pos;
		}

		std::string element = inner.substr(start, pos - start);
		if (!element.empty()) { arr.push_back(parseValue(element)); }

		// Skip comma and whitespace
		while (pos < inner.length() && (std::isspace(inner[pos]) || inner[pos] == ',')) ++pos;
	}

	return arr;
}

std::vector<std::pair<std::string, std::string>> JsonParser::splitJsonObject(const std::string& jsonStr) {
	
	std::vector<std::pair<std::string, std::string>> pairs;
	size_t pos = 0;

	// Skip opening brace
	while (pos < jsonStr.length() && jsonStr[pos] != '{') { ++pos; }
	if (pos < jsonStr.length()) ++pos; // Skip the opening brace

	while (pos < jsonStr.length()) {
		// Skip whitespace
		while (pos < jsonStr.length() && std::isspace(jsonStr[pos])) { ++pos; }
		if (pos >= jsonStr.length()) { break; }
		
		// Check for closing brace
		if (jsonStr[pos] == '}') { break; }
		
		// Extract key
		std::string key = extractJsonKey(jsonStr, pos);
		if (key.empty()) { break; }
		
		// Skip whitespace and colon
		while (pos < jsonStr.length() && (std::isspace(jsonStr[pos]) || jsonStr[pos] == ':')) { ++pos; }
		if (pos >= jsonStr.length()) { break; } 
		
		// Extract value
		std::string value = extractJsonValue(jsonStr, pos);
		if (!value.empty()) { pairs.emplace_back(key, value); }
		
		// Skip comma and whitespace
		while (pos < jsonStr.length() && (std::isspace(jsonStr[pos]) || jsonStr[pos] == ',')) { ++pos; }
	}
	return pairs;
}

std::string JsonParser::extractJsonKey(const std::string& jsonStr, size_t& pos) {
	if (pos >= jsonStr.length() || jsonStr[pos] != '"') { return ""; }
	
	++pos; // Skip opening quote
	size_t start = pos;
	bool escaped = false;
	
	while (pos < jsonStr.length()) {
		if (escaped) { escaped = false; ++pos; continue; }
		if (jsonStr[pos] == '\\') { escaped = true; ++pos; continue; }
		if (jsonStr[pos] == '"') { size_t length = pos - start; ++pos; return jsonStr.substr(start, length); }
		++pos;
	}
	return "";
}

std::string JsonParser::extractJsonValue(const std::string& jsonStr, size_t& pos) {
	while (pos < jsonStr.length() && std::isspace(jsonStr[pos])) ++pos;
	if (pos >= jsonStr.length()) return "";

	size_t start = pos;
	bool inString = false;
	bool escaped = false;
	int nestLevel = 0;

	// Handle string values
	if (jsonStr[pos] == '"') {
		inString = true;
		++pos;
		while (pos < jsonStr.length()) {
			if (escaped) { escaped = false; ++pos; continue; }
			if (jsonStr[pos] == '\\') { escaped = true; ++pos; continue; }
			if (jsonStr[pos] == '"' && !escaped) { ++pos; return jsonStr.substr(start, pos - start); }
			++pos;
		}
		return jsonStr.substr(start, pos - start);
	}

	// Handle objects and arrays
	if (jsonStr[pos] == '{' || jsonStr[pos] == '[') {
		char openChar = jsonStr[pos];
		char closeChar = (openChar == '{') ? '}' : ']';
		++pos;
		nestLevel = 1;

		while (pos < jsonStr.length() && nestLevel > 0) {
			if (escaped) { escaped = false; ++pos; continue; }
			if (jsonStr[pos] == '\\') { escaped = true; ++pos; continue; }
			if (jsonStr[pos] == '"' && !escaped) { inString = !inString; ++pos; continue; }
			if (!inString) { 
				if (jsonStr[pos] == openChar) ++nestLevel;
				else if (jsonStr[pos] == closeChar) --nestLevel;
			}
			++pos;
		}
		return jsonStr.substr(start, pos - start);
	}

	// Handle simple values
	while (pos < jsonStr.length()) {
		if (jsonStr[pos] == ',' || jsonStr[pos] == '}' || jsonStr[pos] == ']'){ break; }
		++pos;
	}
	return jsonStr.substr(start, pos - start);
}

JsonType JsonParser::getType(const std::string& key) const {
	auto value = getValue(key);
	return value ? value->getType() : JsonType::EMPTY;
}

bool JsonParser::isEmpty(const std::string &key) const{
	if(data.find(key) != data.end()){ return data.at(key)->isEmpty(); }
	return false;
}

bool JsonParser::isBool(const std::string &key) const{
	if(data.find(key) != data.end()){ return data.at(key)->isBool(); }
	return false;
}

bool JsonParser::isNumeric(const std::string &key) const{
	if(data.find(key) != data.end()){ return data.at(key)->isNumeric(); }
	return false;
}

bool JsonParser::isFloat(const std::string &key) const{
	if(data.find(key) != data.end()){ return data.at(key)->isFloat(); }
	return false;
}

bool JsonParser::isDouble(const std::string &key) const{
	if(data.find(key) != data.end()){ return data.at(key)->isDouble(); }
	return false;
}

bool JsonParser::isString(const std::string &key) const{
	if(data.find(key) != data.end()){ return data.at(key)->isString(); }
	return false;
}

bool JsonParser::isArray(const std::string &key) const{
	if(data.find(key) != data.end()){ return data.at(key)->isArray(); }
	return false;
}

bool JsonParser::isObject(const std::string &key) const{
	if(data.find(key) != data.end()){ return data.at(key)->isObject(); }
	return false;
}

bool JsonParser::isNull(const std::string &key) const{
	if(data.find(key) != data.end()){ return data.at(key)->isNull(); }
	return false;
}

std::optional<JsonValue> JsonParser::getValue(const std::string& key) const {
	auto it = data.find(key);
	if (it != data.end()) {
		return *it->second;
	}
	return std::nullopt;
}

std::optional<bool> JsonParser::getBool(const std::string& key) const {
	auto value = getValue(key);
	return value ? value->getBool() : std::nullopt;
}

std::optional<int> JsonParser::getInt(const std::string& key) const {
	auto value = getValue(key);
	return value ? value->getInt() : std::nullopt;
}

std::optional<float> JsonParser::getFloat(const std::string& key) const {
	auto value = getValue(key);
	return value ? value->getFloat() : std::nullopt;
}

std::optional<double> JsonParser::getDouble(const std::string& key) const {
	auto value = getValue(key);
	return value ? value->getDouble() : std::nullopt;
}

std::optional<std::string> JsonParser::getString(const std::string& key) const {
	auto value = getValue(key);
	return value ? value->getString() : std::nullopt;
}

std::shared_ptr<JsonArray> JsonParser::getArray(const std::string& key) const {
	auto value = getValue(key);
	if (!value || !value->isArray()) return nullptr;
	return value->getArray();
}

std::shared_ptr<JsonObject> JsonParser::getObject(const std::string& key) const {
	auto value = getValue(key);
	if (!value || !value->isObject()) return nullptr;
	return value->getObject();
}

std::vector<std::string> JsonParser::getFields(const std::string& key) const {
	if (key.empty()) { return JsonObject::getFields(); }

	auto value = getValue(key);
	if (!value || !value->isObject()) return {};
	auto obj = value->getObject();
	return obj ? obj->getFields() : std::vector<std::string>();
}


} // namespace WSM

