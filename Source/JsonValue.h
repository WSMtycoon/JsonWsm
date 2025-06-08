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
			const JsonValue& operator[](size_t index) const;
			const JsonValue& operator[](const std::string& field) const;

			// IsTypes
			inline bool isBool() const {return type == JsonType::BOOL;};
			inline bool isInt() const {return type == JsonType::INT;};
			inline bool isFloat() const {return type == JsonType::FLOAT;};
			inline bool isDouble() const {return type == JsonType::DOUBLE;};
			inline bool isDecimal() const {return isFloat() || isDouble();};
			inline bool isString() const {return type == JsonType::STRING;}; 
			inline bool isVariant() const{return isBool() || isInt() || isDecimal() || isString();};

			inline bool isArray() const {return (type == JsonType::ARRAY && block != nullptr) ? true : false;};
			inline bool isObject() const {return (type == JsonType::OBJECT && block != nullptr) ? true : false;};

			inline bool isEmpty() const {return type == JsonType::EMPTY;};
			inline bool isNull() const {return type == JsonType::NULL_TYPE;};

			bool isBlock() const{return (isObject() || isArray) ? true : false; };

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
			std::vector<std::string> getFields() const;
			
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
			const JsonValue& get(std::string field) const;
			const JsonValue& get(size_t index) const;

			VariantType data;
			std::shared_ptr<JsonBlock> block;
	};
}

#endif // WSM_JSON_PARSER_VALUE_H
