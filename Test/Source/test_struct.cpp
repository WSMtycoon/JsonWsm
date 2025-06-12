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

void strIndeng(size_t set){if(0 < set < 20 ) {while(set--){std::cout << "|   ";}std::cout << "|---";}};
std::string strBool(bool is){return is ? "True" : "\033[31mFalse\033[0m";}
std::string strAction(bool is){return is ? "Yes" : "No";}
std::string strSign(bool is){return is ? ", " : "";}
std::string strEquel(bool is){return is ? " == ": " != ";}
std::string strTest(bool test, bool is){
	if (is && test) {return "Completed successfully!";}
	if (is && !test) {return "Failed validation!";}
	if (!is) { return "Not Testing!"; } 
	return "Error StrTest Metod!";
}

// Вспомогательные функции для проверки
bool verifyObject(const WSM::JsonValue& object, const std::vector<std::string> fields, size_t indent = 0){

	bool obj = (object.isObject()) ? true: false;
	bool size = object.size() == fields.size();
	bool field = true; for(std::string item: fields){ if(!object.hasField(item)){  field = false; break;} }

	std::cout << "\n";
	strIndeng(indent);
	std::cout << "OBJECT " << strBool(obj) << ":";
	std::cout << " Size " << fields.size() << "[" << object.size() << "] " << strBool(size) << ":"; 
	std::cout << " {";
	for(size_t i = 0; i < fields.size();){
		std::cout << "\"" + fields[i] + "\": "; 
		std::cout << strAction(object.hasField(fields[i]));
		std::cout << strSign(++i < fields.size());
	}
	std::cout << "}" << std::flush;
	return obj && size && field;
}

bool verifyArray(const WSM::JsonValue& array, size_t length, size_t indent = 0) {
	std::cout << "\n"; 
	bool arr = (array.isArray()) ? true: false;
	bool size = array.size() == length;
	strIndeng(indent);
	std::cout << "ARRAY " << strBool(size);
	std::cout << " Size " << length << "[" << array.size() << "] " << strBool(size) << std::flush;

	return arr && size;
}

bool verifyValue(const WSM::JsonValue& element, WSM::JsonType type, std::string value, size_t indent = 0) {
	bool item = element.getType() == type;
	bool equel = value == element.getValueString();
	std::cout <<"\n"; 
	strIndeng(indent);
	std::cout << WSM::JsonUtils::getJsonTypeString(type) << " " << WSM::JsonUtils::getJsonTypeString(element.getType()) << " - " << strBool(item) << " || equel - " << strBool(equel) << ": ";
	std::cout << value << strEquel(equel) << element.getValueString() << std::flush;
	return item && equel;
}

