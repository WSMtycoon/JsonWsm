CXX = g++
CXXFLAGS = -std=c++17 -I.
LDFLAGS = 

# Directories
SRC_DIR = Source
TEST_DIR = Test/Source
BIN_DIR = bin
OBJ_DIR = Object

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TEST_PARSER_OBJ = $(OBJ_DIR)/test_parser.o
TEST_STRUCT_OBJ = $(OBJ_DIR)/test_struct.o

# Executables
TEST_PARSER_EXE = $(BIN_DIR)/test_parser.exe
TEST_STRUCT_EXE = $(BIN_DIR)/test_struct.exe

# Default target
all: clean_bin $(TEST_PARSER_EXE) $(TEST_STRUCT_EXE)

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

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test files
$(OBJ_DIR)/test_parser.o: $(TEST_DIR)/test_parser.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/test_struct.o: $(TEST_DIR)/test_struct.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link test_parser executable
$(TEST_PARSER_EXE): $(TEST_PARSER_OBJ) $(OBJS) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $@

# Link test_struct executable
$(TEST_STRUCT_EXE): $(TEST_STRUCT_OBJ) $(OBJS) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $@

.PHONY: all clean clean_bin
