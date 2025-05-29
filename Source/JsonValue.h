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

#ifndef WSM_JSON_PARSER_VALUE_H
#define WSM_JSON_PARSER_VALUE_H

#include <string>
#include <variant>
#include <sstream>
#include <iomanip>
#include <optional>
#include <memory>

#include "JsonArray.h"
#include "JsonObject.h"

namespace WSM {

	enum class JsonType {BOOL, INT, FLOAT, DOUBLE, STRING, ARRAY, OBJECT, EMPTY, NULL_TYPE};
	using VariantType = std::variant<bool, int, float, double, std::string>;

	struct JsonData {
		VariantType primitive;
		std::shared_ptr<JsonBlock> block = nullptr;
	};

	class JsonValue {
		public:

			bool empty() const;
			size_t size() const;

			// IsTypes
			bool isBool() const {return type == JsonType::BOOL;};
			bool isNumeric() const {return type == JsonType::INT;};
			bool isFloat() const {return type == JsonType::FLOAT;};
			bool isDouble() const {return type == JsonType::DOUBLE;};
			bool isString() const {return type == JsonType::STRING;}; 
			bool isArray() const {return type == JsonType::ARRAY;};
			bool isObject() const {return type == JsonType::OBJECT;};
			bool isEmpty() const {return type == JsonType::EMPTY;};
			bool isNull() const {return type == JsonType::NULL_TYPE;};

			bool isBlock() const{return type == JsonType::OBJECT || type == JsonType::ARRAY; };
			bool isVariant() const{return type == JsonType::BOOL || type == JsonType::INT || type == JsonType::FLOAT || type == JsonType::DOUBLE || type == JsonType::STRING;};

			// Getters
			JsonType getType() const {return type;};

			std::optional<bool> JsonValue::getBool() const;
			std::optional<int> JsonValue::getInt() const; 
			std::optional<float> JsonValue::getFloat() const;
			std::optional<double> JsonValue::getDouble() const;
			std::optional<std::string> JsonValue::getString() const;
			std::optional<const VariantType&> getVariant() const;
			std::shared_ptr<JsonBlock> getBlock() const { return data.block; }

			std::string getValueString() const;
			
			// Setters
			void set(bool value) { type = JsonType::BOOL; data.primitive = value; }
			void set(int value) { type = JsonType::INT; data.primitive = value; }
			void set(float value) { type = JsonType::FLOAT; data.primitive = value; }
			void set(double value) { type = JsonType::DOUBLE; data.primitive = value; }
			void set(const std::string& value) { type = JsonType::STRING; data.primitive = value; }
			void set(const JsonArray& block) { type = block.getType(); data.block = std::make_shared<JsonArray>(block); }
			void set(const JsonObject& block) { type = block.getType(); data.block = std::make_shared<JsonObject>(block); }
			void setNull() { type = JsonType::NULL_TYPE; }

			// Constructors
			JsonValue() : type(JsonType::NULL_TYPE) {}
			JsonValue(bool value) : type(JsonType::BOOL) { data.primitive = value; }
			JsonValue(int value) : type(JsonType::INT) { data.primitive = value; }
			JsonValue(float value) : type(JsonType::FLOAT) { data.primitive = value; }
			JsonValue(double value) : type(JsonType::DOUBLE) { data.primitive = value; }
			JsonValue(const std::string& value) : type(JsonType::STRING) { data.primitive = value; }
			JsonValue(const JsonBlock& value) : type(value.getType()) { data.block = std::make_shared<JsonBlock>(value); }

		protected:
			JsonType type = JsonType::NULL_TYPE;
		private:
			JsonData data;
	};
}

#endif // WSM_JSON_PARSER_VALUE_H
