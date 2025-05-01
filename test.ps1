# Script for testing JsonWSM
# Usage: .\test.ps1 [file_path]
# If no path is specified, all .txt files in the Test folder will be tested
# If a path is specified, only the specified file will be tested

# Get file path from parameter
param(
    [string]$FilePath = ""
)

# Define path to Test folder
$testPath = Join-Path $PSScriptRoot "Test"

# Define path to executable file
$executablePath = Join-Path $PSScriptRoot "bin\json_test.exe"

# Check if executable file exists
if (-not (Test-Path $executablePath)) {
    Write-Host "Error: Executable file not found: $executablePath" -ForegroundColor Red
    exit 1
}

# If no path is specified, test all .txt files in the Test folder
if ($FilePath -eq "") {
    Write-Host "Testing all .txt files in the Test folder..." -ForegroundColor Green
    
    # Get list of all .txt files in the Test folder
    $testFiles = Get-ChildItem -Path $testPath -Filter "*.txt"
    
    if ($testFiles.Count -eq 0) {
        Write-Host "Error: No .txt files found in the Test folder" -ForegroundColor Red
        exit 1
    }
    
    # Test each file
    foreach ($file in $testFiles) {
        $fullPath = $file.FullName
        Write-Host "Testing file: $($file.Name)" -ForegroundColor Cyan
        
        & $executablePath $fullPath
        
        if ($LASTEXITCODE -ne 0) {
            Write-Host "Test for file $($file.Name) failed!" -ForegroundColor Red
        } else {
            Write-Host "Test for file $($file.Name) completed successfully!" -ForegroundColor Green
        }
        
        Write-Host "----------------------------------------" -ForegroundColor Yellow
    }
} else {
    # Define full path to file
    $fullPath = ""
    
    # Check if path is absolute
    if ([System.IO.Path]::IsPathRooted($FilePath)) {
        # If path is absolute, use it as is
        $fullPath = $FilePath
    } else {
        # If path is relative, add path to Test folder
        $fullPath = Join-Path $testPath $FilePath
    }
    
    # Check if file exists
    if (-not (Test-Path $fullPath)) {
        Write-Host "Error: File not found: $fullPath" -ForegroundColor Red
        exit 1
    }
    
    Write-Host "Testing file: $fullPath" -ForegroundColor Green
    
    # Run test with specified file
    & $executablePath $fullPath
    
    # Check execution result
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Test failed!" -ForegroundColor Red
        exit 1
    } else {
        Write-Host "Test completed successfully!" -ForegroundColor Green
    }
} 