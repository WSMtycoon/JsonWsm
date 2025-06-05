/**************************************************************************/
/**                  Created by WsmTycoon on 23.05.2025                  **/
/**************************************************************************/

/** You are solely responsible for determining the appropriateness of using
 * or redistributing the Work and assume any risks associated with Your
 * exercise of permissions under this License. **/
/** Software distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. **/

#ifndef WSM_JSON_PARSER_OBJECT_H
#define WSM_JSON_PARSER_OBJECT_H

class JsonValue;

#include <map>
#include <vector>
#include "JsonBlock.h"
#include "JsonValue.h"

namespace WSM {
	class JsonObject: public JsonBlock{
		public:
			
			// Element access
			bool empty() const override { return data.empty(); }
			size_t size() const override { return data.size(); }

			// Extracting an element
			JsonValue& operator[](size_t index) override;
			JsonValue& operator[](const std::string& field) override;
			const JsonValue& operator[](size_t index) const override;
			const JsonValue& operator[](const std::string& field) const override;

			// Block Typt
			JsonType getType() const override{ return JsonType::OBJECT; }
		
			// Element manipulation
			void push_back( const JsonValue& element, std::string field ) override;
			void erase(const size_t index) override {erase(std::to_string(index));};
			void erase(const std::string& field) override;
			void clear() override { data.clear(); }
			
			// Key operations
			bool hasField(const std::string& field) const override{ return data.find(field) != data.end(); }
			bool hasField(const size_t index) const override {return hasField(std::to_string(index));};
			std::vector<std::string> getFields() const override;

			// Return Json 
			std::string getValueString() const override;

			// Constructors
			JsonObject() = default;

		protected:
			JsonValue& get(std::string field);
			const JsonValue& get(std::string field) const;
			std::map<std::string, std::shared_ptr<JsonValue>> data;
	};
} // namespace WSM

#endif // WSM_JSON_PARSER_OBJECT_H
