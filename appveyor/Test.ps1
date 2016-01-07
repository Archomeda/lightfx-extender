if ($env:PLATFORM -eq "x86" -or $env:PLATFORM -eq "x64") {
    vstest.console /Platform:$env:PLATFORM /logger:Appveyor bin\Release_Tests\$env:PLATFORM\UnitTests.dll
    if ($LastExitCode -ne 0) { exit $LastExitCode }
} elseif ($env:PLATFORM -eq "release") {
    Write-Host "This job is in release mode; skip testing" -ForegroundColor "Yellow"
}
