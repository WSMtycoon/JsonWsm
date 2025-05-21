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
	
#include "../../Source/JsonWSM.h"
#include <iostream>
#include <fstream>
#include <string>

void verifyField(const WSM::JsonMin& json, const std::string& path, WSM::JsonType expectedType) {
    // Get the value at the path
    std::any value = json.getValue(path);
    
    // Check if the value exists
    if (value.type() == typeid(void*)) {
        std::cout << "Error: Field not found: " << path << std::endl;
        return;
    }

    // Get the actual type
    WSM::JsonType actualType = json.getType(path);
    if (actualType != expectedType) {
        std::cout << "Error: Type mismatch for " << path 
                  << ". Expected: " << static_cast<int>(expectedType)
                  << ", Got: " << static_cast<int>(actualType) << std::endl;
    }
}

void verifyArraySize(const WSM::JsonMin& json, const std::string& path, size_t expectedSize) {
    // Get the value at the path
    std::any value = json.getValue(path);
    
    // Check if the value exists and is an array
    if (value.type() == typeid(void*) || !json.isArray(path)) {
        std::cout << "Error: Array not found: " << path << std::endl;
        return;
    }

    size_t actualSize = json.getSize(path);
    if (actualSize != expectedSize) {
        std::cout << "Error: Array size mismatch for " << path
                  << ". Expected: " << expectedSize
                  << ", Got: " << actualSize << std::endl;
    }
}

int main() {
	std::string testJson = R"({
        "user": {
            "id": 1,
            "name": "John Doe",
            "profile": {
                "age": 30,
                "address": {
                    "street": "123 Main St",
                    "city": "New York",
                    "coordinates": {
                        "latitude": 40.7128f,
                        "longitude": -74.0060f
                    }
                }
            }
        },
        "items": [
            {
                "id": 1,
                "name": "Item 1",
                "details": {
                    "price": 19.99f,
                    "stock": 100
                }
            },
            {
                "id": 2,
                "name": "Item 2",
                "details": {
                    "price": 29.99f,
                    "stock": 50
                }
            }
        ],
        "nested_arrays": [
            [
                [1, 2, 3],
                [4, 5, 6]
            ],
            [
                [7, 8, 9],
                [10, 11, 12]
            ]
        ],
        "mixed_structures": [
            {
                "array": [1, 2, 3],
                "object": {
                    "nested_array": [[1, 2], [3, 4]]
                }
            }
        ]
    })";

    WSM::JsonMin parser(testJson);

    if (!parser.isCorrectly()) {
        std::cout << "Error: Failed to parse JSON" << std::endl;
        return 1;
    }

    // Test field existence and types
    verifyField(parser, "user", WSM::JsonType::OBJECT);
    verifyField(parser, "user.id", WSM::JsonType::INT);
    verifyField(parser, "user.name", WSM::JsonType::STRING);
    verifyField(parser, "user.profile.age", WSM::JsonType::INT);
    verifyField(parser, "user.profile.address.coordinates.latitude", WSM::JsonType::FLOAT);
    verifyField(parser, "user.profile.address.coordinates.longitude", WSM::JsonType::FLOAT);

    // Test array sizes
    verifyArraySize(parser, "items", 2);
    verifyArraySize(parser, "nested_arrays", 2);
    verifyArraySize(parser, "nested_arrays[0]", 2);
    verifyArraySize(parser, "nested_arrays[0][0]", 3);
    verifyArraySize(parser, "mixed_structures", 1);
    verifyArraySize(parser, "mixed_structures[0].array", 3);
    verifyArraySize(parser, "mixed_structures[0].object.nested_array", 2);

    // Test non-existent fields
    if (parser.hasField("nonexistent")) {
        std::cout << "Error: Found non-existent field 'nonexistent'" << std::endl;
    }
    if (parser.hasField("user.nonexistent")) {
        std::cout << "Error: Found non-existent field 'user.nonexistent'" << std::endl;
    }

    // Test array access
    if (parser.getSize("items[0].details") != 0) {
        std::cout << "Error: Expected object, got array for 'items[0].details'" << std::endl;
    }

    // Test nested array access
    if (parser.getSize("nested_arrays[0][0]") != 3) {
        std::cout << "Error: Incorrect size for nested array 'nested_arrays[0][0]'" << std::endl;
    }

    std::cout << "All structure tests completed" << std::endl;
    return 0;
} 