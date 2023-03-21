# GTA2 Frontend Fix:
FrontendFix is a plugin that enhances, improves and makes you enjoy GTA2 on modern machines.

## Compatibility:
Currently compatible only with GTA2 9.6.0.0

## Features: 
 #### UI changes:
 - New settings menu page based on the PlayStation release of the game.
 - PlayStation like loading screen.
 - PlayStation like pause menu.
 - Restored console background music when returning to main menu.

#### Improvements
 - Full mouse support (in game and in menu).
 - Ditches gta2manager.exe and settings from the registry in favor of a formal xml file.
 - Ability to control Player Heading through Mouse.
 - Native support for Fullscreen, Windowed and Borderless Windowed mode.
 - 32bit resolutions.
 - Multi5 language support.

## Screenshots:
<p align="center">
<img src="https://i.imgur.com/6wTlCpk.png" width="320" height="180">
<img src="https://i.imgur.com/nnLwhsW.png" width="320" height="180">
<img src="https://i.imgur.com/zyeeMrQ.png" width="320" height="180">
<img src="https://i.imgur.com/qD9qDJx.png" width="320" height="180">
<img src="https://i.imgur.com/kIooq03.png" width="320" height="180">
</p>

## Compiling:
Requirements:
 - Visual Studio 2019
 - [Plugin SDK](https://github.com/DK22Pac/plugin-sdk)
 
#### Submodules needs to be updated in order to build this project, run the following command after cloning the repository:
 ```
git submodule update --init --recursive
```

## Download:
Download the latest archive from the [releases](https://github.com/gennariarmando/gta2-frontend-fix/releases) page.

# Installation:
#### Installing an ASI Loader:
An ASI Loader is required in order to inject the plugin into the game, if you already have one skip to the next step.\
Recommended: [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader)

Place the DLL file (renamed into "dinput.dll") into your GTA2 directory.

Requires [Widescreen Fix](https://thirteenag.github.io/wfp#gta2)

#### Installing FrontendFix:
Archive content: 
- FrontendFixII.asi
- FrontendFixII.ini
- Data\GTAudio\M.wav

Paste the content of the "data" folder into your GTA2 "data" directory.\
Create a folder called "scripts" inside your GTA2 directory and paste both FrontendFixII.asi and FrontendFix.ini in it.

## Links:
- [plugin-sdk](https://github.com/DK22Pac/plugin-sdk)
- [pugixml](https://github.com/zeux/pugixml)
