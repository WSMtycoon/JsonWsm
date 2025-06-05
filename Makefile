# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -I.
LDFLAGS = 

# Directories
SRC_DIR = Source
TEST_DIR = Test/Source
UTILS_DIR = Utils/Source
BIN_DIR = bin
OBJ_DIR = Object

# Header files (interface-only)
HEADER_FILES = \
    $(SRC_DIR)/JsonType.h \
    $(SRC_DIR)/JsonBlock.h

# Source files in correct compilation order
PARSER_SRCS = \
    $(SRC_DIR)/JsonValue.cpp \
    $(SRC_DIR)/JsonArray.cpp \
    $(SRC_DIR)/JsonObject.cpp \
    $(SRC_DIR)/JsonWsm.cpp

TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)
UTILS_SRCS = $(wildcard $(UTILS_DIR)/*.cpp)

# Object files
PARSER_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(PARSER_SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(TEST_SRCS))
UTILS_OBJS = $(patsubst $(UTILS_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(UTILS_SRCS))

# Executables
PARSER_LIB = $(BIN_DIR)/libjsonparser.a
TEST_EXE = $(BIN_DIR)/test_parser.exe
UTILS_EXE = $(BIN_DIR)/utils.exe

# Default target
all: clean_bin parser utils test

# Create directories if they don't exist
$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

# Clean only bin directory
clean_bin:
	rm -rf $(BIN_DIR)
	mkdir -p $(BIN_DIR)

# Clean everything
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

# Build parser library
parser: $(PARSER_LIB)

# Build utils
utils: $(UTILS_EXE)

# Build tests
test: $(TEST_EXE)

# Compile parser source files with dependencies
$(OBJ_DIR)/JsonValue.o: $(SRC_DIR)/JsonValue.cpp $(SRC_DIR)/JsonValue.h $(HEADER_FILES) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/JsonArray.o: $(SRC_DIR)/JsonArray.cpp $(SRC_DIR)/JsonArray.h $(SRC_DIR)/JsonBlock.h $(SRC_DIR)/JsonValue.h $(SRC_DIR)/JsonType.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/JsonObject.o: $(SRC_DIR)/JsonObject.cpp $(SRC_DIR)/JsonObject.h $(SRC_DIR)/JsonBlock.h $(SRC_DIR)/JsonValue.h $(SRC_DIR)/JsonType.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/JsonWsm.o: $(SRC_DIR)/JsonWsm.cpp $(SRC_DIR)/JsonWsm.h $(SRC_DIR)/JsonObject.h $(SRC_DIR)/JsonValue.h $(SRC_DIR)/JsonType.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test files
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile utils files
$(OBJ_DIR)/%.o: $(UTILS_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create parser library
$(PARSER_LIB): $(PARSER_OBJS) | $(BIN_DIR)
	ar rcs $@ $^

# Link utils executable
$(UTILS_EXE): $(UTILS_OBJS) $(PARSER_LIB) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $@

# Link test executable
$(TEST_EXE): $(TEST_OBJS) $(PARSER_LIB) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $@

.PHONY: all clean clean_bin parser utils test 