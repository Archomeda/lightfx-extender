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

Write-Host "Update build version to $version" -ForegroundColor "Yellow"
(gc $file) -replace "#define CURRENT_VERSION ""0.0.0""","#define CURRENT_VERSION ""$version""" | Set-Content $file
Update-AppveyorBuild -Version $version
