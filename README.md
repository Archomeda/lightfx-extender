# LightFX Extender
[![Build status](https://ci.appveyor.com/api/projects/status/ual57rxoe7wf0pm3/branch/master?svg=true)](https://ci.appveyor.com/project/Archomeda/lightfx-extender/branch/master)

This plugin allows gamers to take advantage of LightFX (or AlienFX) features in
order to extend it to other hardware such as Logitech keyboards and Lightpack
devices. It also adds some small extra features for specific games that have one
or more extra APIs.

## Installation
Download the newest version of LightFX Extender from the releases tab and unzip
`LightFX.dll` to the game folder where you want to have LightFX Extender. Upon
starting that game, LightFX Extender should load up and place itself in the
notification area next to the clock on your taskbar. Right-click on the icon and
enable the devices that you want to have LightFX Extender support for.

For advanced configuration, please check the file `settings.json` in the
configuration folder (`%APPDATA%\LightFX Extender`) or right-click on the icon
in your taskbar and choose *Open configuration folder*. This file will be
created after you launched and closed the game after installing LightFX Extender
for the first time. Deleting this file will cause the settings to be reset to
default. More information about the advanced configuration can be found on
[the wiki](https://github.com/Archomeda/lightfx-extender/wiki/Configuration).

### Updating
LightFX Extender has support for automatic updates. However, there is always a
possibility that this might fail. If this happens, just follow the instructions
under [Installation](#Installation). If you want to disable automatic updates,
open `settings.json` within the configuration folder (see
[Installation](#Installation)) and change the entry `AutoUpdatesEnabled` to
`false`.

## Support
### Hardware
- Logitech keyboards and mice with LEDs through Logitech Gaming Software
8.58.177 or higher
  - Version 8.57.145 is supported as well, but requires
[additional configuration](https://github.com/Archomeda/lightfx-extender/wiki/Logitech-Color-Range)
  - For version 8.58.177 and lower, G110 users have to enable
[a workaround](https://github.com/Archomeda/lightfx-extender/wiki/Logitech-G110-Workaround)
for a bug that will be fixed in an upcoming version
- Corsair keyboards and mice for both RGB and Monochrome (by @antonpup)
- Lightpack devices through a recent version of Prismatik (or similar hardware
and software that support the same API, tested on version 5.11.1 only)
- Alienware devices that originally support LightFX

### Games
Theoretically, every game that has support for LightFX, is supported by LightFX
Extender. But it's possible that some games don't work nice with it, so if you
found a game that's not working with LightFX Extender, submit an issue. The
tested games are:
- Dying Light (uses x64)
- Guild Wars 2
 - Extended feature: Change color based on your PvP/WvW team upon entering the
   map or switching PvP teams
 - The 64-bit client is not detecting the 64-bit build of LightFX Extender
   properly

*Unless otherwise specified, the games are x86 and therefore use the x86 build
of LightFX Extender.*

There is an [external community maintained list](http://alienfx.cyanlabs.net/)
available that aims to provide detailed information about games that support
LightFX and LightFX Extender. Please note that, although I was indirectly
involved in its creation, it's not made by me and therefore not supported by me.
Please do not report any issues regarding that list here, but go to the website
instead.

## Contribute
If you want to contribute to the project, make a fork, change stuff and do a
pull request. Submitting issues is also welcome.

Example contributions that come to mind are:
- Extending support to more hardware
- Extending support for games that have (an) extra API(s) at their disposal
(like Guild Wars 2)
- Bugfixes if you encounter a bug
