$file = "src/VersionInfo.h"

Write-Host "Push build version $($env:APPVEYOR_BUILD_VERSION) to $file" -ForegroundColor "Yellow"
(gc $file) -replace "#define CURRENT_VERSION ""0.0.0""","#define CURRENT_VERSION ""$($env:APPVEYOR_BUILD_VERSION)""" | Set-Content $file
