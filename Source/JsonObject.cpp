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

#include "JsonObject.h"
#include <sstream>
#include <iomanip>

namespace WSM {

	std::optional<JsonValue &> JsonObject::operator[](size_t index){ return get(std::to_string(index)); }
	std::optional<JsonValue &> JsonObject::operator[](const std::string& field) { return get(field); }
	std::optional<JsonValue &> JsonObject::get(std::string field){
		if(!data.empty() && data.find(field) != data.end()){ return data[field]; }
		return std::nullopt;
	}

	void JsonObject::push_back(std::string field, const JsonValue & element){ 
		data.emplace(field, element); 
	}

	void JsonObject::erase(const std::string& field) { 
		if(!data.empty() && data.find(field) != data.end()){ data.erase(field); }
	}

	std::vector<std::string> JsonObject::getFields() const {
		std::vector<std::string> fields;
		fields.reserve(data.size());
		for (const auto& pair : data) { fields.push_back(pair.first); }
		return fields;
	}

	std::string JsonObject::getValueString() const {
		std::stringstream ss;
		ss << "{";
		bool first = true;
		for (const auto& pair : data) {
			if (!first) ss << ", ";
			ss << "\"" << pair.first << "\": " << pair.second.getValueString();
			first = false;
		}
		ss << "}";
		return ss.str();
	}

} // namespace WSM
