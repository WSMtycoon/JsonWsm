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

#ifndef WSM_JSON_PARSER_UTILS_H
#define WSM_JSON_PARSER_UTILS_H


#include <sstream>
#include <iomanip>
#include <regex>
#include <iostream>

#include "../../Source/JsonParser.h"

namespace WSM {

	class JsonUtils {
	public:
		// Type checking utilities
		static std::string getJsonTypeString(const JsonType& value);
		
		// Testing helper methods
		static bool compareJsonElements(const JsonValue& a, const JsonValue& b);
		static bool compareJsonObjects(const JsonObject& a, const JsonObject& b);
		static bool compareJsonArrays(const JsonArray& a, const JsonArray& b);
		
		
		// Validation utilities
		static bool isValidJsonString(const std::string& str);
		static bool isValidJsonNumber(const std::string& str);
		static bool isValidJsonBoolean(const std::string& str);
		static bool isValidJsonNull(const std::string& str);
		
		// Path utilities
		static std::vector<std::string> splitPath(const std::string& path);
		static bool isValidPath(const std::string& path);
		
		// Debug utilities
		static void printJsonElement(const JsonValue& element, int indent = 0);
		static void printJsonObject(const JsonObject& object, int indent = 0);
		static void printJsonArray(const JsonArray& array, int indent = 0);

		static void traverse(const std::function<void(const std::string& path, const JsonValue& value, JsonType type)>& callback, 
				 const std::string& prefix = "");
	private:
		// Traversal helper
		static void traverseHelper(const JsonValue& value, 
					   const std::function<void(const std::string& path, const JsonValue& value, JsonType type)>& callback,
					   const std::string& prefix);			 

	};

} // namespace WSM

#endif // WSM_JSON_PARSER_UTILS_H
