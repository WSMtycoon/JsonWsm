# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -I.
LDFLAGS = 

# Directories
SRC_DIR = Source
TEST_DIR = Test/Source
UTILS_DIR = Utils/Source
BIN_DIR = bin
LIB_DIR = lib
OBJ_DIR = Object

# Header-only files
HEADER_ONLY_FILES = \
    $(SRC_DIR)/JsonType.h \
    $(SRC_DIR)/JsonBlock.h \
    $(SRC_DIR)/JsonValue.h \
    $(SRC_DIR)/JsonArray.h \
    $(SRC_DIR)/JsonObject.h \
    $(SRC_DIR)/JsonParser.h 

# Source files
PARSER_SRCS = \
    $(SRC_DIR)/JsonValue.cpp \
    $(SRC_DIR)/JsonArray.cpp \
    $(SRC_DIR)/JsonObject.cpp \
    $(SRC_DIR)/JsonParser.cpp

UTILS_SRCS = $(wildcard $(UTILS_DIR)/*.cpp)

# Test files
TEST_PARSER_SRC = $(TEST_DIR)/test_parser.cpp
TEST_STRUCT_SRC = $(TEST_DIR)/test_struct.cpp

# Object files
PARSER_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(PARSER_SRCS))
UTILS_OBJS = $(patsubst $(UTILS_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(UTILS_SRCS))
TEST_PARSER_OBJ = $(OBJ_DIR)/test_parser.o
TEST_STRUCT_OBJ = $(OBJ_DIR)/test_struct.o

# Libraries and executables
PARSER_LIB = $(LIB_DIR)/libjsonparser.a
UTILS_LIB = $(LIB_DIR)/libjsonutils.a
PARSER_EXE = $(BIN_DIR)/parser.exe
STRUCT_EXE = $(BIN_DIR)/struct.exe

# Create necessary directories
$(shell mkdir -p $(BIN_DIR) $(LIB_DIR) $(OBJ_DIR))

# Default target
all: parser utils tests libs

# Build parser object files
parser: $(PARSER_OBJS)

# Build utils object files
utils: $(UTILS_OBJS)

# Build tests struct
struct: $(STRUCT_EXE)

# Build tests parser
tests: $(PARSER_EXE) $(STRUCT_EXE)

# Build library
libs: $(PARSER_LIB) $(UTILS_LIB)

# Clean all temporary build files
clean:
	rm -rf $(BIN_DIR) $(LIB_DIR) $(OBJ_DIR)

# Compile parser source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADER_ONLY_FILES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile utils source files
$(OBJ_DIR)/%.o: $(UTILS_DIR)/%.cpp $(HEADER_ONLY_FILES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test files
$(TEST_PARSER_OBJ): $(TEST_PARSER_SRC) $(HEADER_ONLY_FILES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_STRUCT_OBJ): $(TEST_STRUCT_SRC) $(HEADER_ONLY_FILES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create parser library
$(PARSER_LIB): $(PARSER_OBJS)
	ar rcs $@ $^

# Create utils library
$(UTILS_LIB): $(UTILS_OBJS)
	ar rcs $@ $^

# Link test executables
$(PARSER_EXE): $(TEST_PARSER_OBJ) $(PARSER_LIB) $(UTILS_LIB)
	$(CXX) $(LDFLAGS) $^ -o $@

$(STRUCT_EXE): $(TEST_STRUCT_OBJ) $(PARSER_LIB) $(UTILS_LIB)
	$(CXX) $(LDFLAGS) $^ -o $@

.PHONY: all clean parser utils test lib 