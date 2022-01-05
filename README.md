# Diablo 2 mod generator

Generator is inspired by [d2modmaker](https://github.com/tlentz/d2modmaker). It provides fast and easy way to create mod without any modding knowledge.  
Features included:  
- GUI-based desktop application with "one click and go" solution;
- Ability to adjust monster stats;
- Tune options to increase Set/Unique/Rare drops;
- Increase monster density and boss pack count to insane numbers;
- Last but not least, item randomizer - switch magic properties between Uniques, Runewords, Sets, Magic/Rare and even gems and runes!

# Quick start
![mainwindow](docs/mainwindow.png "Main Window")  
Main Window  
1. Download zip archive from [releases/tag/0.1](releases/tag/0.1);
2. Extract wherever you want and go inside extracted folder;
3. Run modgen.exe (see the screenshot)
4. Check if "D2R path" is filled correctly; if not, edit it manually to point "Diablo II Resurrected/" folder;
5. Optional; press "Copy Settings.json" to initialize new mod with your default game settings (music volume etc)
6. Optional and recommended; close Battlenet client, press "Set launch to mod" to setup game launch in modded offline mode. You need to do it once for specific mod.
7. Launch Battlenet; if you skiped previous step, go to "Settings near Play button" - "Game Settings" - "Diablo II Resurrected" - "Additional command line arguments" and paste string from "D2R commandline"
8. Press "Generate" button
9. Launch the game and go to offline mode. Create a new character and start.
10. If you done all correctly, character will have key in the inventory after creation.  
Now go and play around with other settings, click "Generate" and restart D2R again!  
Note: all you offline character still exist in non-modded save folder.  
Note 2: mod do not affect online play in any way, you won't get banned for it.  
Note 3: if you want to return to unmodded, press "Reset launch to unmodded" (again, BNet must be closed when you press this button).

# How it looks like

# Detailed description how everything works

# KnownIssues
1. If there is many dead bodies on the screen, some portals won't open (e.g. near Summoner). Workaround - go far away to another location, wait while corpses disappeared and then trigger the portal.
2. If charges spawned on a gem/rune, it will constantly update count on gem mouse hover. This glitch is fixes after gem is inserted in socket.
3. Game trying to render limited item label on the screen - if there are too many items, you will see only closest ones.
4. Some mods can be spawn on items that do not support them (like Replenish quantity on Amulet). (TODO - future releases)

# Q&A and Troubleshooting
If you encounter an issue (hang/crash) in generator itself, grab applog.txt (File->Browse to settings folder) (make sure to copy file just after the crash). Then create an issue and attach this file.  
NB: author do not respond for any issues in D2R itself (all on your own risk!). However, you can create an issue to address game problems if you know correct way to fix it.

# How to build from the source
Prerequisites:
1. **Qt 5.15 LTS**
2. **CMake 3.20**  (prior versions should be fine though).
3. **C++17 capable compiler**. Tested: MSVC 2019 16.11.  

If requirements are met, then go "cmake/build/build install". For windows platform, "build install" creates install in binary directory with all runtime dependencies.

# Contribute
1. follow clang-format rules in src/ directory;
2. ConfigPages.cpp is probably your main focus of interest for adding new rules.

# Used thirdparty products
- (MIT) CascLib (https://github.com/ladislav-zezula/CascLib) is used for Blizzard CASC data storage read;
- (Zlib) Zlib library used internally by CascLib.

# Third party components as non-distributed dependency
- Qt , https://www.qt.io/, GPL/LGPL/Commercial; components utilized by d2modgen are available under LGPL.

# License
Project source code is licensed under [MIT license](LICENSE).