int main() {
	// Тестовая JSON структура
	std::string testJson = R"({
		"user": { 
			"id": 1,
			"action": false,
			"profile": {"name": "John Doe", "age": 30 }, 
			"address": { "street": "123 Main St", "city": "New York" }, 
			"coordinates": { "latitude": "40.7128", "longitude": "-74.006" } 
		},
		"items": [
			{"id": 1, "name": "Item 1", "details": { "price": 19.99f, "stock": 100}},
			{"id": 2, "name": "Item 2", "details": { "price": "29.99f", "stock": 50 }} 
		],
		"error": {"empty":, "null": null, "string": ""},
		"array": [ true, 9023, 9134987094l, 34.33f, 73.20871987, "Hell"],
		"nested_arrays": [[[1, 2, 3], [4, 5, 6]], [[7, 8, 9], [10, 11, 12]]],
		"mixed_structures": [{"array": [1, 2, 3], "object": {"nested_array": [[1, 2], [3, 4]]}}]
	})";

	WSM::JsonParser parser(testJson);
	bool testParser = parser.isCorrectlyParsed();
	bool testBool = true; bool isBool = false;
	bool testNull = true; bool isNull = false;
	bool testEmpty = true; bool isEmpty = false;
	bool testInt = true; bool isInt = false;
	bool testLong = true; bool isLong = false;
	bool testFloat = true; bool isFloat = false; 
	bool testDouble = true; bool isDouble = false;
	bool testString = true; bool isString = false;
	bool testObject = true; bool isObject = false;
	bool testArray = true; bool isArray = false;
	bool testOObject = true; bool isOObject = false; 
	bool testAObject = true; bool isAObject = false; 
	bool testOArray = true; bool isOArray = false; 
	bool testAArray = true; bool isAArray = false; 
	bool testMixStruct = true; bool isMixStruct = false; 
	// Проверка базовых методов

	std::cout << "Parsing is Corrert - " << strBool(testParser) << std::endl;
	std::cout << "Parsing - {";
	auto fields = parser.getFields();
	for(size_t i = 0; i < fields.size();){ std::cout << "\"" + fields[i] + "\"" << strSign(++i < fields.size()); }
	std::cout << "}" << std::endl;

	// Проверка user объекта
	if(parser.hasField("user") && verifyObject(parser["user"], {"id","action","profile", "address", "coordinates"}, 0)){
		if(!verifyValue(parser["user"]["id"], WSM::JsonType::INT ,"1", 1)){testInt = false; isObject = false;}; isInt = true;
		if(!verifyValue(parser["user"]["action"], WSM::JsonType::BOOL , "false", 1)){testBool = false; isObject = false;}; isBool = true;
		if(parser["user"].hasField("profile") && verifyObject(parser["user"]["profile"], {"name", "age"}, 1)) {
			if(!verifyValue(parser["user"]["profile"]["name"], WSM::JsonType::STRING, "John Doe", 2)) { testString = false; testOObject = false;} isString = true;
			if(!verifyValue(parser["user"]["profile"]["age"], WSM::JsonType::INT, "30", 2)) { testInt = false; testOObject = false;} isInt = true;
		} else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"user\"][\"profile\"]" << std::flush; testOObject = false; }
		if(parser["user"].hasField("address") && verifyObject(parser["user"]["address"], {"street", "city"}, 1)) {
			if(!verifyValue(parser["user"]["address"]["street"], WSM::JsonType::STRING, "123 Main St", 2)) { testString = false; testOObject = false;}; isString = true;
			if(!verifyValue(parser["user"]["address"]["city"], WSM::JsonType::STRING, "New York", 2)) { testString = false; testOObject = false;}; isString = true;
		} else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"user\"][\"address\"]" << std::flush; testOObject = false; }
		if(parser["user"].hasField("coordinates") && verifyObject(parser["user"]["coordinates"], {"latitude", "longitude"}, 1)) {
			if(!verifyValue(parser["user"]["coordinates"]["latitude"], WSM::JsonType::DOUBLE, "40.7128", 2)) { testDouble = false; testOObject = false; }; isDouble = true;
			if(!verifyValue(parser["user"]["coordinates"]["longitude"], WSM::JsonType::DOUBLE, "-74.006", 2)) { testDouble = false; testOObject = false; }; testOObject = true;
			isOObject = true;
		} else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"user\"][\"coordinates\"]" << std::flush; testOObject = false; }
		isObject = true;
	} else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"user\"]" << std::flush; testObject = false; }

	// Проверка массива items
	if(parser.hasField("items") && verifyArray(parser["items"], 2, 0)) {
		// Проверка первого элемента
		if(parser["items"].hasField(0)) {
			if(verifyObject(parser["items"][0], {"id", "name", "details"}, 1)) {
				if(!verifyValue(parser["items"][0]["id"], WSM::JsonType::INT, "1", 2)) { testInt = false;}; isInt = true;
				if(!verifyValue(parser["items"][0]["name"], WSM::JsonType::STRING, "Item 1", 2)) { testString = false;} ; isString = true;
				isAObject = true;
				if(parser["items"][0].hasField("details") && verifyObject(parser["items"][0]["details"], {"price", "stock"}, 2)) {
					if(!verifyValue(parser["items"][0]["details"]["price"], WSM::JsonType::FLOAT, "19.99f", 3)) { testFloat = false; testOObject = false; }; isFloat = true;
					if(!verifyValue(parser["items"][0]["details"]["stock"], WSM::JsonType::INT, "100", 3)) { testInt = false; testOObject = false;}; isInt = true;
				} else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"items\"][0][\"details\"]" << std::flush; testOObject = false; }
			} else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"items\"][0]" << std::flush; testAObject = false; }
		} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"items\"][0]" << std::flush; testArray = false; }
		// Проверка второго элемента
		if(parser["items"].hasField(1)) {
			if(verifyObject(parser["items"][1], {"id", "name", "details"}, 1)) {
				if(!verifyValue(parser["items"][1]["id"], WSM::JsonType::INT, "2", 2)) { testInt = false; testAObject = false;}; isInt = true;
				if(!verifyValue(parser["items"][1]["name"], WSM::JsonType::STRING, "Item 2", 2)) { testString = false; testAObject = false;} isString = true;
				isAObject = true;
				if(parser["items"][1].hasField("details") && verifyObject(parser["items"][1]["details"], {"price", "stock"}, 2)) {
					if(!verifyValue(parser["items"][1]["details"]["price"], WSM::JsonType::FLOAT, "29.99f", 3)) { testFloat = false; testAObject = false;}; isFloat = true;
					if(!verifyValue(parser["items"][1]["details"]["stock"], WSM::JsonType::INT, "50", 3)) { testInt = false; testAObject = false;}; isInt = true;
				} else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"items\"[1][\"details\"]" << std::flush; testAObject = false; }
			} else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"items\"][1]" << std::flush; testAObject = false; }
		} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"items\"][1]" << std::flush; testArray = false; }
		isArray = true;
	} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"items\"]" << std::flush; testArray = false; }
	
	// Проверка стрктуры массива. 
	if(parser.hasField("array") && verifyArray(parser["array"], 6, 0)) {
		if(parser["array"].hasField(0)) { if(!verifyValue(parser["array"][0], WSM::JsonType::BOOL, "true", 1)) { testBool = false; testArray = false; }; isBool = true;
		} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"array\"][0]" << std::flush; testArray = false; }
		if(parser["array"].hasField(1)) { if(!verifyValue(parser["array"][1], WSM::JsonType::INT, "9023", 1)) { testInt = false; testArray = false; }; isInt = true;
		} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"array\"][1]" << std::flush; testArray = false; }
		if(parser["array"].hasField(2)) { if(!verifyValue(parser["array"][2], WSM::JsonType::LONG, "9134987094l", 1)) { testLong = false; testArray = false; }; isLong = true;
		} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"array\"][2]" << std::flush; testArray = false; }
		if(parser["array"].hasField(3)) { if(!verifyValue(parser["array"][3], WSM::JsonType::FLOAT, "34.33f", 1)) { testFloat = false; testArray = false; }; isFloat = true;
		} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"array\"][3]" << std::flush; testArray = false; }
		if(parser["array"].hasField(4)) { if(!verifyValue(parser["array"][4], WSM::JsonType::DOUBLE, "73.20871987", 1)) { testDouble = false; testArray = false; }; isDouble = true;
		} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"array\"][4]" << std::flush; testArray = false; }
		if(parser["array"].hasField(5)) { if(!verifyValue(parser["array"][5], WSM::JsonType::STRING, "Hell", 1)) { testString = false; testArray = false; }; isString = true;
		} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"array\"][5]" << std::flush; testArray = false; }
	} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"array\"]" << std::flush; testArray = false; }
	
	// Проверка сложных типов
	if(parser.hasField("error") && verifyObject(parser["error"], {"empty", "null", "string"}, 0)) {
		if(parser["error"].hasField("empty")) { if(!verifyValue(parser["error"]["empty"], WSM::JsonType::EMPTY, "", 1)) { testEmpty = false; }; isEmpty = true;}
		else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"error\"][\"empty\"]" << std::flush; testEmpty = false; }
		if(parser["error"].hasField("null")) { if(!verifyValue(parser["error"]["null"], WSM::JsonType::NULL_TYPE, "null", 1)) { testNull = false; }; isNull = true;}
		else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"error\"][\"null\"]" << std::flush; testNull = false; }
		if(parser["error"].hasField("string")) { if(!verifyValue(parser["error"]["string"], WSM::JsonType::STRING, "", 1)) { testString = false; }; isString = true;}
		else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"error\"][\"string\"]" << std::flush; testString = false; }
	} else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"error\"]" << std::flush; testEmpty = false; testNull = false; }

	// Проверка сложной структуры массивов
	if(parser.hasField("nested_arrays") && verifyArray(parser["nested_arrays"], 2, 0)) {
		if(parser["nested_arrays"].hasField(0) && verifyArray(parser["nested_arrays"][0], 2, 1)) {
			if(parser["nested_arrays"][0].hasField(0) && verifyArray(parser["nested_arrays"][0][0], 3, 2)) {
				if(!verifyValue(parser["nested_arrays"][0][0][0], WSM::JsonType::INT, "1", 3)) { testInt = false; testAArray = false; }; isInt = true;
				if(!verifyValue(parser["nested_arrays"][0][0][1], WSM::JsonType::INT, "2", 3)) { testInt = false; testAArray = false; }; isInt = true;
				if(!verifyValue(parser["nested_arrays"][0][0][2], WSM::JsonType::INT, "3", 3)) { testInt = false; testAArray = false; }; isInt = true;
				isAArray = true;
			} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"nested_arrays\"][0][0]" << std::flush; testAArray = false; }
		} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"nested_arrays\"][0]" << std::flush; testAArray = false; }
	} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"nested_arrays\"]" << std::flush; testArray = false; }

	// Проверка смешанных структур
	if(parser.hasField("mixed_structures") && verifyArray(parser["mixed_structures"], 1, 0)) {
		if(parser["mixed_structures"].hasField(0)) {
			if(verifyObject(parser["mixed_structures"][0], {"array", "object"}, 1)) {
				if(parser["mixed_structures"][0].hasField("array")) {
					if(verifyArray(parser["mixed_structures"][0]["array"], 3, 2)) {
						if(!verifyValue(parser["mixed_structures"][0]["array"][0], WSM::JsonType::INT, "1", 3)) { testInt = false; testOArray = false;}; isInt = true;
						if(!verifyValue(parser["mixed_structures"][0]["array"][1], WSM::JsonType::INT, "2", 3)) { testInt = false; testOArray = false;}; isInt = true;
						if(!verifyValue(parser["mixed_structures"][0]["array"][2], WSM::JsonType::INT, "3", 3)) { testInt = false; testOArray = false;}; isInt = true;
						isOArray = true;
					} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"mixed_structures\"][0][\"array\"]" << std::flush; testOArray = false; }
				} else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"mixed_structures\"][0][\"array\"]" << std::flush; testOObject = false; }
				if(parser["mixed_structures"][0].hasField("object")) {
					if(verifyObject(parser["mixed_structures"][0]["object"], {"nested_array"}, 2)) {
						if(parser["mixed_structures"][0]["object"].hasField("nested_array")) {
							if(verifyArray(parser["mixed_structures"][0]["object"]["nested_array"], 2, 3)) {
								if(parser["mixed_structures"][0]["object"]["nested_array"].hasField(0)) {
									if(verifyArray(parser["mixed_structures"][0]["object"]["nested_array"][0], 2, 4)) {
										if(!verifyValue(parser["mixed_structures"][0]["object"]["nested_array"][0][0], WSM::JsonType::INT, "1", 5)) { testInt = false; testMixStruct = false;}; isInt = true;
										if(!verifyValue(parser["mixed_structures"][0]["object"]["nested_array"][0][1], WSM::JsonType::INT, "2", 5)) { testInt = false; testMixStruct = false;}; isInt = true;
										isMixStruct = true;
									} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"mixed_structures\"][0][\"object\"][\"nested_array\"][0]" << std::flush; testMixStruct = false; }
								} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"mixed_structures\"][0][\"object\"][\"nested_array\"][0]" << std::flush; testArray = false; }
							} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"mixed_structures\"][0][\"object\"][\"nested_array\"]" << std::flush; testOArray = false; }
						} else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"mixed_structures\"[0][\"object\"][\"nested_array\"]" << std::flush; testOObject = false; }
					} else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"mixed_structures\"][0][\"object\"]" << std::flush; testOObject = false; }
				} else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"mixed_structures\"][0][\"object\"]" << std::flush; testOObject = false; }
			} else { std::cout << "\n\033[31mError:\033[0m Invalid Object parser[\"mixed_structures\"][0]" << std::flush; testAObject = false; }
		} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"mixed_structures\"][0]" << std::flush; testAArray = false; }
	} else { std::cout << "\n\033[31mError:\033[0m Invalid Array parser[\"mixed_structures\"]" << std::flush; testArray = false; }
	isArray = true;
	std::cout << "\n\n";
	
	std::cout << "Parser tests - " << strTest(testParser, true) << std::endl;
	
	std::cout << "Boolean tests - " << strTest(testBool, isBool) << std::endl;
	std::cout << "Integer tests - " << strTest(testInt, isInt) << std::endl;
	std::cout << "Empty tests - " << strTest(testEmpty, isEmpty) << std::endl;
	std::cout << "Null tests - " << strTest(testNull, isNull) << std::endl;
	std::cout << "Long tests - " << strTest(testLong, isLong) << std::endl;
	std::cout << "Float tests - " << strTest(testFloat, isFloat) << std::endl;
	std::cout << "Double tests - " << strTest(testDouble, isDouble) << std::endl;
	std::cout << "String tests - " << strTest(testString, isString) << std::endl;
	std::cout << "Object structure tests - " << strTest(testObject, isObject) << std::endl;
	std::cout << "Array structure tests - " << strTest(testArray, isArray) << std::endl;
	std::cout << "Object in Object structure tests - " << strTest(testOObject, isOObject) << std::endl;
	std::cout << "Object in Array structure tests - " << strTest(testAObject, isAObject) << std::endl;
	std::cout << "Array in Object structure tests - " << strTest(testOArray, isOArray) << std::endl;
	std::cout << "Array in Array structure tests - " << strTest(testAArray, isAArray) << std::endl;
	std::cout << "Mix Struct tests - " << strTest(testMixStruct, isMixStruct) << std::endl;

	std::cout << "\n"<< std::flush;

	bool testTypes = testBool && testInt && testLong && testFloat && testDouble && testString && testNull && testEmpty;
	bool isTypes = isBool && isInt && isLong && isFloat && isDouble && isString && isNull && isEmpty;
	bool testStruct = testObject && testArray && testOObject && testAObject && testOArray && testAArray && testMixStruct;
	bool isStruct = isObject && isArray && isOObject && isAObject && isOArray && isAArray && isMixStruct;

	std::cout << "Types tests - " << strTest(testTypes, isTypes) << std::endl;
	std::cout << "Structure tests - " << strTest(testStruct, isStruct) << std::endl;
	std::cout << "All tests - " << strTest(testTypes && testStruct, isTypes && isStruct) << std::endl;

	std::cout << "\n"<< std::flush;

	return 0;
} 