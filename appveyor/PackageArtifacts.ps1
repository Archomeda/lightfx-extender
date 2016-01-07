if ($env:PLATFORM -eq "x86" -or $env:PLATFORM -eq "x64") {
    7z a "artifacts\LightFX-Extender-$($env:PLATFORM)_v$($env:APPVEYOR_BUILD_VERSION).zip" "$($env:APPVEYOR_BUILD_FOLDER)\bin\Release\$($env:PLATFORM)\*.*"
} elseif ($env:PLATFORM -eq "release") {
    New-Item -ItemType Directory -Force -Path artifacts\work\bin | Out-Null
    New-Item -ItemType Directory -Force -Path artifacts\work\x86 | Out-Null
    New-Item -ItemType Directory -Force -Path artifacts\work\x64 | Out-Null

    Copy-Item bin\Release\x86\*.dll artifacts\work\bin\
    Move-Item artifacts\work\bin\LightFX.dll artifacts\work\x86\
    Copy-Item bin\Release\x64\*.dll artifacts\work\bin\
    Move-Item artifacts\work\bin\LightFX.dll artifacts\work\x64\

    7z a -r "artifacts\LightFX-Extender-x86_v$($env:APPVEYOR_BUILD_VERSION).zip" "$($env:APPVEYOR_BUILD_FOLDER)\artifacts\work\x86\*.*"
    if ($LastExitCode -ne 0) exit $LastExitCode
    7z a -r "artifacts\LightFX-Extender-x64_v$($env:APPVEYOR_BUILD_VERSION).zip" "$($env:APPVEYOR_BUILD_FOLDER)\artifacts\work\x64\*.*"
    if ($LastExitCode -ne 0) exit $LastExitCode
    7z a -r "artifacts\LightFX-Extender-full_v$($env:APPVEYOR_BUILD_VERSION).zip" "$($env:APPVEYOR_BUILD_FOLDER)\artifacts\work\*.*"
    if ($LastExitCode -ne 0) exit $LastExitCode
    ISCC appveyor\BuildInstaller.iss
    if ($LastExitCode -ne 0) exit $LastExitCode
    Remove-Item -Recurse -Force artifacts\work
}
