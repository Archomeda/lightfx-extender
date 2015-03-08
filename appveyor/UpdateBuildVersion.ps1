$file = "src/VersionInfo.h"

$version = $env:APPVEYOR_BUILD_VERSION
if ($env:APPVEYOR_REPO_TAG -eq "false") {
    $postfix = "dev"
    if ($env:APPVEYOR_PULL_REQUEST_NUMBER) {
        $postfix = "$postfix-PR$($env:APPVEYOR_PULL_REQUEST_NUMBER)"
    } elseif ($env:APPVEYOR_REPO_BRANCH -ne "master") {
        $postfix = "$postfix-$($env:APPVEYOR_REPO_BRANCH)"
    }

    if (!($version -like "*-$postfix")) {
        $version = "$version-$postfix"
    }
}

Write-Host "AppVeyor build version: $($env:APPVEYOR_BUILD_VERSION)" -ForegroundColor "Yellow"
Write-Host "Custom build version: $version" -ForegroundColor "Yellow"

if ($version -ne $env:APPVEYOR_BUILD_VERSION) {
    Write-Host "  - Apply to $file" -ForegroundColor "Yellow"
    (gc $file) -replace "#define CURRENT_VERSION "".+""","#define CURRENT_VERSION ""$version""" | Set-Content $file

    Write-Host "  - Send to AppVeyor Build Worker API" -ForegroundColor "Yellow"
    Update-AppveyorBuild -Version $version
}
