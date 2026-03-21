/**************************************************************************/
/**                  Created by WsmTycoon on 07.06.2025                  **/
/**************************************************************************/

/** You are solely responsible for determining the appropriateness of using
 * or redistributing the Work and assume any risks associated with Your
 * exercise of permissions under this License. **/
/** Software distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. **/

#ifndef WSM_JSON_PARSER_TYPE_H
#define WSM_JSON_PARSER_TYPE_H

namespace WSM {
	enum class JsonType {BOOL, INT, LONG, FLOAT, DOUBLE, STRING, ARRAY, OBJECT, EMPTY, NULL_TYPE};
}	
#endif // WSM_JSON_TYPE_H