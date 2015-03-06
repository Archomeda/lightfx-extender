$file = "src/VersionInfo.h"

$version = $env:APPVEYOR_BUILD_VERSION
if ($env:APPVEYOR_REPO_TAG -eq "false") {
    $version = "$version-dev"
    if ($env:APPVEYOR_PULL_REQUEST_NUMBER) {
        $version = "$version-PR$($env:APPVEYOR_PULL_REQUEST_NUMBER)"
    } elseif ($env:APPVEYOR_REPO_BRANCH -ne "master") {
        $version = "$version-$($env:APPVEYOR_REPO_BRANCH)"
    }
}

Write-Host "Current build version: $version" -ForegroundColor "Yellow"

Write-Host "  - Apply to $file" -ForegroundColor "Yellow"
(gc $file) -replace "#define CURRENT_VERSION ""\d+\.\d+\.\d+""","#define CURRENT_VERSION ""$version""" | Set-Content $file

Write-Host "  - Send to AppVeyor Build Worker API" -ForegroundColor "Yellow"
Update-AppveyorBuild -Version $version
