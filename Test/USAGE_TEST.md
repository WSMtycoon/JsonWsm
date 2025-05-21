# JsonWSM Test Usage Guide

## Overview

This guide explains how to run and use the test suite for the JsonWSM JSON parser.

## Running Tests

### Prerequisites

- C++17 compatible compiler
- Make
- PowerShell (for build script)

### Building and Running Tests

1. Navigate to the project directory
2. Run the following commands:

```bash
# Build the project
./build.ps1

# Run tests
./test.ps1           # Run all tests
./test.ps1 struct    # Run structure test
./test.ps1 types     # Run data types test
./test.ps1 base    # Run nested objects test
./test.ps1 invalid   # Run invalid JSON test
./test.ps1 C:/TestPath/test.txt   # Run running test file
```

## Test Files

The test suite includes several JSON files in the `Test` directory:

### Structure Tests
- `test_struct.txt` - Tests basic JSON structure and formatting
- `test_invalid.txt` - Tests error handling for malformed JSON

### Data Type Tests
- `test_types.txt` - Tests all supported data types
- `test_float.txt` - Tests float type with precision limits
- `test_numeric.txt` - Tests numeric type with precision limits

### Nested Structure Tests
- `test_struct.txt` - Tests nested objects and arrays
- `test_base.txt` - Tests complex nested structures

### Special Cases
- `test_simbol.txt` - Tests UTF-8 character handling

## Test Output

When running the test executable, it will:

1. Display the input JSON file contents
2. Parse the JSON and show the structure
3. Print additional information about the JSON object:
   - Root object size
   - Whether the root object is empty
   - List of fields in the root object
   - Whether the JSON was parsed correctly (isCorrectly())

## Example Test Output

```bash
Input JSON:
{
  "name": "John",
  "age": 30,
  "active": true
}

Parsed JSON structure:
name                           (string): "John"
age                            (integer): 30
active                         (boolean): true

Additional Information:
Root object size: 3
Root object empty: no
Root object fields: name, age, active
Parsed correctly: yes
```

## Creating Custom Tests

To create your own test cases:

1. Create a new JSON file in the `Test` directory with a descriptive name:
   - `test_<category>.txt` for category tests
   - `test_<specific>.txt` for specific feature tests

2. Add a comment at the top of the file describing what the test does:
   ```json
   # Test file for specific feature
   # This test verifies...
   {
     "test": "data"
   }
   ```

3. Run the test:
   ```bash
   ./test.ps1 <category>
   ```

## Troubleshooting

If you encounter issues:

1. Ensure all prerequisites are installed
2. Check that the JSON file is valid
3. Verify the file path is correct
4. Check for any compilation errors in the build output
5. Use the isCorrectly() method to verify JSON structure
