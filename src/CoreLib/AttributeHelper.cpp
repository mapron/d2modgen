/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "AttributeHelper.hpp"

#include <algorithm>
#include <unordered_map>
#include <vector>

#include <cassert>

namespace D2ModGen {

namespace {

const std::vector<AttributeDesc> s_attributes{
    AttributeDesc{ "ac", { AttributeFlag::Defense } }.setRange(-10, 2037),              // +# Defense
    AttributeDesc{ "ac-miss", { AttributeFlag::Defense } }.setRange(1, 511),            // +# Defense vs. Missile
    AttributeDesc{ "ac-hth", { AttributeFlag::Defense } }.setRange(1, 255),             // +# Defense vs. Melee
    AttributeDesc{ "red-dmg", { AttributeFlag::DamageReduction } }.setRange(1, 63),     // Damage Reduced by #
    AttributeDesc{ "red-dmg%", { AttributeFlag::Resistance } }.setRange(-25, 255),      // Damage Reduced by #%
    AttributeDesc{ "ac%", { AttributeFlag::Defense } }.setRange(1, 511),                // +#% Enhanced Defense
    AttributeDesc{ "red-mag", { AttributeFlag::DamageReduction } }.setRange(1, 63),     // Magic Damage Reduced by #
    AttributeDesc{ "str", { AttributeFlag::Stats } }.setRange(-32, 223),                // +# to Strength
    AttributeDesc{ "dex", { AttributeFlag::Stats } }.setRange(-32, 95),                 // +# to Dexterity
    AttributeDesc{ "vit", { AttributeFlag::Stats } }.setRange(-32, 95),                 // +# to Vitality
    AttributeDesc{ "enr", { AttributeFlag::Stats } }.setRange(-32, 95),                 // +# to Energy
    AttributeDesc{ "mana", { AttributeFlag::Stats } }.setRange(-32, 223),               // +# to Mana
    AttributeDesc{ "mana%", { AttributeFlag::Stats } }.setRange(-10, 53),               // Increase Maximum Mana #%
    AttributeDesc{ "hp", { AttributeFlag::Stats } }.setRange(-32, 479),                 // +# to Life
    AttributeDesc{ "hp%", { AttributeFlag::Stats } }.setRange(-10, 53),                 // Increase Maximum Life #%
    AttributeDesc{ "att", { AttributeFlag::Attack } }.setRange(1, 1023),                // +# to Attack Rating
    AttributeDesc{ "block", {}, { AttributeItemReq::Shield } },                         // #% Increased Chance of Blocking
    AttributeDesc{ "cold-min", { AttributeFlag::Damage } }.setRange(1, 255),            // +# to Minimum Cold Damage
    AttributeDesc{ "cold-max", { AttributeFlag::Damage } }.setRange(1, 511),            // +# to Maximum Cold Damage
    AttributeDesc{ "cold-len", { AttributeFlag::Damage } },                             // # of Frames (25 Frames = 1 Second)
    AttributeDesc{ "fire-min", { AttributeFlag::Damage } }.setRange(1, 255),            // +# to Minimum Fire Damage
    AttributeDesc{ "fire-max", { AttributeFlag::Damage } }.setRange(1, 511),            // +# to Maximum Fire Damage
    AttributeDesc{ "ltng-min", { AttributeFlag::Damage } }.setRange(1, 63),             // +# to Minimum Lightning Damage
    AttributeDesc{ "ltng-max", { AttributeFlag::Damage } }.setRange(1, 1023),           // +# to Maximum Lightning Damage
    AttributeDesc{ "pois-min", { AttributeFlag::Damage } },                             // +# to Minimum Poison Damage
    AttributeDesc{ "pois-max", { AttributeFlag::Damage } },                             // +# to Maximum Poison Damage
    AttributeDesc{ "pois-len", { AttributeFlag::Damage } },                             // # of Frames (25 Frames = 1 Second)
    AttributeDesc{ "dmg-min", { AttributeFlag::Damage } }.setRange(1, 63),              // +# to Minimum Damage
    AttributeDesc{ "dmg-max", { AttributeFlag::Damage } }.setRange(1, 127),             // +# to Maximum Damage
    AttributeDesc{ "dmg%", { AttributeFlag::Damage } }.setRange(1, 511),                // +#% Enhanced Damage
    AttributeDesc{ "dmg-to-mana" },                                                     // #% Damage Taken Goes To Mana
    AttributeDesc{ "res-fire", { AttributeFlag::Resistance } }.setRange(-90, 205),      // Fire Resist +#%
    AttributeDesc{ "res-fire-max", { AttributeFlag::Resistance } }.setRange(1, 31),     // +#% to Maximum Fire Resist
    AttributeDesc{ "res-ltng", { AttributeFlag::Resistance } }.setRange(-90, 205),      // Lightning Resist +#%
    AttributeDesc{ "res-ltng-max", { AttributeFlag::Resistance } }.setRange(1, 31),     // +#% to Maximum Lightning Resist
    AttributeDesc{ "res-cold", { AttributeFlag::Resistance } }.setRange(-90, 205),      // Cold Resist +#%
    AttributeDesc{ "res-cold-max", { AttributeFlag::Resistance } }.setRange(1, 31),     // +#% to Maximum Cold Resist
    AttributeDesc{ "res-mag", { AttributeFlag::Resistance } }.setRange(-90, 255),       // Magic Resist +#%
    AttributeDesc{ "res-mag-max", { AttributeFlag::Resistance } }.setRange(1, 31),      // +#% to Maximum Magic Resist
    AttributeDesc{ "res-pois", { AttributeFlag::Resistance } }.setRange(-90, 205),      // Poison Resist +#%
    AttributeDesc{ "res-pois-max", { AttributeFlag::Resistance } }.setRange(1, 31),     // +#% to Maximum Poison Resist
    AttributeDesc{ "res-all", { AttributeFlag::Resistance } }.setRange(-50, 205),       // All Resistances +#
    AttributeDesc{ "res-all-max", { AttributeFlag::Resistance } }.setRange(1, 31),      // Currently Not Used
    AttributeDesc{ "abs-fire%", { AttributeFlag::Resistance } }.setRange(1, 127),       // +# Fire Absorb
    AttributeDesc{ "abs-fire", { AttributeFlag::DamageReduction } }.setRange(1, 127),   // Fire Absorb #%
    AttributeDesc{ "abs-ltng%", { AttributeFlag::Resistance } }.setRange(1, 127),       // +# Lightning Absorb
    AttributeDesc{ "abs-ltng", { AttributeFlag::DamageReduction } }.setRange(1, 127),   // Lightning Absorb #%
    AttributeDesc{ "abs-mag%", { AttributeFlag::Resistance } }.setRange(1, 127),        // +# Magic Absorb
    AttributeDesc{ "abs-mag", { AttributeFlag::DamageReduction } }.setRange(1, 127),    // Magic Absorb #%
    AttributeDesc{ "abs-cold%", { AttributeFlag::Resistance } }.setRange(1, 127),       // +# Cold Absorb
    AttributeDesc{ "abs-cold", { AttributeFlag::DamageReduction } }.setRange(1, 127),   // Cold Absorb #%
    AttributeDesc{ "dur", { AttributeFlag::Durability } },                              // Durability: # of #
    AttributeDesc{ "dur%", { AttributeFlag::Durability } },                             // Increase Maximum Durability #%
    AttributeDesc{ "regen", { AttributeFlag::HasRange }, {}, -30, 33 },                 // Replenish Life +#
    AttributeDesc{ "thorns", { AttributeFlag::HasRange }, {}, 1, 127 },                 // Attacker Takes Damage of #
    AttributeDesc{ "swing1", { AttributeFlag::Speed } }.setRange(-20, 107),             // +#% Increased Attack Speed
    AttributeDesc{ "swing2", { AttributeFlag::Speed } }.setRange(-20, 107),             // +#% Increased Attack Speed
    AttributeDesc{ "swing3", { AttributeFlag::Speed } }.setRange(-20, 107),             // +#% Increased Attack Speed
    AttributeDesc{ "gold%", { AttributeFlag::HasRange }, {}, -100, 411 },               // #% Extra Gold from Monsters
    AttributeDesc{ "mag%", { AttributeFlag::HasRange }, {}, -100, 155 },                // #% Better Chance of Getting Magic Items
    AttributeDesc{ "knock", {}, { AttributeItemReq::Weapon } },                         // Knockback
    AttributeDesc{ "regen-stam" },                                                      // Heal Stamina Plus #%
    AttributeDesc{ "regen-mana" },                                                      // Regenerate Mana #%
    AttributeDesc{ "stam" },                                                            // +# Maximum Stamina
    AttributeDesc{ "manasteal", { AttributeFlag::Leech } }.setRange(1, 127),            // #% Mana stolen per hit
    AttributeDesc{ "lifesteal", { AttributeFlag::Leech } }.setRange(1, 127),            // #% Life stolen per hit
    AttributeDesc{ "ama", { AttributeFlag::Skills } }.setRange(1, 7),                   // +# to Amazon Skill Levels
    AttributeDesc{ "pal", { AttributeFlag::Skills } }.setRange(1, 7),                   // +# to Paladin Skill Levels
    AttributeDesc{ "nec", { AttributeFlag::Skills } }.setRange(1, 7),                   // +# to Necromancer Skill Levels
    AttributeDesc{ "sor", { AttributeFlag::Skills } }.setRange(1, 7),                   // +# to Sorceress Skill Levels
    AttributeDesc{ "bar", { AttributeFlag::Skills } }.setRange(1, 7),                   // +#  to Barbarian Skill Levels
    AttributeDesc{ "light", { AttributeFlag::HasRange }, {}, -4, 11 },                  // +# to Light Radius
    AttributeDesc{ "ease", {}, { AttributeItemReq::Weapon, AttributeItemReq::Armor } }, // Requirements -#%
    AttributeDesc{ "move1", { AttributeFlag::Speed } }.setRange(-20, 107),              // +#% Faster Run/Walk
    AttributeDesc{ "move2", { AttributeFlag::Speed } }.setRange(-20, 107),              // +#% Faster Run/Walk
    AttributeDesc{ "move3", { AttributeFlag::Speed } }.setRange(-20, 107),              // +#% Faster Run/Walk
    AttributeDesc{ "balance1", { AttributeFlag::Speed } }.setRange(-20, 107),           // +#% Faster Hit Recovery
    AttributeDesc{ "balance2", { AttributeFlag::Speed } }.setRange(-20, 107),           // +#% Faster Hit Recovery
    AttributeDesc{ "balance3", { AttributeFlag::Speed } }.setRange(-20, 107),           // +#% Faster Hit Recovery
    AttributeDesc{ "block1", { AttributeFlag::Speed } }.setRange(-20, 107),             // +#% Faster Block Rate
    AttributeDesc{ "block2", { AttributeFlag::Speed } }.setRange(-20, 107),             // +#% Faster Block Rate
    AttributeDesc{ "block3", { AttributeFlag::Speed } }.setRange(-20, 107),             // +#% Faster Block Rate
    AttributeDesc{ "cast1", { AttributeFlag::Speed } }.setRange(-20, 107),              // +#% Faster Cast Rate
    AttributeDesc{ "cast2", { AttributeFlag::Speed } }.setRange(-20, 107),              // +#% Faster Cast Rate
    AttributeDesc{ "cast3", { AttributeFlag::Speed } }.setRange(-20, 107),              // +#% Faster Cast Rate
    AttributeDesc{ "res-pois-len", { AttributeFlag::Resistance } }.setRange(-20, 235),  // Poison Length Reduced by #%
    AttributeDesc{ "dmg", { AttributeFlag::Damage } }.setRange(-20, 491),               // Damage +#
    AttributeDesc{ "howl" },                                                            // Hit Causes Monster to Flee #%
    AttributeDesc{ "stupidity" },                                                       // Hit Blinds Target +#
    AttributeDesc{ "ignore-ac", { AttributeFlag::Attack } },                            // Ignore Target's Defense
    AttributeDesc{ "reduce-ac", { AttributeFlag::Attack } },                            // -#% Target Defense
    AttributeDesc{ "noheal" },                                                          // Prevent Monster Heal
    AttributeDesc{ "half-freeze" },                                                     // Half Freeze Duration
    AttributeDesc{ "att%", { AttributeFlag::Attack } },                                 // #% Bonus to Attack Rating
    AttributeDesc{ "dmg-ac", { AttributeFlag::Attack } },                               // -# to Monster Defense Per Hit
    AttributeDesc{ "dmg-demon", { AttributeFlag::Damage } }.setRange(-20, 491),         // +#% Damage to Demons
    AttributeDesc{ "dmg-undead", { AttributeFlag::Damage } }.setRange(-20, 491),        // +#% Damage to Undead
    AttributeDesc{ "att-demon", { AttributeFlag::Attack } },                            // +# to Attack Rating against Demons
    AttributeDesc{ "att-undead", { AttributeFlag::Attack } },                           // +# to Attack Rating against Undead
    AttributeDesc{ "fireskill", { AttributeFlag::Skills } }.setRange(1, 7),             // +# to Fire Skills
    AttributeDesc{ "poisskill", { AttributeFlag::Skills } }.setRange(1, 7),
    AttributeDesc{ "coldskill", { AttributeFlag::Skills } }.setRange(1, 7),
    AttributeDesc{ "magskill", { AttributeFlag::Skills } }.setRange(1, 7),
    AttributeDesc{ "ltngskill", { AttributeFlag::Skills } }.setRange(1, 7),
    AttributeDesc{ "allskills", { AttributeFlag::Skills } }.setRange(1, 7),         // +# to All Skills
    AttributeDesc{ "light-thorns" },                                                // Attacker Takes Lightning Damage of #
    AttributeDesc{ "freeze" },                                                      // Freezes Target +#
    AttributeDesc{ "openwounds", { AttributeFlag::HasRange }, {}, 1, 127 },         // #% Chance of Open Wounds
    AttributeDesc{ "crush", { AttributeFlag::OP } }.setRange(1, 127),               // #% Chance of Crushing Blow
    AttributeDesc{ "kick", { AttributeFlag::Damage } },                             // +# Kick Damage
    AttributeDesc{ "mana-kill", { AttributeFlag::HasRange }, {}, 1, 127 },          // +# to Mana after each Kill
    AttributeDesc{ "demon-heal" },                                                  // +# Life after each Demon Kill
    AttributeDesc{ "bloody" },                                                      // Visuals Only
    AttributeDesc{ "deadly", { AttributeFlag::Damage } },                           // #% Deadly Strike
    AttributeDesc{ "slow", { AttributeFlag::HasRange }, {}, 1, 127 },               // Slows Target by #%
    AttributeDesc{ "nofreeze", { AttributeFlag::OP } },                             // Cannot Be Frozen
    AttributeDesc{ "stamdrain" },                                                   // #% Slower Stamina Drain
    AttributeDesc{ "reanimate" },                                                   // Reanimate As: [Returned]
    AttributeDesc{ "pierce", { AttributeFlag::Missile, AttributeFlag::Quantity } }, // Piercing Attack
    AttributeDesc{ "magicarrow", { AttributeFlag::Missile } },                      // Fires Magic Arrows
    AttributeDesc{ "explosivearrow", { AttributeFlag::Missile } },                  // Fires Explosive Arrows or Bolts
    AttributeDesc{ "dru", { AttributeFlag::Skills } }.setRange(1, 7),               // +# to Druid Skill Levels
    AttributeDesc{ "ass", { AttributeFlag::Skills } }.setRange(1, 7),               // +# to Assassin Skill Levels
    AttributeDesc{ "skill", { AttributeFlag::Skills } }.setRange(1, 7),             // +# to [Skill] ([Class] only)
    AttributeDesc{ "skilltab", { AttributeFlag::Skills } }.setRange(1, 7),          // +# to [Class Skill Tab] Skills
    AttributeDesc{ "aura" },                                                        // Level # [Skill] Aura When Equipped
    AttributeDesc{ "att-skill" },                                                   // #% Chance to cast level # [Skill] on attack
    AttributeDesc{ "hit-skill" },                                                   // #% Chance to cast level # [Skill] on striking
    AttributeDesc{ "gethit-skill" },                                                // #% Chance to cast level # [Skill] when struck

    AttributeDesc{ "sock", { AttributeFlag::Sockets } }, // Socketed (#)

    AttributeDesc{ "dmg-fire", { AttributeFlag::Damage } }, // Adds #-# Fire Damage
    AttributeDesc{ "dmg-ltng", { AttributeFlag::Damage } }, // Adds #-# Lightning Damage
    AttributeDesc{ "dmg-mag", { AttributeFlag::Damage } },  // Adds #-# Magic Damage
    AttributeDesc{ "dmg-cold", { AttributeFlag::Damage } }, // Adds #-# Cold Damage
    AttributeDesc{ "dmg-pois", { AttributeFlag::Damage } }, // Adds #-# Poison Damage Over # Seconds
    AttributeDesc{ "dmg-norm", { AttributeFlag::Damage } }, // Adds #-# Damage

    AttributeDesc{ "ac/lvl", { AttributeFlag::PerLevel, AttributeFlag::Defense } },               // +# Defense (Based on Character Level)
    AttributeDesc{ "ac%/lvl", { AttributeFlag::PerLevel, AttributeFlag::Defense } },              // +#% Enhanced Defense (Based on Character Level)
    AttributeDesc{ "hp/lvl", { AttributeFlag::PerLevel, AttributeFlag::Stats } },                 // +# to Life (Based on Character Level)
    AttributeDesc{ "mana/lvl", { AttributeFlag::PerLevel, AttributeFlag::Stats } },               // +# to Mana (Based on Character Level)
    AttributeDesc{ "dmg/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },               // +# to Maximum Damage (Based on Character Level)
    AttributeDesc{ "dmg%/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },              // +#% Enhanced Maximum Damage (Based on Character Level)
    AttributeDesc{ "str/lvl", { AttributeFlag::PerLevel, AttributeFlag::Stats } },                // +# to Strength (Based on Character Level)
    AttributeDesc{ "dex/lvl", { AttributeFlag::PerLevel, AttributeFlag::Stats } },                // +# to Dexterity (Based on Character Level)
    AttributeDesc{ "enr/lvl", { AttributeFlag::PerLevel, AttributeFlag::Stats } },                // +# to Energy (Based on Character Level)
    AttributeDesc{ "vit/lvl", { AttributeFlag::PerLevel, AttributeFlag::Stats } },                // +# to Vitality (Based on Character Level)
    AttributeDesc{ "att/lvl", { AttributeFlag::PerLevel, AttributeFlag::Attack } },               // +# to Attack Rating (Based on Character Level)
    AttributeDesc{ "att%/lvl", { AttributeFlag::PerLevel, AttributeFlag::Attack } },              // #% Bonus to Attack Rating (Based on Character Level)
    AttributeDesc{ "dmg-cold/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },          // +# to Maximum Cold Damage (Based on Character Level)
    AttributeDesc{ "dmg-fire/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },          // +# to Maximum Fire Damage (Based on Character Level)
    AttributeDesc{ "dmg-ltng/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },          // +# to Maximum Lightning Damage (Based on Character Level)
    AttributeDesc{ "dmg-pois/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },          // +# to Maximum Poison Damage (Based on Character Level)
    AttributeDesc{ "res-cold/lvl", { AttributeFlag::PerLevel, AttributeFlag::Resistance } },      // Cold Resist +#% (Based on Character Level)
    AttributeDesc{ "res-fire/lvl", { AttributeFlag::PerLevel, AttributeFlag::Resistance } },      // Fire Resist +#% (Based on Character Level)
    AttributeDesc{ "res-ltng/lvl", { AttributeFlag::PerLevel, AttributeFlag::Resistance } },      // Lightning Resist +#% (Based on Character Level)
    AttributeDesc{ "res-pois/lvl", { AttributeFlag::PerLevel, AttributeFlag::Resistance } },      // Poison Resist +#% (Based on Character Level)
    AttributeDesc{ "abs-cold/lvl", { AttributeFlag::PerLevel, AttributeFlag::DamageReduction } }, // Absorbs Cold Damage (Based on Character Level)
    AttributeDesc{ "abs-fire/lvl", { AttributeFlag::PerLevel, AttributeFlag::DamageReduction } }, // Absorbs Fire Damage (Based on Character Level)
    AttributeDesc{ "abs-ltng/lvl", { AttributeFlag::PerLevel, AttributeFlag::DamageReduction } }, // Absorbs Lightning Damage (Based on Character Level)
    AttributeDesc{ "abs-pois/lvl", { AttributeFlag::PerLevel, AttributeFlag::DamageReduction } }, // Absorbs Poison Damage (Based on Character Level)
    AttributeDesc{ "thorns/lvl", { AttributeFlag::PerLevel } },                                   // Attacker Takes Damage of # (Based on Character Level)
    AttributeDesc{ "gold%/lvl", { AttributeFlag::PerLevel } },                                    // #% Extra Gold from Monsters (Based on Character Level)
    AttributeDesc{ "mag%/lvl", { AttributeFlag::PerLevel } },                                     // #% Better Chance of Getting Magic Items (Based on Character Level)
    AttributeDesc{ "regen-stam/lvl", { AttributeFlag::PerLevel } },                               // Heal Stamina Plus #% (Based on Character Level)
    AttributeDesc{ "stam/lvl", { AttributeFlag::PerLevel } },                                     // +# Maximum Stamina (Based on Character Level)
    AttributeDesc{ "dmg-dem/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },           // +#% Damage to Demons (Based on Character Level)
    AttributeDesc{ "dmg-und/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },           // +#% Damage to Undead (Based on Character Level)
    AttributeDesc{ "att-dem/lvl", { AttributeFlag::PerLevel, AttributeFlag::Attack } },           // +# to Attack Rating against Demons (Based on Character Level)
    AttributeDesc{ "att-und/lvl", { AttributeFlag::PerLevel, AttributeFlag::Attack } },           // +# to Attack Rating against Undead (Based on Character Level)
    AttributeDesc{ "crush/lvl", { AttributeFlag::PerLevel, AttributeFlag::OP } },                 // #% Chance of Crushing Blow (Based on Character Level)
    AttributeDesc{ "wounds/lvl", { AttributeFlag::PerLevel } },                                   // #% Chance of Open Wounds (Based on Character Level)
    AttributeDesc{ "kick/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },              // +# Kick Damage (Based on Character Level)
    AttributeDesc{ "deadly/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },            // #% Deadly Strike (Based on Character Level)

    AttributeDesc{ "rep-dur", { AttributeFlag::Durability } },                // Repairs 1 durability in # seconds
    AttributeDesc{ "rep-quant", { AttributeFlag::Quantity } },                // Replenishes quantity
    AttributeDesc{ "stack", { AttributeFlag::Quantity } },                    // Increased Stack Size
    AttributeDesc{ "pierce-fire" }.setRange(1, 255),                          // -#% to Enemy Fire Resistance
    AttributeDesc{ "pierce-ltng" }.setRange(1, 255),                          // -#% to Enemy Lightning Resistance
    AttributeDesc{ "pierce-cold" }.setRange(1, 255),                          // -#% to Enemy Cold Resistance
    AttributeDesc{ "pierce-pois" }.setRange(1, 255),                          // -#% to Enemy Poison Resistance
    AttributeDesc{ "indestruct", { AttributeFlag::Durability } },             // Indestructible
    AttributeDesc{ "charged" },                                               // Level # [Skill] (#/# Charges)
    AttributeDesc{ "extra-fire" }.setRange(-50, 461),                         // +#% to Fire Skill Damage
    AttributeDesc{ "extra-ltng" }.setRange(-50, 461),                         // +#% to Lightning Skill Damage
    AttributeDesc{ "extra-cold" }.setRange(-50, 461),                         // +#% to Cold Skill Damage
    AttributeDesc{ "extra-pois" }.setRange(-50, 461),                         // +#% to Poison Skill Damage
    AttributeDesc{ "dmg-elem", { AttributeFlag::Damage } },                   // Adds #-# Fire/Lightning/Cold Damage
    AttributeDesc{ "dmg-elem-min", { AttributeFlag::Damage } },               // +# to Minimum Fire/Lightning/Cold Damage
    AttributeDesc{ "dmg-elem-max", { AttributeFlag::Damage } },               // +# to Maximum Fire/Lightning/Cold Damage
    AttributeDesc{ "all-stats", { AttributeFlag::Stats } }.setRange(-32, 95), // +# to all Attributes
    AttributeDesc{ "addxp", { AttributeFlag::OP } }.setRange(-50, 461),       // +#% to Experience Gained
    AttributeDesc{ "heal-kill" },                                             // +# Life after each Kill
    AttributeDesc{ "cheap" }.setRange(1, 127),                                // Reduces all Vendor Prices #%
    AttributeDesc{ "rip" },                                                   // Slain Monsters Rest in Peace
    AttributeDesc{ "att-mon%", { AttributeFlag::Attack } },                   // #% to Attack Rating versus [Monster Type]
    AttributeDesc{ "dmg-mon%", { AttributeFlag::Damage } },                   // #% to Damage versus [Monster Type]
    AttributeDesc{ "kill-skill" },                                            // #% Chance to cast level # [Skill] when you Kill an Enemy
    AttributeDesc{ "death-skill" },                                           // #% Chance to cast level # [Skill] when you Die
    AttributeDesc{ "levelup-skill" },                                         // #% Chance to cast level # [Skill] when you Level-Up
    AttributeDesc{ "skill-rand", { AttributeFlag::Skills } },                 // +# to [Skill] ([Class] only)
    AttributeDesc{ "fade" },                                                  // Visuals Only
    AttributeDesc{ "levelreq" },                                              // Required Level: #
    AttributeDesc{ "ethereal", { AttributeFlag::Durability } },               // Ethereal
    AttributeDesc{ "oskill", { AttributeFlag::Skills } }.setRange(1, 63),     // +# to [Skill]
    AttributeDesc{ "state" },                                                 // Applies a State on the unit
    AttributeDesc{ "randclassskill", { AttributeFlag::Skills } },             // +# to [Class] Skill Levels

    AttributeDesc{ "map-glob-monsterrarity", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-extra-fire", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-glob-density", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-play-addxp", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-extra-cold", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-play-mag-gold%", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-extra-ltng", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-extra-pois", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-extra-mag", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-glob-arealevel", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-att-pierce", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-att-cast-speed", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-hp%", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-ed%", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-splash", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-openwounds", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-play-regen", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-crush", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-phys-as-extra-ltng", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-phys-as-extra-cold", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-phys-as-extra-fire", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-phys-as-extra-pois", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-phys-as-extra-mag", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-glob-add-mon-doll", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-glob-add-mon-succ", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-glob-add-mon-vamp", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-glob-add-mon-cow", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-glob-add-mon-horde", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-glob-add-mon-ghost", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-glob-add-mon-souls", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-glob-add-mon-fetish", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-ac%", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-abs-fire%", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-abs-ltng%", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-abs-mag%", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-abs-cold%", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-red-dmg", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-velocity%", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-regen", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-lifesteal-hp%", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-balance1", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-play-balance1", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-play-lightradius", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-mon-curseresist-hp%", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-play-res-all", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-play-ac%", { AttributeFlag::PD2Map } },
    AttributeDesc{ "map-play-block", { AttributeFlag::PD2Map } },

    AttributeDesc{ "inc-splash-radius" },
    AttributeDesc{ "leapspeed" },
    AttributeDesc{ "blood-warp-life-reduction" },
    AttributeDesc{ "extra-skele-war" },
    AttributeDesc{ "extra-skele-mage" },
    AttributeDesc{ "heal-hit" },
    AttributeDesc{ "rep-charge" },
    AttributeDesc{ "grims-extra-skele-mage" },
    AttributeDesc{ "cast", { AttributeFlag::Speed } },
    AttributeDesc{ "inc_splash_radius" },
    AttributeDesc{ "ias-frw" },
    AttributeDesc{ "pierce-phys" },
    AttributeDesc{ "extra-spirits" },
    AttributeDesc{ "joust-reduction" },
    AttributeDesc{ "maxcurse" },
    AttributeDesc{ "extra-revives" },
    AttributeDesc{ "curse-res" },
    AttributeDesc{ "randclassskill2", { AttributeFlag::Skills } },
    AttributeDesc{ "randclassskill1", { AttributeFlag::Skills } },
    AttributeDesc{ "gust-reduction" },
    AttributeDesc{ "dclone-clout" },
    AttributeDesc{ "maxlevel-clout" },
    AttributeDesc{ "dev-clout" },
    AttributeDesc{ "socketed-text" },
    AttributeDesc{ "silence-fhr-ias" },
    AttributeDesc{ "extra-hydra" },
    AttributeDesc{ "extra-valk" },
    AttributeDesc{ "extra-magi" },
    AttributeDesc{ "plague-fcr-pierce" },
    AttributeDesc{ "infinityspeed" },
    AttributeDesc{ "extra-golem" },
    AttributeDesc{ "mana-steal" },

    AttributeDesc{ "Light" },
    AttributeDesc{ "Thorns" },

    AttributeDesc{ "pierce-immunity-cold" },
    AttributeDesc{ "pierce-immunity-fire" },
    AttributeDesc{ "pierce-immunity-light" },
    AttributeDesc{ "pierce-immunity-poison" },
    AttributeDesc{ "pierce-immunity-damage" },
    AttributeDesc{ "pierce-immunity-magic" },
    AttributeDesc{ "charge-noconsume" },

    // RotW:
    AttributeDesc{ "pierce-mag" }.setRange(1, 255),                            // -#% to Enemy Magic Resistance
    AttributeDesc{ "extra-mag" }.setRange(-50, 461),                           // +#% to Magic Skill Damage
    AttributeDesc{ "war", { AttributeFlag::Skills } }.setRange(1, 7),          // +# to Warlock Skill Levels
    AttributeDesc{ "skilltab-war", { AttributeFlag::Skills } }.setRange(1, 7), // +# to Warlock Skill random tab Levels

};

const std::unordered_map<std::string, int> s_attributesIndex = [] {
    std::unordered_map<std::string, int> result;
    int                                  i = 0;
    for (const auto& attr : s_attributes)
        result[attr.code] = i++;
    return result;
}();

int attributesIndexValue(const std::string& code)
{
    auto it = s_attributesIndex.find(code);
    return it == s_attributesIndex.cend() ? -1 : it->second;
}

}

AttributeConsume getAttributeConsume(const std::string& code)
{
    if (code.empty() || code.starts_with("*"))
        return AttributeConsume::Skip;

    const bool isKnown = attributesIndexValue(code) != -1;
    if (isKnown)
        return AttributeConsume::Known;

    return AttributeConsume::Keep;
}

const AttributeDesc& getAttributeDesc(const std::string& code)
{
    static const AttributeDesc s_stub{};
    const int                  index = attributesIndexValue(code);
    assert(index >= 0);
    if (index < 0)
        return s_stub;
    return s_attributes[index];
}

bool isMinMaxRange(const std::string& code)
{
    if (getAttributeConsume(code) != AttributeConsume::Known)
        return false;

    const AttributeDesc& desc     = getAttributeDesc(code);
    const bool           hasRange = desc.flags.contains(AttributeFlag::HasRange);
    return hasRange;
}

void UniqueAttributeChecker::add(const std::string& attr)
{
    m_data.insert(attr);
    static const StringSet s_aliased{
        "swing1",
        "swing2",
        "swing3",
        "move1",
        "move2",
        "move3",
        "balance1",
        "balance2",
        "balance3",
        "block1",
        "block2",
        "block3",
        "cast1",
        "cast2",
        "cast3",
        "cast",
    };
    static const std::vector<StringSet> s_aliasedGroups{
        {
            "swing1",
            "swing2",
            "swing3",
        },
        {
            "move1",
            "move2",
            "move3",
        },
        {
            "balance1",
            "balance2",
            "balance3",
        },
        {
            "block1",
            "block2",
            "block3",
        },
        {
            "cast1",
            "cast2",
            "cast3",
            "cast",
        },
    };
    if (s_aliased.contains(attr)) {
        for (const auto& group : s_aliasedGroups) {
            if (group.contains(attr)) {
                m_data.insert(group.cbegin(), group.cend());
                break;
            }
        }
    }
}

void UniqueAttributeChecker::add(const StringSet& attrs)
{
    for (auto& attr : attrs)
        add(attr);
}

int powerMultiply(const std::string& code, int value, int powerMultPercent)
{
    if (getAttributeConsume(code) != AttributeConsume::Known)
        return value;

    const AttributeDesc& desc     = getAttributeDesc(code);
    const bool           hasRange = desc.flags.contains(AttributeFlag::HasRange);
    if (!hasRange)
        return value;
    int powerPercent = 100;
    if (desc.flags.contains(AttributeFlag::Skills)
        || desc.flags.contains(AttributeFlag::DamageReduction)
        || desc.flags.contains(AttributeFlag::Speed))
        powerPercent = 50;

    if (powerMultPercent > 100) {
        const int diff = (powerMultPercent - 100) * powerPercent / 100;
        value          = value * (100 + diff) / 100;
    } else if (powerMultPercent < 100) {
        const int diff = (100 - powerMultPercent);
        value          = value * (100 - diff) / 100;
    }
    value = std::clamp(value, desc.minValue, desc.maxValue);
    return value;
}

}
