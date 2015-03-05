$file = "appveyor.yml"

if ($env:APPVEYOR_REPO_TAG -eq "true") {
    $regex = "version: (\d+)\.(\d+)\.(\d+)\.\{build\}"
    $match = select-string -Path $file -Pattern $regex

    $newVersion = $match.Matches[0].Groups[1].Value + "." + $match.Matches[0].Groups[2].Value + "." + ([convert]::ToInt32($match.Matches[0].Groups[3].Value, 10) + 1)
    Write-Host "Prepare for next version automatically: $newVersion" -ForegroundColor "Yellow"

    Write-Host "  - Apply to appveyor.yml" -ForegroundColor "Yellow"
    (gc $file) -replace $regex,"version: $newVersion.{build}" | Set-Content $file

    Write-Host "  - Commit and push to GitHub repository" -ForegroundColor "Yellow"
    git config --global credential.helper store
    git config --global user.name $env:APPVEYOR_REPO_COMMIT_AUTHOR
    git config --global user.email $env:APPVEYOR_REPO_COMMIT_AUTHOR_EMAIL
    Add-Content "$env:USERPROFILE\.git-credentials" "https://$($env:access_token):x-oauth-basic@github.com`n"
    git remote add github "https://github.com/$($env:APPVEYOR_REPO_NAME).git"
    git checkout $env:APPVEYOR_REPO_BRANCH
    git add appveyor.yml
    git commit -m "[AppVeyor] Prepare for version $newVersion [ci skip]"
    git push github master
}
