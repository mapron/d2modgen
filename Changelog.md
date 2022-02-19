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
