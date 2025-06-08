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
	
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "../../Utils/Source/JsonUtils.h"
#include "../../Source/JsonParser.h"

std::string strBool(bool is){return is ? "True" : "False";}
std::string strAction(bool is){return is ? "Yes" : "No";}
std::string strSign(bool is){return is ? ", " : ": ";}
std::string strEquel(bool is){return is ? " == ": " != ";}
std::string strTest(bool is){return is ? "completed successfully!" : "failed validation!";}

// Вспомогательные функции для проверки
bool verifyObject(const WSM::JsonValue& object, const std::vector<std::string> fields , size_t length){

	bool obj = (object.isObject()) ? true: false;
	bool size = object.size() == length;
	bool field = true; for(std::string item: fields){ if(!object.hasField(item)){  field = false; break;} }

	std::cout << "\n";
	std::cout << "Type Object " << strBool(obj) << ":";
	std::cout << " Size " << length << "[" << object.size() << "] " << strBool(size) << ":"; 
	std::cout << " {";
	for(size_t i = 0; i < fields.size();){
		std::cout << "\"" + fields[i] + "\": "; 
		std::cout << strAction(object.hasField(fields[i]));
		std::cout << strSign(++i < fields.size());
	}
	std::cout << "}" << std::flush;
	return obj && size && field;
}

bool verifyArray(const WSM::JsonValue& array, size_t length) {
	std::cout << "\n"; 
	bool arr = (array.isArray()) ? true: false;
	bool size = array.size() == length;

	std::cout << "Type Array " << strBool(size);
	std::cout << " Size " << length << "[" << array.size() << "] " << strBool(size) << std::flush;

	return arr && size;
}

bool verifyValue(const WSM::JsonValue& element, WSM::JsonType type, std::string value) {
	std::cout <<"\n"; 
	bool item = element.getType() == type;
	bool equel = value == element.getValueString();

	std::cout << "    Type " << WSM::JsonUtils::getJsonTypeString(type) << " equel - " << strBool(equel) << ": ";
	std::cout << value << strEquel(equel) << element.getValueString() << std::flush;
	return item && equel;
}

