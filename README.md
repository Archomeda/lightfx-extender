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
in your taskbar and choose *Open configuration folder*.

### Updating
LightFX Extender has support for automatic updates. However, there is always a
possibility that this might fail. If this happens, just follow the instructions
under *Installation*. If you want to disable automatic updates, open
`settings.json` within the configuration folder (see *Installation*) and change
the entry `AutoUpdatesEnabled` to `false`.

## Support
### Hardware
- Logitech keyboards and mice with LEDs through Logitech Gaming Software
8.57.145 or higher
- Lightpack devices through a recent version of Prismatik (or similar hardware
and software that support the same API)

### Extended game features
- Guild Wars 2 through the officially supported Mumble Link API (pulse color
based on your PvP/WvW team upon entering map or switching PvP teams)

## Contribute
If you want to contribute to the project, make a fork, change stuff and do a
pull request. Submitting issues is also welcome.

Example contributions that come to mind are:
- Extending support to more hardware
- Extending support for games that have (an) extra API(s) at their disposal
(like Guild Wars 2)
- Bugfixes if you encounter a bug
