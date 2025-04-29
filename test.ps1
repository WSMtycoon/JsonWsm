# PowerShell script to run tests on all .txt files in the test directory

# Get the directory where the script is located
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# Path to the executable
$executablePath = Join-Path $scriptDir "bin\Json_test.exe"

# Check if the executable exists
if (-not (Test-Path $executablePath)) {
    Write-Error "Executable not found at: $executablePath"
    exit 1
}

# Get all .txt files in the test directory
$testFiles = Get-ChildItem -Path (Join-Path $scriptDir "Test") -Filter "*.txt"

if ($testFiles.Count -eq 0) {
    Write-Error "No test files found in the Test directory"
    exit 1
}

# Run tests on each file
foreach ($file in $testFiles) {
    $filePath = $file.FullName
    Write-Host "`n==================================================="
    Write-Host "Running test on file: $($file.Name)"
    Write-Host "===================================================`n"
    
    # Run the executable with the file path as an argument
    & $executablePath $filePath
    
    # Check if the command was successful
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Test failed for file: $($file.Name)"
    }
}

Write-Host "`nAll tests completed."