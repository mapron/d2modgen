## 0.6.7
Updated stormlib and Casclib, recent version of Casclib contains fix for Steam version of D2R. Thank you, Ladislav Zezula!

Small feature: added support for new RotW attributes (like magic pierce and Warlock skills) in rando.


## 0.6.6

New Features:

* Added compatibility checkbox on main page to create mod for D2R Pre-RotW expansion.
* Added Horadric cube recipe option to make cheat small charms.  

Here are all Small Charm cheat recipes (SC + X + Y). You should be able to craft them on level 1. If you get too high lvl req, just re-craft again.

* key + stamina = all skills + all stats
* key + antidote = damage reduction
* key + thawing = absorb
* key + any hp = HP and vitality
* key + any mana = Mana and energy
* TP + stamina = faster frames
* TP + antidote = max res + poison len
* TP + thawing = all res
* TP + any hp = hp regen
* TP + any mana = mana regen
* ID + stamina = pierce res
* ID + antidote = +elemental dmg
* ID + thawing = OW/CB/DS
* ID + any hp = IAS and Enh Dmg
* ID + any mana = attack rating and defence


## 0.6.5
Fixed some issues were reported with newer D2R installation (inability to read CASC storage). CASC library was updated to fresh master.

## 0.6.4
Added Korean translation. Thanks to 반졔 (Ban)!

## 0.6.3
Added version to window's title.

## 0.6.2
Tiny tech release: reduced install zip size from 16 to 10 megabytes (removed unnecessary dlls)

## 0.6.1
Fixed a bug related to ProjectD2 mpq load process.

## 0.6.0

Main change of new version is 4 pre-made overall config preset to start:
* basic;
* friendly_chaos;
* magic_rando;
* oh_no

[Read more about them](presets/readme.txt)


## 0.5.9
Small bugfixes to support current ProjectD2 update in Randomizer.

## 0.5.8
Changes:
* Removed "enable Sunder Charms" feature as it no needed anymore
* Huge internal code refactor (some regressions in functionality can happen).

