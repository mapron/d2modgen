
# How it looks like
![density](game/density.png "Density")  
High density setting in Lost City  
![rando_gem](game/rando_gem.png "rando_gem")  
Random affixes set on gem with Randomizer  
![rando_rune](game/rando_rune.png "rando_rune")  
That's and Eld rune I always wanted to see!  
![rando_rare](game/rando_rare.png "rando_rare")  
Nice boots i found in Cold Plains Normal!  
![rando_unique](game/rando_unique.png "rando_unique")  
Another GG find, Pelta Lunata! Trying to use balanced drops next time...  
![drops_max](game/drops_max.png "drops_max")  
A lot of features enabled here - more Boss drops, increased Uniques/drop count/Rune drops etc. Almost close to D3 drop count (please, dont go that far!)  

# Detailed description how everything works
1. Game (D2 and D2R) stores a lot of properties/balance of game content in tables - imagine an Excel spreadsheet or database table inside the game. One way to modify this data - provide a set of txt files (which are CSV - tab-separted). Starting with D2R, that can be done in a very simple way - creating a mod subfolder, so custom mod no not override the main game. d2modgen utilizes this feature.
2. Main game data located in "D2R folder", mod in "D2R folder/mod/modname/modname.mpq/"; save files stored in "User folder/Saved Games", mod save files (and games settings) stored in  "User folder/Saved Games/mods/modname". You can easily transfer your data between modded and non-modded versions (for example, shared storage).
3. To run the game in modded version, you need to provide command-line option, like "-mod rando -txt" will run D2R with mod "rando" enabled.

# Feature tabs
For each tab, just click or hover over question button (?) to get help about features on each page.  
Also, as you noticed, support for Dark and Light theme is available - choose what you prefer in Actions -> Theme.

#### Randomize items  
![Randomize items](rando/items.png)  

#### Randomize monsters  
![Randomize monsters](rando/monsters.png)  

#### Randomize some skills  
![Randomize skills](rando/skills.png)  


#### Balance items
![Balance items](balance/items.png)  

#### Balance monsters
![Balance monsters](balance/monsters.png)  

#### Balance skills
![Balance skills](balance/skills.png)  


#### Balance map areas  
![Balance areas](balance/areas.png)  

#### Balance character
![Balance character](balance/character.png)  

#### Easier Gambling
![Balance Gambling](balance/gambling.png)  


#### Drop rates - items
![Drop rates - items](droprate/items.png)  

#### Drop rates - runes
![Drop rates - runes](droprate/runes.png)  

#### Drop rates - misc
![Drop rates - misc](droprate/misc.png)  


#### Cube recipes to blaze through  
![Horadric cube](other/cube.png)  


#### Quality of life
![Quality of life](other/qol.png)  


#### Expanded inventory
![Expanded inventory](other/inventory.png)  


# Using commandline
You also can run d2modgen in commandline mode (without UI). `bin/modgen_console.exe`   
will generate output using default application config (in `%LOCALAPPDATA%`).  
You can create script/batch file/global hotkey command that runs this executeable, and it will generate and update mod files.  
If you want to have new seed on every run, use "Create new seed on every Generate call" checkbox.  

# KnownIssues
1. If there is many dead bodies on the screen, some portals won't open (e.g. near Summoner). Workaround - go far away to another location, wait while corpses disappeared and then trigger the portal.
2. You need to completely quit out of the game for Diablo2:Resurrected, for mod changes to apply. In Diablo2 legacy (LoD/PD2), however, you can just reload save again without quitting.
