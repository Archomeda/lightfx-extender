# LightFX Extender
[![Build status](https://ci.appveyor.com/api/projects/status/ual57rxoe7wf0pm3/branch/master?svg=true)](https://ci.appveyor.com/project/Archomeda/lightfx-extender/branch/master)

LightFX Extender allows gamers to take advantage of LightFX (or AlienFX) features in other hardware besides Alienware, such as Logitech keyboards and Lightpack devices.
It also adds some small extra features for specific games that have one or more extra APIs.

## Installation
Go to releases, find the latest version and download the correct architecture (x86 for 32-bit and x64 for 64-bit) for the game you want to use LightFX Extender for.
After downloading, extract LightFX.dll and CUESDK_2013.dll (for 32-bit) or CUESDK.x64_2013.dll (for 64-bit) to the game folder.

When you start the game, LightFX Extender should be loaded automatically with the game.
Sometimes you have to enable LightFX or AlienFX in the game first; this varies for each game.
If LightFX Extender is loaded, you should see an icon in the notification area next to the clock on your taskbar.
You can right-click on that icon in order to enable or disable various detected devices.
If LightFX Extender is started for the first time, it will automatically attempt to enable all supported devices.

For advanced configuration, please check the settings.json file in the configuration folder (%APPDATA%\LightFX Extender) or right-click on the icon in your taskbar and choose *Open configuration folder*.
If this file has not been created yet, it will automatically be created the first time you start and close the game.
Deleting this file will cause the settings to be reset to default.
More information about the advanced configuration can be found on [the wiki](https://github.com/Archomeda/lightfx-extender/wiki/Configuration).

### Updating
LightFX Extender has support for automatic updates.
However, there is always a possibility that this might fail.
If this happens, just follow the instructions under [Installation](#Installation).
If you want to disable automatic updates, open settings.json in the configuration folder (see [Installation](#Installation)) and change the entry `AutoUpdatesEnabled` to `false`.

## Support
### Hardware
- Alienware devices that originally support LightFX
- Corsair keyboards and mice for both RGB and Monochrome through Corsair Utility Engine 1.13.36 or higher (contributed by [@antonpup](https://github.com/antonpup))
- Lightpack devices through a recent version of Prismatik (or similar hardware and software that support the same API, tested on version 5.11.1 only)
- Logitech keyboards and mice with LEDs through Logitech Gaming Software 8.58.177 or higher
  - Version 8.57.145 is supported as well, but requires [additional configuration](https://github.com/Archomeda/lightfx-extender/wiki/Logitech-Color-Range)
  - For version 8.58.177 and lower, G110 users have to enable [a workaround](https://github.com/Archomeda/lightfx-extender/wiki/Logitech-G110-Workaround)
- Razer Chroma products through Synapse (contributed by [@antonpup](https://github.com/antonpup))

### Games
Theoretically, every game that has support for LightFX, is supported by LightFX Extender.
But it's possible that some games don't work well with it, so if you found a game that's not working with LightFX Extender, submit an issue.
The tested games are:
- Dying Light (64-bit)
- Guild Wars 2 (32-bit and 64-bit)
  - Extended feature: Change color based on your PvP/WvW team upon entering the map or switching PvP teams (currently only working for 32-bit)

*Unless otherwise specified, the games are x86 and therefore use the x86 build of LightFX Extender.*

There is an [external community maintained list](http://alienfx.cyanlabs.net/) available that aims to provide detailed information about games that support LightFX and LightFX Extender.
Please note that, although I was indirectly involved in its creation, it's not made by me and therefore not supported by me.
Please do not report any issues regarding that list here, but go to the website instead.

## Contribute
If you want to contribute to the project, make a fork, change stuff and do a pull request.
Submitting issues is also welcome.

Example contributions that come to mind are:
- Extending support to other hardware
- Extending support for games that have (an) extra API(s) at their disposal (like Guild Wars 2)
- Bugfixes if you encounter a bug
