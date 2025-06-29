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

#include <sstream>
#include <iomanip>

#include "../Header/JsonWsm/JsonObject.h"

namespace WSM {

	JsonValue& JsonObject::operator[](size_t index){ return get( std::to_string(index)); }
	JsonValue& JsonObject::operator[](const std::string& field) { return get(field); }
	const JsonValue& JsonObject::operator[](size_t index) const { return get(std::to_string(index)); }
	const JsonValue& JsonObject::operator[](const std::string& field) const { return get(field); }

	JsonValue &JsonObject::get(std::string field){
		auto it = data.find(field);
		if (it == data.end()) { throw std::invalid_argument("Field not found: " + field); }
		return *(it->second);
	}

	const JsonValue &JsonObject::get(std::string field) const {
		auto it = data.find(field);
		if (it == data.end()) { throw std::invalid_argument("Field not found: " + field); }
		return *(it->second);
	}

	void JsonObject::push_back(const JsonValue &element, std::string field){
		data.emplace(field, std::make_shared<JsonValue>(element)); 
	}

	void JsonObject::erase(const std::string &field){
		if(!data.empty() && data.find(field) != data.end()){ data.erase(field); }
	}

	std::vector<std::string> JsonObject::getFields() const {
		std::vector<std::string> fields;
		fields.reserve(data.size());
		for (const auto& pair : data) { fields.push_back(pair.first); }
		return fields;
	}

	std::string JsonObject::getJsonString() const {
		std::stringstream ss;
		ss << "{";
		bool first = true;
		for (const auto& pair : data) {
			if (!first) ss << ", ";
			ss << "\"" << pair.first << "\": " << pair.second->getValueString();
			first = false;
		}
		ss << "}";
		return ss.str();
	}

} // namespace WSM