int main() {
	// Тестовая JSON структура
	std::string testJson = R"({
		"user": { 
			"id": 1,
			"profile": {"name": "John Doe", "age": 30 }, 
			"address": { "street": "123 Main St", "city": "New York" }, 
			"coordinates": { "latitude": "40.7128f", "longitude": "-74.0060f" } 
		},
		"items": [
			{"id": 1, "name": "Item 1", "details": { "price": "19.99f", "stock": 100}},
			{"id": 2, "name": "Item 2", "details": { "price": "29.99f", "stock": 50 }} 
		],
		"nested_arrays": [[[1, 2, 3], [4, 5, 6]], [[7, 8, 9], [10, 11, 12]]],
		"mixed_structures": [{"array": [1, 2, 3], "object": {"nested_array": [[1, 2], [3, 4]]}}]
	})";

	WSM::JsonParser parser(testJson);
	bool testParser = parser.isCorrectlyParsed();
	bool testBool = true;
	bool testInt = true;
	bool testLong = true;
	bool testFloat = true; 
	bool testDouble = true;
	bool testString = true;
	bool testObject = true;
	bool testArray = true;
	// Проверка базовых методов
	
	std::cout << "Parsing is Corrert - " << strBool(testParser);
	
	if(parser.hasField("user") && verifyObject(parser["user"], {"id","profile", "address", "coordinates"}, 4)){
		if(!verifyValue(parser["user"]["id"], WSM::JsonType::INT ,"1")){testInt = false;};
	} else { std::cout << "\nError: Invalid Object parser[\"user\"]" << std::flush; testObject = false; }

	// Проверка user объекта
	if(parser["user"].hasField("profile") && verifyObject(parser["user"]["profile"], {"name", "age"}, 2)) {
		if(!verifyValue(parser["user"]["profile"]["name"], WSM::JsonType::STRING, "John Doe")) { testString = false; }
		if(!verifyValue(parser["user"]["profile"]["age"], WSM::JsonType::INT, "30")) { testInt = false; }
	} else { std::cout << "\nError: Invalid Object parser[\"user\"][\"profile\"]" << std::flush; testObject = false; }

	if(parser["user"].hasField("address") && verifyObject(parser["user"]["address"], {"street", "city"}, 2)) {
		if(!verifyValue(parser["user"]["address"]["street"], WSM::JsonType::STRING, "123 Main St")) { testString = false; }
		if(!verifyValue(parser["user"]["address"]["city"], WSM::JsonType::STRING, "New York")) { testString = false; }
	} else { std::cout << "\nError: Invalid Object parser[\"user\"][\"address\"]" << std::flush; testObject = false; }

	if(parser["user"].hasField("coordinates") && verifyObject(parser["user"]["coordinates"], {"latitude", "longitude"}, 2)) {
		if(!verifyValue(parser["user"]["coordinates"]["latitude"], WSM::JsonType::FLOAT, "40.7128f")) { testFloat = false; }
		if(!verifyValue(parser["user"]["coordinates"]["longitude"], WSM::JsonType::FLOAT, "-74.0060f")) { testFloat = false; }
	} else { std::cout << "\nError: Invalid Object parser[\"user\"][\"coordinates\"]" << std::flush; testObject = false; }

	// Проверка массива items
	if(parser.hasField("items") && verifyArray(parser["items"], 2)) {
		// Проверка первого элемента
		if(parser["items"].hasField(0) && verifyObject(parser["items"][0], {"id", "name", "details"}, 3)) {
			if(!verifyValue(parser["items"][0]["id"], WSM::JsonType::INT, "1")) { testInt = false; }
			if(!verifyValue(parser["items"][0]["name"], WSM::JsonType::STRING, "Item 1")) { testString = false; }
			if(parser["items"][0].hasField("details") && verifyObject(parser["items"][0]["details"], {"price", "stock"}, 2)) {
				if(!verifyValue(parser["items"][0]["details"]["price"], WSM::JsonType::FLOAT, "19.99f")) { testFloat = false; }
				if(!verifyValue(parser["items"][0]["details"]["stock"], WSM::JsonType::INT, "100")) { testInt = false; }
			} else { std::cout << "\nError: Invalid Object parser[\"items\"][0][\"details\"]" << std::flush; testObject = false; }
		} else { std::cout << "\nError: Invalid Object parser[\"items\"][0]" << std::flush; testObject = false; }

		// Проверка второго элемента
		if(parser["items"].hasField(1) && verifyObject(parser["items"][1], {"id", "name", "details"}, 3)) {
			if(!verifyValue(parser["items"][1]["id"], WSM::JsonType::INT, "2")) { testInt = false; }
			if(!verifyValue(parser["items"][1]["name"], WSM::JsonType::STRING, "Item 2")) { testString = false; }
			if(parser["items"][1].hasField("details") && verifyObject(parser["items"][1]["details"], {"price", "stock"}, 2)) {
				if(!verifyValue(parser["items"][1]["details"]["price"], WSM::JsonType::FLOAT, "29.99f")) { testFloat = false; }
				if(!verifyValue(parser["items"][1]["details"]["stock"], WSM::JsonType::INT, "50")) { testInt = false; }
			} else { std::cout << "\nError: Invalid Object parser[\"items\"[1][\"details\"]" << std::flush; testObject = false; }
		} else { std::cout << "\nError: Invalid Object parser[\"items\"][1]" << std::flush; testObject = false; }
	} else { std::cout << "\nError: Invalid Array parser[\"items\"]" << std::flush; testArray = false; }

	// Проверка вложенных массивов
	if(parser.hasField("nested_arrays") && verifyArray(parser["nested_arrays"], 2)) {
		if(parser["nested_arrays"].hasField(0) && verifyArray(parser["nested_arrays"][0], 2)) {
			if(parser["nested_arrays"][0].hasField(0) && verifyArray(parser["nested_arrays"][0][0], 3)) {
				if(!verifyValue(parser["nested_arrays"][0][0][0], WSM::JsonType::INT, "1")) { testInt = false; }
				if(!verifyValue(parser["nested_arrays"][0][0][1], WSM::JsonType::INT, "2")) { testInt = false; }
				if(!verifyValue(parser["nested_arrays"][0][0][2], WSM::JsonType::INT, "3")) { testInt = false; }
			} else { std::cout << "\nError: Invalid Array parser[\"nested_arrays\"][0][0]" << std::flush; testArray = false; }
		} else { std::cout << "\nError: Invalid Array parser[\"nested_arrays\"][0]" << std::flush; testArray = false; }
	} else { std::cout << "\nError: Invalid Array parser[\"nested_arrays\"]" << std::flush; testArray = false; }

	// Проверка смешанных структур
	if(parser.hasField("mixed_structures") && verifyArray(parser["mixed_structures"], 1)) {
		if(parser["mixed_structures"].hasField(0) && verifyObject(parser["mixed_structures"][0], {"array", "object"}, 2)) {
			if(parser["mixed_structures"][0].hasField("array") && verifyArray(parser["mixed_structures"][0]["array"], 3)) {
				if(!verifyValue(parser["mixed_structures"][0]["array"][0], WSM::JsonType::INT, "1")) { testInt = false; }
				if(!verifyValue(parser["mixed_structures"][0]["array"][1], WSM::JsonType::INT, "2")) { testInt = false; }
				if(!verifyValue(parser["mixed_structures"][0]["array"][2], WSM::JsonType::INT, "3")) { testInt = false; }
			} else { std::cout << "\nError: Invalid Array parser[\"mixed_structures\"][0][\"array\"]" << std::flush; testArray = false; }
			if(parser["mixed_structures"][0].hasField("object") && verifyObject(parser["mixed_structures"][0]["object"], {"nested_array"}, 1)) {
				if(parser["mixed_structures"][0]["object"].hasField("nested_array") && verifyArray(parser["mixed_structures"][0]["object"]["nested_array"], 2)) {
					if(parser["mixed_structures"][0]["object"]["nested_array"].hasField(0) && verifyArray(parser["mixed_structures"][0]["object"]["nested_array"][0], 2)) {
						if(!verifyValue(parser["mixed_structures"][0]["object"]["nested_array"][0][0], WSM::JsonType::INT, "1")) { testInt = false; }
						if(!verifyValue(parser["mixed_structures"][0]["object"]["nested_array"][0][1], WSM::JsonType::INT, "2")) { testInt = false; }
					} else { std::cout << "\nError: Invalid Array parser[\"mixed_structures\"][0][\"object\"][\"nested_array\"][0]" << std::flush; testArray = false; }
				} else { std::cout << "\nError: Invalid Array parser[\"mixed_structures\"][0][\"object\"][\"nested_array\"]" << std::flush; testArray = false; }
			} else { std::cout << "\nError: Invalid Object parser[\"mixed_structures\"][0][\"object\"]" << std::flush; testObject = false; }
		} else { std::cout << "\nError: Invalid Object parser[\"mixed_structures\"][0]" << std::flush; testArray = false; }
	} else { std::cout << "\nError: Invalid Array parser[\"mixed_structures\"]" << std::flush; testArray = false; }

	std::cout << "\n";
	
	std::cout << "Parser tests - " << strTest(testParser) << std::endl;
	std::cout << "Boolean tests - " << strTest(testBool) << std::endl;
	std::cout << "Integer tests - " << strTest(testInt) << std::endl;
	std::cout << "Long tests - " << strTest(testLong) << std::endl;
	std::cout << "Float tests - " << strTest(testFloat) << std::endl;
	std::cout << "Double tests - " << strTest(testDouble) << std::endl;
	std::cout << "String tests - " << strTest(testString) << std::endl;
	std::cout << "Object structure tests - " << strTest(testObject) << std::endl;
	std::cout << "Array structure tests - " << strTest(testArray) << std::endl;

	return 0;
} 