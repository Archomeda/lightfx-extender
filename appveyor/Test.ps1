if ($env:PLATFORM -eq "x86" -or $env:PLATFORM -eq "x64") {
    vstest.console /Platform:$(env:PLATFORM) /logger:Appveyor bin\Release_Tests\$(env:PLATFORM)\UnitTests.dll
}
