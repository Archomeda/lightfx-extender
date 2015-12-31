. .\appveyor\ApplyVersion.ps1

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

Write-Host "Current build version: $version" -ForegroundColor "Yellow"

if ($version -ne $env:APPVEYOR_BUILD_VERSION) {
    Write-Host "  - Send to AppVeyor Build Worker API" -ForegroundColor "Yellow"
    Update-AppveyorBuild -Version $version
} else {
    Write-Host "  - AppVeyor build version is already up-to-date" -ForegroundColor "Yellow"
}

Write-Host "  - Apply to src\LFXE\VersionInfo.h" -ForegroundColor "Yellow"
ApplyVersionToVersionInfoH $version

Write-Host "  - Apply to src\LFXE\LightFXExtender.rc" -ForegroundColor "Yellow"
ApplyVersionToResource $version
