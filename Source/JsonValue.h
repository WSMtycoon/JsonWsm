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

#include "JsonBlock.h"

namespace WSM {

	class JsonArray;
	class JsonObject;

	using VariantType = std::variant<bool, int, float, double, std::string>;

	struct JsonData {
		VariantType primitive;
		std::shared_ptr<JsonBlock> block = nullptr;
	};

	class JsonValue {
		public:

			bool empty() const;
			size_t size() const;

			JsonValue& operator[](size_t index);
			JsonValue& operator[](const std::string& field);

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

			std::optional<bool> getBool() const;
			std::optional<int> getInt() const;
			std::optional<float> getFloat() const;
			std::optional<double> getDouble() const;
			std::optional<std::string> getString() const;
			std::shared_ptr<JsonBlock> getBlock() const { return block; }
			std::shared_ptr<JsonArray> getArray() const;
			std::shared_ptr<JsonObject> getObject() const;

			std::string getValueString() const;
			
			// Setters
			void set() { type = JsonType::EMPTY; }
			void set(bool value) { type = JsonType::BOOL; data = value; }
			void set(int value) { type = JsonType::INT; data = value; }
			void set(float value) { type = JsonType::FLOAT; data = value; }
			void set(double value) { type = JsonType::DOUBLE; data = value; }
			void set(const std::string& value) { type = JsonType::STRING; data = value; }
			void set(const JsonArray& array);
			void set(const JsonObject& object);
			void setNull() { type = JsonType::NULL_TYPE; }

			// Constructors
			JsonValue() : type(JsonType::NULL_TYPE) {}
			JsonValue(JsonType set) : type(set) {}
			JsonValue(bool value) : type(JsonType::BOOL) { data = value; }
			JsonValue(int value) : type(JsonType::INT) { data = value; }
			JsonValue(float value) : type(JsonType::FLOAT) { data = value; }
			JsonValue(double value) : type(JsonType::DOUBLE) { data = value; }
			JsonValue(const std::string& value) : type(JsonType::STRING) { data = value; }
			JsonValue(const JsonArray& array);
			JsonValue(const JsonObject& object);

		protected:
			
			JsonType type = JsonType::NULL_TYPE;
		
		private:
			
			JsonValue& get(std::string field);
			JsonValue& get(size_t index);

			VariantType data;
			std::shared_ptr<JsonBlock> block;
	};
}

#endif // WSM_JSON_PARSER_VALUE_H
