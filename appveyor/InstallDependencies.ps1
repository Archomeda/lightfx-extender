if ($env:PLATFORM -eq "release") {
    # Install Inno Setup only in release mode
    Write-Host "Installing Inno Setup..." -ForegroundColor "Yellow"
    Invoke-WebRequest "http://www.jrsoftware.org/download.php/is-unicode.exe" -OutFile InnoSetup-Installer.exe
    if ($LastExitCode -ne 0) exit $LastExitCode
    ./InnoSetup-Installer.exe /SP- /VERYSILENT /SUPPRESSMSGBOXES /NORESTART
    if ($LastExitCode -ne 0) exit $LastExitCode
    $env:PATH = "$($env:PATH);C:\Program Files (x86)\Inno Setup 5"
}