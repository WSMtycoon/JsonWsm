/**************************************************************************/
/**                  Created by WsmTycoon on 21.05.2025                  **/
/**************************************************************************/

/** You are solely responsible for determining the appropriateness of using
 * or redistributing the Work and assume any risks associated with Your
 * exercise of permissions under this License. **/
/** Software distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. **/

#include "JsonValue.h"
#include "JsonObject.h"
#include "JsonArray.h"

namespace WSM {

	bool JsonValue::empty() const { return false; }

	size_t JsonValue::size() const{
		if(isBlock() && block.get() != nullptr){ return block.get()->size();}
		else { return -1; }
	}

	JsonValue& JsonValue::operator[](size_t index){ return get(index);}
	JsonValue& JsonValue::operator[](const std::string &field){ return get(field); }
	const JsonValue& JsonValue::operator[](size_t index) const { return get(index); }
	const JsonValue& JsonValue::operator[](const std::string &field) const { return get(field); }

	JsonValue &JsonValue::get(std::string field){
		if(isBlock()){
			if(isArray()){get(std::stoul(field));}
			if(isObject() && block.get()->hasField(field)){return block.get()->operator[](field); }
		}
		throw std::invalid_argument("Element not found.");
	}

	JsonValue &JsonValue::get(size_t index){
		if(isBlock()){
			if(isObject()){get(std::to_string(index));}
			if(isArray() && block.get()->hasField(index)){return block.get()->operator[](index);}
		}
		throw std::invalid_argument("Element not found.");
	}

	const JsonValue &JsonValue::get(std::string field) const {
		if(isBlock()){
			if(isArray()){get(std::stoul(field));}
			if(isObject() && block.get()->hasField(field)){return block.get()->operator[](field); }
		}
		throw std::invalid_argument("Element not found.");
	}

	const JsonValue &JsonValue::get(size_t index) const {
		if(isBlock()){
			if(isObject()){get(std::to_string(index));}
			if(isArray() && block.get()->hasField(index)){return block.get()->operator[](index);}
		}
		throw std::invalid_argument("Element not found.");
	}

	std::optional<bool> JsonValue::getBool() const {
		if(isBool()){
			if ( auto val = std::get_if<bool>(&data) ) { return std::optional<bool>(*val); }
		}
	    return std::nullopt;
	}
	std::optional<int> JsonValue::getInt() const{
		if(isInt()){
			if ( auto val = std::get_if<int>(&data) ) { return std::optional<int>(*val); }
		}
		return std::nullopt;
	}

	std::optional<float> JsonValue::getFloat() const{
		if(isFloat()){
			if ( auto val = std::get_if<float>(&data) ) { return std::optional<float>(*val); }
		}
		return std::nullopt;
	}
	std::optional<double> JsonValue::getDouble() const{
		if(isDouble()){
			if ( auto val = std::get_if<double>(&data) ) { return std::optional<double>(*val); }
		}
		return std::nullopt;
	}
	std::optional<std::string> JsonValue::getString() const{
		if(isString()){
			if ( auto val = std::get_if<std::string>(&data) ) { return std::optional<std::string>(*val); }
		}
		return std::nullopt;
	}
	std::vector<std::string> JsonValue::getFields() const {
		if (isBlock()) { return block->getFields(); }
		return std::vector<std::string>();
	}
	std::shared_ptr<JsonArray> JsonValue::getArray() const {
		if (isArray()) {
			return std::dynamic_pointer_cast<JsonArray>(block);
		}
		return nullptr;
	}
	std::shared_ptr<JsonObject> JsonValue::getObject() const {
		if (isObject()) {
			return std::dynamic_pointer_cast<JsonObject>(block);
		}
		return nullptr;
	}

	std::string JsonValue::getValueString() const {
		if(isBool()){ return std::get<bool>(data) ? "true" : "false";}
		if(isInt()){ return std::to_string(std::get<int>(data));}
		if(isLong()){ return std::to_string(std::get<long>(data));}
		if(isFloat()){ 
			std::stringstream ss;
			ss << std::setprecision(7) << std::get<float>(data) << "f";
			return ss.str();
		}
		if(isDouble()){
			std::stringstream ss; 
			ss << std::setprecision(15) << std::get<double>(data);
			return ss.str();
		}
		if(isString()) {return std::get<std::string>(data); }
		if(isObject()) {return "Type Object";}
		if(isArray()) {return "Type Array";}
		return "";
	}

	std::string JsonValue::getJsonString() const{
		switch (type) {
			case JsonType::BOOL: return std::get<bool>(data) ? "true" : "false";
			case JsonType::INT: return std::to_string(std::get<int>(data));
			case JsonType::LONG: return std::to_string(std::get<long>(data));
			case JsonType::FLOAT: {
				std::stringstream ss;
				ss << std::setprecision(7) << std::get<float>(data) << "f";
				return ss.str();
			}
			case JsonType::DOUBLE: {  
				std::stringstream ss; 
				ss << std::setprecision(15) << std::get<double>(data);
				return ss.str();
			}
			case JsonType::STRING: return "\"" + std::get<std::string>(data) + "\"";
			case JsonType::ARRAY: { return block->getJsonString(); } 
			case JsonType::OBJECT: { return block->getJsonString(); }
			case JsonType::NULL_TYPE: return "null";
			default: return "";
		}
	}

	void JsonValue::set(const JsonArray &array)	{ type = JsonType::ARRAY; block = std::make_shared<JsonArray>(array); }
	void JsonValue::set(const JsonObject &object){ type = JsonType::OBJECT; block = std::make_shared<JsonObject>(object); }

	JsonValue::JsonValue(const JsonArray &array): type(JsonType::ARRAY) { block = std::make_shared<JsonArray>(array); }
	JsonValue::JsonValue(const JsonObject &object) : type(JsonType::OBJECT) { block = std::make_shared<JsonObject>(object); }

} // namespace WSM
	