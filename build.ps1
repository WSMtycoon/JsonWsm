param(
    [Parameter()]
    [ValidateSet('parser', 'utils', 'test', 'all', 'clean')]
    [string]$Target = 'parser'
)

# Build the project
Write-Host "Building target: $Target"

switch ($Target) {
    'parser' {
        Write-Host "Building parser library..."
        make parser
    }
    'utils' {
        Write-Host "Building utils..."
        make utils
    }
    'test' {
        Write-Host "Building tests..."
        make test
    }
    'all' {
        Write-Host "Building all components..."
        make all
    }
    'clean' {
        Write-Host "Cleaning build files..."
        make clean
    }
}

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build successful."
} else {
    Write-Host "Build failed."
    exit 1
}


