Function ApplyVersion($file, $regex, $replace, $encoding = "ASCII") {
    (Get-Content $file -encoding $encoding) -replace $regex,$replace | Set-Content $file -encoding $encoding
}

Function ApplyVersionToVersionInfoH($version) {
    $regex = '(#define CURRENT_VERSION ").+(")'
    $replace = "`${1}$version`${2}"
    ApplyVersion -file "src\VersionInfo.h" -regex $regex -replace $replace
}

Function ApplyVersionToAppVeyorYml($version) {
    $regex = "(version: ).+"
    $replace = "`${1}$version"
    ApplyVersion -file "appveyor.yml" -regex $regex -replace $replace
}

Function ApplyVersionToResource($version) {
    $versionText = $version
    $version = $version.split("-")[0] -replace "\.",","
    $regex = "(FILEVERSION ).+"
    $replace = "`${1}$version"
    ApplyVersion -file "src\LightFXExtender.rc" -regex $regex -replace $replace -encoding "Unicode"

    $regex = "(PRODUCTVERSION ).+"
    $replace = "`${1}$version"
    ApplyVersion -file "src\LightFXExtender.rc" -regex $regex -replace $replace -encoding "Unicode"

    $regex = "(VALUE ""FileVersion"", "").+("")"
    $replace = "`${1}$versionText`${2}"
    ApplyVersion -file "src\LightFXExtender.rc" -regex $regex -replace $replace -encoding "Unicode"

    $regex = "(VALUE ""ProductVersion"", "").+("")"
    $replace = "`${1}$versionText`${2}"
    ApplyVersion -file "src\LightFXExtender.rc" -regex $regex -replace $replace -encoding "Unicode"
}
