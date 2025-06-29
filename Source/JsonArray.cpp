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

#include "../Header/JsonWsm/JsonArray.h"

namespace WSM {

	JsonArray::JsonArray(std::vector<JsonValue> array){
		if(!array.empty()){ for(auto element: array){ data.emplace_back(std::make_shared<JsonValue>(element)); } }
	}

	JsonValue& JsonArray::operator[](size_t index){ return get(index); }
	JsonValue& JsonArray::operator[](const std::string &field){ return get(std::stoul(field)); }
	const JsonValue& JsonArray::operator[](size_t index) const { return get(index); }
	const JsonValue& JsonArray::operator[](const std::string &field) const { return get(std::stoul(field)); }

	JsonValue &JsonArray::get(size_t index){
		if (index >= 0 && index < data.size()) { return *(data[index].get()); } 
		throw std::invalid_argument("Index not found - " + std::to_string(index));
	}

	const JsonValue &JsonArray::get(size_t index) const {
		if (index >= 0 && index < data.size()) { return *(data[index].get()); } 
		throw std::invalid_argument("Index not found - " + std::to_string(index));
	}

	std::vector<std::string> JsonArray::getFields() const{
		std::vector<std::string> fields;
		fields.reserve(data.size());
		for (size_t i = 0; i < data.size(); ++i) { fields.push_back(std::to_string(i)); }
		return fields;
	}

	std::string JsonArray::getJsonString() const{
		std::stringstream ss;
		ss << "[";
		for (size_t i = 0; i < data.size(); ++i) {
			if (i > 0) ss << ", ";
			ss << data[i]->getValueString();
		}
		ss << "]";
		return ss.str();
	}
} // namespace WSM



