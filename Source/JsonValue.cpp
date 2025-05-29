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


namespace WSM {

	class JsonArray;
	class JsonObject;

	bool JsonValue::empty() const { return false; }

	size_t JsonValue::size() const
	{
		if(isBlock() && data.block != nullptr){ return data.block.get()->size();}
		else { return -1; }
	}

	std::optional<const VariantType&> JsonValue::getVariant() const{
		if (isVariant()) { return data.primitive; }
		return std::nullopt;
	}

	std::optional<bool> JsonValue::getBool() const {
		if(type == JsonType::BOOL && std::holds_alternative<bool>(data.primitive)) {return std::get<bool>(data.primitive);}
		return std::nullopt;
	}
	std::optional<int> JsonValue::getInt() const{
		if(type == JsonType::INT && std::holds_alternative<int>(data.primitive)){ return std::get<int>(data.primitive); }
		return std::nullopt;
	}

	std::optional<float> JsonValue::getFloat() const{
		if(type == JsonType::FLOAT && std::holds_alternative<float>(data.primitive)){return std::get<float>(data.primitive);}
		return std::nullopt;
	}

	std::optional<double> JsonValue::getDouble() const{
		if(type == JsonType::DOUBLE && std::holds_alternative<double>(data.primitive)){return std::get<double>(data.primitive);}
		return std::nullopt;
	}

	std::optional<std::string> JsonValue::getString() const{
		if(type == JsonType::STRING && std::holds_alternative<std::string>(data.primitive)){return std::get<std::string>(data.primitive);}
		return std::nullopt;
	}


	std::string JsonValue::getValueString() const {
		switch (type) {
			case JsonType::BOOL: return std::get<bool>(data.primitive) ? "true" : "false";
			case JsonType::INT: return std::to_string(std::get<int>(data.primitive));
			case JsonType::DOUBLE: {  
				std::stringstream ss; 
				ss << std::fixed << std::setprecision(12) << std::get<double>(data.primitive);
				return ss.str();
			}
			case JsonType::STRING: return "\"" + std::get<std::string>(data.primitive) + "\"";
			case JsonType::NULL_TYPE: return "null";
			default: return "";
		}
	}

} // namespace WSM
	