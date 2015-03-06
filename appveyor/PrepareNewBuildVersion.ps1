$fileAppveyor = "appveyor.yml"
$fileVersionHeader = "src/VersionInfo.h"

$regexFileAppveyor = "version: (\d+)\.(\d+)\.(\d+)\.\{build\}"
$regexFileVersionHeader = "#define CURRENT_VERSION "".+"""

if ($env:APPVEYOR_REPO_TAG -eq "true") {
    $match = select-string -Path $fileAppveyor -Pattern $regexFileAppveyor

    $newVersion = $match.Matches[0].Groups[1].Value + "." + $match.Matches[0].Groups[2].Value + "." + ([convert]::ToInt32($match.Matches[0].Groups[3].Value, 10) + 1)
    Write-Host "Prepare for next version automatically: $newVersion" -ForegroundColor "Yellow"

    Write-Host "  - Apply to $fileAppveyor" -ForegroundColor "Yellow"
    (gc $fileAppveyor) -replace $regexFileAppveyor,"version: $newVersion.{build}" | Set-Content $fileAppveyor

    Write-Host "  - Apply to $fileVersionHeader" -ForegroundColor "Yellow"
    (gc $fileVersionHeader) -replace $regexFileVersionHeader,"#define CURRENT_VERSION ""$newVersion-dev""" | Set-Content $fileVersionHeader

    Write-Host "  - Commit and push to GitHub repository" -ForegroundColor "Yellow"
    git config --global credential.helper store
    git config --global user.name $env:APPVEYOR_REPO_COMMIT_AUTHOR
    git config --global user.email $env:APPVEYOR_REPO_COMMIT_AUTHOR_EMAIL
    Add-Content "$env:USERPROFILE\.git-credentials" "https://$($env:access_token):x-oauth-basic@github.com`n"
    git remote add github "https://github.com/$($env:APPVEYOR_REPO_NAME).git"
    git checkout -q $env:APPVEYOR_REPO_BRANCH
    git add -q $fileAppveyor $fileVersionHeader
    git commit -q -m "[AppVeyor] Prepare for version $newVersion [ci skip]"
    git push -q github master
} else {
    Write-Host "No tag has been pushed; skip preparing for a new version" -ForegroundColor "Yellow"
}
