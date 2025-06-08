# PowerShell script for running JSON parser tests
# If no path is specified, all .txt files in the Test folder will be tested
# If a path is specified, only the specified file will be tested

# Get file path from parameter
param( [string]$testPath = "" )

# Function to run a single test
function Run-Test {
    param($testFile)
    # Run the test executable with the test file
    ./bin/parser.exe $testFile
}

# Main execution
if ($testPath -eq "") {
    # Case 1: No parameters - run all tests in Test folder
    Write-Host "Running all tests in Test folder...`n"
    $testFiles = Get-ChildItem -Path "Test/Json" -Filter "test_*.json"
    foreach ($file in $testFiles) {
        Run-Test $file.FullName
    }
}
elseif ($testPath -match "^(\./|\.\./|[A-Za-z]:\\)") {
    # Case 2: Full path specified (starts with ./ or ../ or drive letter)
    if (Test-Path $testPath) {
        Run-Test $testPath
    }
    else {
        Write-Host "Error: File not found: $testPath"
    }
}
else {
    # Case 3: Test name or number specified
    $testFile = "Test/Json/test_$testPath.json"
    if (Test-Path $testFile) {
        Run-Test $testFile
    }
    else {
        Write-Host "Error: Test file not found: $testFile"
        Write-Host "Available test files in Test folder:"
        Get-ChildItem -Path "Test/Json" -Filter "test_*.json" | ForEach-Object {
            Write-Host "  $($_.Name)"
        }
    }
} 