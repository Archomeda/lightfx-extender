if ($env:PLATFORM -eq "release") {
    # Install Inno Setup only in release mode

    Invoke-WebRequest "http://www.jrsoftware.org/download.php/is-unicode.exe" -OutFile InnoSetup-Installer.exe
    ./InnoSetup-Installer.exe /SP- /VERYSILENT /SUPPRESSMSGBOXES /NORESTART
    $env:PATH = "$($env:PATH);C:\Program Files (x86)\Inno Setup 5"
}