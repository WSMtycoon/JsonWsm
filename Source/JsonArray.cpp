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

#include "JsonArray.h"
#include <sstream>
#include <iomanip>

namespace WSM {

	std::optional<JsonValue &> JsonArray::get(size_t index){
		if (index > -1 && index < data.size()) { return data[index]; }
		return std::nullopt;
	}

	std::vector<std::string> JsonArray::getFields() const{
		std::vector<std::string> fields;
		fields.reserve(data.size());
		for (size_t i = 0; i < data.size(); ++i) { fields.push_back(std::to_string(i)); }
		return fields;
	}

	std::string JsonArray::getValueString() const{
		std::stringstream ss;
		ss << "[";
		for (size_t i = 0; i < data.size(); ++i) {
			if (i > 0) ss << ", ";
			ss << data[i].getValueString();
		}
		ss << "]";
		return ss.str();
	}

} // namespace WSM



