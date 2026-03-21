/**************************************************************************/
/**                  Created by WsmTycoon on 24.05.2025                  **/
/**************************************************************************/

/** You are solely responsible for determining the appropriateness of using
 * or redistributing the Work and assume any risks associated with Your
 * exercise of permissions under this License. **/
/** Software distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. **/

#ifndef WSM_JSON_PARSER_BLOCK_H
#define WSM_JSON_PARSER_BLOCK_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <optional>

#include "JsonType.h"

namespace WSM {

	class JsonValue;

	class JsonBlock {
		public:
			// Element access
			virtual bool empty() const = 0;
			virtual size_t size() const = 0;

			// Extracting an element
			virtual JsonValue& operator[](size_t index) = 0;
			virtual JsonValue& operator[](const std::string& field) = 0;
			virtual const JsonValue& operator[](size_t index) const = 0;
			virtual const JsonValue& operator[](const std::string& field) const = 0;

			// Block Type
			virtual JsonType getType() const = 0;
		
			// Element manipulation
			virtual void push_back(const JsonValue& element, std::string field = "") = 0;
			virtual void erase(const size_t) = 0;
			virtual void erase(const std::string& field) = 0;
			virtual void clear() = 0;
			
			// Key operations
			virtual bool hasField(const std::string&) const = 0;
			virtual bool hasField(const size_t) const = 0;
			virtual std::vector<std::string> getFields() const = 0;
			
			// Return Json 
			virtual std::string getJsonString() const = 0;
			
			//Constructor
			JsonBlock(){};
			virtual ~JsonBlock() = default;
		protected:
	};

} // namespace WSM

#endif // WSM_JSON_PARSER_BLOCK_H