## 0.5.7
Various bug fixes:
* Fixed "Bug: Always max defense armor from Perfect rolls doesn't work."
* Fixed missing change notification on D2 legacy path edit (that's why it could not be edited)
* Fixed pregen step as 'csv folder' no working
* Set partial bonuses are no more greedy in generation with randomizer (there was a few of them, now much more)

## 0.5.6
Small bugfixes:
* Added option to disable XP penalty.
* Improve set partial bonuses. Now they can be added in rando when initial item did not have them.
* Added feature 'Multiple sets copies'. This will create separate sets with number in the end, like 'Sigons Complete Steel 3', 'Angelic Wings 6' etc.
* Added separate slider for number of properties for sets in rando
* Fixed crash with CSV pregen, if there was slash at the end of folder path.
* Auto create item-names translations so user can have extra custom sets.

## 0.5.5
Just another fix for D2R 2.5 patch (updated 'monstat' column aliases - thx blizzard mentioning this in changelog.)

## 0.5.4
Release with support of new Sunder Charms introduced in D2R 2.5:
* New Sunder properties correctly randomized;
* Minus res do not convert to +1% res for some charms;
* Added QoL checkbox to enable drops of Sunder Charms anywhere (check zones with TC75 and above).

## 0.5.3
Main 2 changes related to better bug diagnostics:
* Added exception handling to generation phase so you getting message like 'Generation failed in step 'randomizer' with error X' instead of crash;
* Added exception to common error (missing columns) so you again getting nice dialog instead of crash, reporting 'unknown column 'blah' in table 'runes'

## 0.5.2
Various bugfixes related to pre-gen step:
* Data change in 'D2R mod' combo works correctly;
* Improved json merge (e.g. if mod have some item.json changes and modgen have them too);
* Added workaround for column 'worldevent' renamed to 'diablocloneweight' in 2.4.
* Added couple options: 'Quest drops' and 'Show item level'

## 0.5.1
Couple bug fixes after 0.5:
* fixed random seed not restored;
* fixed random seed not applied on 'create new seed' action;
* fixed monsters HP slider was invisible on Monster Stat tab;
* fixed resistance penalty was applying with wrong sign (+100% instead of -100% for example)

## 0.5
New features:
* Added New tab - Requirements!
* Now you can play around with level requirements and get powerful items early.
* Added 'max hard skill points' option into Character tab. Now you can dump 60 hard points into Blizzard.
Noticeable changes: 
* Complete overhaul of modular engine (so bad things can happen, report if something broken).  
* NB: random seed is not compatible with previous versions. You will get different results with same seed from 0.4.1.

## 0.4.1
New features:
* Added New Randmoizer tab - Skills Randomizer!
* Skills Randomizer allow you to shuffle skills within the class.
* Skills Randomizer allow you to change elemental type of damage randomly.
* Added new Perfect Rolls page, incorporating different 'perfect' options from other tabs.

## 0.4
New features:
* Added Undo action.
* Added quick presets feature (for now on three tabs).
* Added option to increase high-level drops.
* Added Rune Drop tab (to make some space on Item Drops).
* Improved 'Increase Rune drops' slider to be much more smooth.
* Randomizer can now accept custom properties list to randomize.
Bug fixes:
* Fixed a bug when slider value will slowly creep down on eash program start.
* Fixed 'perfect roll' not perfect rolling some things.

## 0.3.4
Bug fixes:
* Fixed crashes when generating randomizer for D2 legacy;
* Fixed work with attributes for PD2 - not PD2-specific not thrown away when work with randomizer;
* Also splash modifier always kept when it present (not randomized).

## 0.3.3
New features:
* Options to tune monster resistances;
Bug fixes:
* fixed XP slider for stats not working.


## 0.3.2
New Features:
* Rework of randomizer tab. Prop sliders are removed, added tow new relative count sliders.
Bug fixes:
* Fixed randomizer being unreliable with balance + item fit.

## 0.3.1
New Features:
* Randomizer now support 'addition only' mode for properties.
* New Horadric Cube cheese added.
* Added sliders to tweak mercenary HP/dmg

Bug fixes:
* Fixed various tiny config or generation erorrs (not important for user experience).


## 0.3
New features:  
* Post-gen and pre-gen steps now a more powerful replacement than 'mod merge' - you can merge different mods and resolve conflicts automatically!
* Overall UI rework;
* added Light/Dark theme support;
* ability to change language (English/Russian for now);
* disable/enable tab on panel switch;
* added context help buttons, documentation moved from Readme.MD to UI context (nobody read github docs);
* added commandline support;

## 0.2.7
New features:  
* Eeach tab have separate enable control and reset button;
* Randomizer introduces new option: fit properties to item type. Now you can select percent, how much you wish to new generated items resemble old item classes (rings - old rings, belts - old belts etc).

## 0.2.6
Bug fixes:  
* Fixed crashes when generate for legacy D2 and ProjectD2.

## 0.2.5

Critical fix: fixed bug when D2 legacy dir was wiped out.

## 0.2.4
New Features:  
* Added new "Challenge" tab to allow extra resistance penalty for character;
* Perfect rolls option now tweak automods too.
* Added option to skip knockback/monster flee affix.  

Bug fixes:  
* Item randomizer now respect some properties to have sense when spawn: sockets, durability options, throwable options will now spawn on items that support them.
* Improved support for partial set bonuses on items for Randomizer (they were ignored previously).  

Documentation:  
*  Updated docs to include latest features;


## 0.2.3
New Features:  
* New feature: mod merge tab; allows to include other mods contents on generate;
* Reworked item filters - instead modifying droptables, now it shortens labels or hide them, items still available for pickup.  

Bug fixes:  
* Disabled buggy charm gambling;
* Quest items do not randomize their stats now;
* Monster randomiser is rewritten form scratch; more balanced spawns, minions, skills and TCs now fixed;

## 0.2.2
New Features:  
* Added Monster Randomizer! 
* Gambling tuning experimental support (still have quirks);
* Add quest skip recipes (fast creation of Tal staff and Khalim Will);
* Added character tab, to allow add Horadric cube, change stats per level or lower overall requirement;
* New 'Quality of life' tab, change stacks/books size, remove Unique charm limit, enable Teleport in town, or reduce repair costs!  

Bug fixes:  
* Fixed issue that 'unbalanced' (=99) mode was not 'crazy enough';

## 0.2.1
New Features:  
* Added Horadric cube tab with a few recipes handy for testing purpose;
* Option to make uniqes with same base equal rarity (for randomizer that does not make sense anyway)
* Perfect roll now a thing in non-randomized mode.
* New density option.
* Workaround for lack of loot filter - pseudofilter done through discarding drops.
* Option to replace +skills and charges to oskills.  

Bug Fixes:  
* Fixed issue with spawning "defence on time" affix.


## 0.2
New features:  
* Added Legacy D2 support;
* Improved options for Drop rates;
* Added ability to generate different uniques.

## 0.1.1

Patch release with minor UI improvements:  
* Added function to create shortcut;
* Sliders now more reliable;
* Some styles enhancement.

## 0.1
First public release, features included:  
* Monster balance;
* Density adjust;
* Drops increase;
* Item prop randomization.
