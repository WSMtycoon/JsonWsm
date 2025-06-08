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

#pragma once

#include <memory>
#include <functional>
#include <string>
#include <map>
#include <vector>
#include <optional>

#include "JsonArray.h"
#include "JsonObject.h"

namespace WSM {

	class JsonParser: public JsonObject {
	public:

		// Type checking methods
		JsonType getType(const std::string& key) const;
		bool isEmpty(const std::string& key) const;
		bool isBool(const std::string& key) const;
		bool isNumeric(const std::string& key) const;
		bool isFloat(const std::string& key) const;
		bool isDouble(const std::string& key) const;
		bool isString(const std::string& key) const;
		bool isArray(const std::string& key) const;
		bool isObject(const std::string& key) const;
		bool isNull(const std::string& key) const;

		bool isCorrectlyParsed() const { return correctlyParsed; }
		
		// Value retrieval methods
		std::optional<bool> getBool(const std::string& key) const;
		std::optional<int> getInt(const std::string& key) const;
		std::optional<float> getFloat(const std::string& key) const;
		std::optional<double> getDouble(const std::string& key) const;
		std::optional<std::string> getString(const std::string& key) const;
		std::shared_ptr<JsonArray> getArray(const std::string& key) const;
		std::shared_ptr<JsonObject> getObject(const std::string& key) const;
		std::vector<std::string> getFields(const std::string& key = "") const;

		JsonParser(const std::string& jsonStr);

	private:
		bool correctlyParsed = false;

		std::optional<JsonValue> getValue(const std::string& key) const;

		// Helper methods
		void parseJson(const std::string& jsonStr);
		JsonValue parseValue(const std::string& value);

		// Parsing helpers
		std::vector<std::pair<std::string, std::string>> splitJsonObject(const std::string& jsonStr);
		std::string extractJsonValue(const std::string& jsonStr, size_t& pos);
		std::string extractJsonKey(const std::string& jsonStr, size_t& pos);
		std::vector<JsonValue> parseArray(const std::string& value);

	};

} // namespace WSM
