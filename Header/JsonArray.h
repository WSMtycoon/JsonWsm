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

#ifndef WSM_JSON_PARSER_ARRAY_H
#define WSM_JSON_PARSER_ARRAY_H

#include <vector>
#include <sstream>
#include <optional>

#include "JsonBlock.h"
#include "JsonValue.h"

namespace WSM {

	class JsonArray: public JsonBlock{
		public:
		
			// Element access
			bool empty() const override { return data.empty(); } ///
			size_t size() const override { return data.size(); } ///

			// Extracting an element
			JsonValue& operator[](size_t index) override; 
			JsonValue& operator[](const std::string& field) override;
			const JsonValue& operator[](size_t index) const override;
			const JsonValue& operator[](const std::string& field) const override;
			
			// Block Type
			JsonType getType() const override { return JsonType::ARRAY; }

			// Element manipulation
			void push_back(const JsonValue& element, std::string field = "") override { data.push_back(std::make_shared<JsonValue>(element));}
			void erase(const std::string& field) override { erase(std::stoi(field)); }
			void erase(const size_t index) override { if (index < data.size()) { data.erase(data.begin() + index); }}
			void clear() override { data.clear(); }

			// Key operations
			bool hasField(const std::string& field) const override { return hasField(std::stoul(field)); };
			bool hasField(const size_t index) const override { return (index >= 0  && index < data.size()) ? true : false; };
			std::vector<std::string> getFields() const override;

			// Return Json
			std::string getJsonString() const override;
			
			// Constructors
			JsonArray() = default;
			JsonArray(std::vector<JsonValue>);
		protected:
			JsonValue& get(size_t index);
			const JsonValue& get(size_t index) const;
			std::vector<std::shared_ptr<JsonValue>> data;
	};

} // namespace WSM

#endif // WSM_JSON_PARSER_ARRAY_H
