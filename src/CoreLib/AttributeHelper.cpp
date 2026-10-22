/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "AttributeHelper.hpp"
#include "MernelPlatform/StringUtils.hpp"

#include <algorithm>
#include <unordered_map>
#include <vector>

#include <cassert>

namespace D2ModGen {

namespace {
using AF  = AttributeFlag;
using CH  = Character;
using IDI = AttributeIdiom;

const std::map<std::string, Character> s_classes{

    { "ama", Character::ama },
    { "sor", Character::sor },
    { "nec", Character::nec },
    { "pal", Character::pal },
    { "bar", Character::bar },
    { "dru", Character::dru },
    { "ass", Character::ass },
    { "war", Character::war },
};

const std::vector<AttributeDesc> s_attributes{

    AttributeDesc{ AttributeId::ac, { AF::Defense } }.setRange(-10, 2037),                                       // +# Defense
    AttributeDesc{ AttributeId::ac_miss, { AF::Defense } }.setRange(1, 511),                                     // +# Defense vs. Missile
    AttributeDesc{ AttributeId::ac_hth, { AF::Defense } }.setRange(1, 255),                                      // +# Defense vs. Melee
    AttributeDesc{ AttributeId::red_dmg, { AF::DamageReduction } }.setRange(1, 63),                              // Damage Reduced by #
    AttributeDesc{ AttributeId::red_dmg_percent, { AF::Resistance } }.setRange(-25, 255),                        // Damage Reduced by #%
    AttributeDesc{ AttributeId::ac_percent, { AF::Defense } }.setRange(1, 511),                                  // +#% Enhanced Defense
    AttributeDesc{ AttributeId::red_mag, { AF::DamageReduction } }.setRange(1, 63),                              // Magic Damage Reduced by #
    AttributeDesc{ AttributeId::str, { AF::Stats } }.setRange(-32, 223),                                         // +# to Strength
    AttributeDesc{ AttributeId::dex, { AF::Stats } }.setRange(-32, 95),                                          // +# to Dexterity
    AttributeDesc{ AttributeId::vit, { AF::Stats } }.setRange(-32, 95),                                          // +# to Vitality
    AttributeDesc{ AttributeId::enr, { AF::Stats } }.setRange(-32, 95),                                          // +# to Energy
    AttributeDesc{ AttributeId::mana, { AF::Stats } }.setRange(-32, 223),                                        // +# to Mana
    AttributeDesc{ AttributeId::mana_percent, { AF::Stats } }.setRange(-10, 53),                                 // Increase Maximum Mana #%
    AttributeDesc{ AttributeId::hp, { AF::Stats } }.setRange(-32, 479),                                          // +# to Life
    AttributeDesc{ AttributeId::hp_percent, { AF::Stats } }.setRange(-10, 53),                                   // Increase Maximum Life #%
    AttributeDesc{ AttributeId::att, { AF::Attack } }.setIdiom(IDI::Brawler).setRange(1, 1023),                  // +# to Attack Rating
    AttributeDesc{ AttributeId::block, {}, { AttributeItemReq::Shield } },                                       // #% Increased Chance of Blocking
    AttributeDesc{ AttributeId::cold_min, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 255),              // +# to Minimum Cold Damage
    AttributeDesc{ AttributeId::cold_max, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 511),              // +# to Maximum Cold Damage
    AttributeDesc{ AttributeId::cold_len, { AF::Damage } }.setIdiom(IDI::Brawler),                               // # of Frames (25 Frames = 1 Second)
    AttributeDesc{ AttributeId::fire_min, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 255),              // +# to Minimum Fire Damage
    AttributeDesc{ AttributeId::fire_max, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 511),              // +# to Maximum Fire Damage
    AttributeDesc{ AttributeId::ltng_min, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63),               // +# to Minimum Lightning Damage
    AttributeDesc{ AttributeId::ltng_max, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 1023),             // +# to Maximum Lightning Damage
    AttributeDesc{ AttributeId::pois_min, { AF::Damage } }.setIdiom(IDI::Brawler),                               // +# to Minimum Poison Damage
    AttributeDesc{ AttributeId::pois_max, { AF::Damage } }.setIdiom(IDI::Brawler),                               // +# to Maximum Poison Damage
    AttributeDesc{ AttributeId::pois_len, { AF::Damage } }.setIdiom(IDI::Brawler),                               // # of Frames (25 Frames = 1 Second)
    AttributeDesc{ AttributeId::dmg_min, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63),                // +# to Minimum Damage
    AttributeDesc{ AttributeId::dmg_max, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 127),               // +# to Maximum Damage
    AttributeDesc{ AttributeId::dmg_percent, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 511),           // +#% Enhanced Damage
    AttributeDesc{ AttributeId::dmg_to_mana },                                                                   // #% Damage Taken Goes To Mana
    AttributeDesc{ AttributeId::res_fire, { AF::Resistance } }.setRange(-90, 205),                               // Fire Resist +#%
    AttributeDesc{ AttributeId::res_fire_max, { AF::Resistance } }.setRange(1, 31),                              // +#% to Maximum Fire Resist
    AttributeDesc{ AttributeId::res_ltng, { AF::Resistance } }.setRange(-90, 205),                               // Lightning Resist +#%
    AttributeDesc{ AttributeId::res_ltng_max, { AF::Resistance } }.setRange(1, 31),                              // +#% to Maximum Lightning Resist
    AttributeDesc{ AttributeId::res_cold, { AF::Resistance } }.setRange(-90, 205),                               // Cold Resist +#%
    AttributeDesc{ AttributeId::res_cold_max, { AF::Resistance } }.setRange(1, 31),                              // +#% to Maximum Cold Resist
    AttributeDesc{ AttributeId::res_mag, { AF::Resistance } }.setRange(-90, 255),                                // Magic Resist +#%
    AttributeDesc{ AttributeId::res_mag_max, { AF::Resistance } }.setRange(1, 31),                               // +#% to Maximum Magic Resist
    AttributeDesc{ AttributeId::res_pois, { AF::Resistance } }.setRange(-90, 205),                               // Poison Resist +#%
    AttributeDesc{ AttributeId::res_pois_max, { AF::Resistance } }.setRange(1, 31),                              // +#% to Maximum Poison Resist
    AttributeDesc{ AttributeId::res_all, { AF::Resistance } }.setRange(-50, 205),                                // All Resistances +#
    AttributeDesc{ AttributeId::res_all_max, { AF::Resistance } }.setRange(1, 31),                               // Currently Not Used
    AttributeDesc{ AttributeId::abs_fire_percent, { AF::Resistance } }.setRange(1, 127),                         // +# Fire Absorb
    AttributeDesc{ AttributeId::abs_fire, { AF::DamageReduction } }.setRange(1, 127),                            // Fire Absorb #%
    AttributeDesc{ AttributeId::abs_ltng_percent, { AF::Resistance } }.setRange(1, 127),                         // +# Lightning Absorb
    AttributeDesc{ AttributeId::abs_ltng, { AF::DamageReduction } }.setRange(1, 127),                            // Lightning Absorb #%
    AttributeDesc{ AttributeId::abs_mag_percent, { AF::Resistance } }.setRange(1, 127),                          // +# Magic Absorb
    AttributeDesc{ AttributeId::abs_mag, { AF::DamageReduction } }.setRange(1, 127),                             // Magic Absorb #%
    AttributeDesc{ AttributeId::abs_cold_percent, { AF::Resistance } }.setRange(1, 127),                         // +# Cold Absorb
    AttributeDesc{ AttributeId::abs_cold, { AF::DamageReduction } }.setRange(1, 127),                            // Cold Absorb #%
    AttributeDesc{ AttributeId::dur, { AF::Durability } },                                                       // Durability: # of #
    AttributeDesc{ AttributeId::dur_percent, { AF::Durability } },                                               // Increase Maximum Durability #%
    AttributeDesc{ AttributeId::regen, { AF::HasRange }, {}, -30, 33 },                                          // Replenish Life +#
    AttributeDesc{ AttributeId::thorns, { AF::HasRange }, {}, 1, 127 },                                          // Attacker Takes Damage of #
    AttributeDesc{ AttributeId::swing3, { AF::Speed } }.setRange(-20, 100).setRounding(5),                       // +#% Increased Attack Speed
    AttributeDesc{ AttributeId::gold_percent, { AF::HasRange }, {}, -100, 411 },                                 // #% Extra Gold from Monsters
    AttributeDesc{ AttributeId::mag_percent, { AF::HasRange }, {}, -100, 155 },                                  // #% Better Chance of Getting Magic Items
    AttributeDesc{ AttributeId::knock, {}, { AttributeItemReq::Weapon } },                                       // Knockback
    AttributeDesc{ AttributeId::regen_stam },                                                                    // Heal Stamina Plus #%
    AttributeDesc{ AttributeId::regen_mana },                                                                    // Regenerate Mana #%
    AttributeDesc{ AttributeId::stam },                                                                          // +# Maximum Stamina
    AttributeDesc{ AttributeId::manasteal, { AF::Leech } }.setIdiom(IDI::Brawler).setRange(1, 127),              // #% Mana stolen per hit
    AttributeDesc{ AttributeId::lifesteal, { AF::Leech } }.setIdiom(IDI::Brawler).setRange(1, 127),              // #% Life stolen per hit
    AttributeDesc{ AttributeId::ama, { AF::Skills } }.setRange(1, 7).setClass(CH::ama),                          // +# to Amazon Skill Levels
    AttributeDesc{ AttributeId::pal, { AF::Skills } }.setRange(1, 7).setClass(CH::pal),                          // +# to Paladin Skill Levels
    AttributeDesc{ AttributeId::nec, { AF::Skills } }.setRange(1, 7).setClass(CH::nec),                          // +# to Necromancer Skill Levels
    AttributeDesc{ AttributeId::sor, { AF::Skills } }.setRange(1, 7).setClass(CH::sor),                          // +# to Sorceress Skill Levels
    AttributeDesc{ AttributeId::bar, { AF::Skills } }.setRange(1, 7).setClass(CH::bar),                          // +#  to Barbarian Skill Levels
    AttributeDesc{ AttributeId::light }.setRange(-4, 11),                                                        // +# to Light Radius
    AttributeDesc{ AttributeId::ease, { AF::Negative }, { AttributeItemReq::Weapon, AttributeItemReq::Armor } }, // Requirements -#%
    AttributeDesc{ AttributeId::move3, { AF::Speed } }.setRange(-20, 100).setRounding(5),                        // +#% Faster Run/Walk
    AttributeDesc{ AttributeId::balance3, { AF::Speed } }.setRange(-20, 100).setRounding(5),                     // +#% Faster Hit Recovery
    AttributeDesc{ AttributeId::block3, { AF::Speed } }.setRange(-20, 100).setRounding(5),                       // +#% Faster Block Rate
    AttributeDesc{ AttributeId::cast3, { AF::Speed } }.setIdiom(IDI::Caster).setRange(-20, 100).setRounding(5),  // +#% Faster Cast Rate
    AttributeDesc{ AttributeId::res_pois_len, { AF::Resistance } }.setRange(-20, 235),                           // Poison Length Reduced by #%
    AttributeDesc{ AttributeId::dmg, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(-20, 491),                 // Damage +#
    AttributeDesc{ AttributeId::howl },                                                                          // Hit Causes Monster to Flee #%
    AttributeDesc{ AttributeId::stupidity },                                                                     // Hit Blinds Target +#
    AttributeDesc{ AttributeId::ignore_ac, { AF::Attack } }.setIdiom(IDI::Brawler).setRange(1, 1),               // Ignore Target's Defense
    AttributeDesc{ AttributeId::reduce_ac, { AF::Attack } }.setIdiom(IDI::Brawler),                              // #% Target Defense
    AttributeDesc{ AttributeId::noheal }.setIdiom(IDI::Brawler),                                                 // Prevent Monster Heal
    AttributeDesc{ AttributeId::half_freeze },                                                                   // Half Freeze Duration
    AttributeDesc{ AttributeId::att_percent, { AF::Attack } }.setIdiom(IDI::Brawler),                            // #% Bonus to Attack Rating
    AttributeDesc{ AttributeId::dmg_ac, { AF::Attack, AF::Negative } }.setIdiom(IDI::Brawler),                   // -# to Monster Defense Per Hit
    AttributeDesc{ AttributeId::dmg_demon, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(-20, 491),           // +#% Damage to Demons
    AttributeDesc{ AttributeId::dmg_undead, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(-20, 491),          // +#% Damage to Undead
    AttributeDesc{ AttributeId::att_demon, { AF::Attack } }.setIdiom(IDI::Brawler),                              // +# to Attack Rating against Demons
    AttributeDesc{ AttributeId::att_undead, { AF::Attack } }.setIdiom(IDI::Brawler),                             // +# to Attack Rating against Undead
    AttributeDesc{ AttributeId::fireskill, { AF::Skills } }.setIdiom(IDI::Caster).setRange(1, 7),                // +# to Fire Skills
    AttributeDesc{ AttributeId::poisskill, { AF::Skills } }.setIdiom(IDI::Caster).setRange(1, 7),
    AttributeDesc{ AttributeId::coldskill, { AF::Skills } }.setIdiom(IDI::Caster).setRange(1, 7),
    AttributeDesc{ AttributeId::magskill, { AF::Skills } }.setIdiom(IDI::Caster).setRange(1, 7),
    AttributeDesc{ AttributeId::ltngskill, { AF::Skills } }.setIdiom(IDI::Caster).setRange(1, 7),
    AttributeDesc{ AttributeId::allskills, { AF::Skills } }.setRange(1, 7),                     // +# to All Skills
    AttributeDesc{ AttributeId::light_thorns },                                                 // Attacker Takes Lightning Damage of #
    AttributeDesc{ AttributeId::freeze }.setIdiom(IDI::Brawler),                                // Freezes Target +#
    AttributeDesc{ AttributeId::openwounds }.setIdiom(IDI::Brawler).setRange(1, 127),           // #% Chance of Open Wounds
    AttributeDesc{ AttributeId::crush, { AF::OP } }.setIdiom(IDI::Brawler).setRange(1, 127),    // #% Chance of Crushing Blow
    AttributeDesc{ AttributeId::kick, { AF::Damage } }.setIdiom(IDI::Brawler),                  // +# Kick Damage
    AttributeDesc{ AttributeId::mana_kill }.setRange(1, 127),                                   // +# to Mana after each Kill
    AttributeDesc{ AttributeId::demon_heal },                                                   // +# Life after each Demon Kill
    AttributeDesc{ AttributeId::bloody },                                                       // Visuals Only
    AttributeDesc{ AttributeId::deadly, { AF::Damage } }.setIdiom(IDI::Brawler),                // #% Deadly Strike
    AttributeDesc{ AttributeId::slow }.setIdiom(IDI::Brawler).setRange(1, 127),                 // Slows Target by #%
    AttributeDesc{ AttributeId::nofreeze, { AF::OP } },                                         // Cannot Be Frozen
    AttributeDesc{ AttributeId::stamdrain },                                                    // #% Slower Stamina Drain
    AttributeDesc{ AttributeId::reanimate },                                                    // Reanimate As: [Returned]
    AttributeDesc{ AttributeId::pierce, { AF::Missile, AF::Quantity } }.setIdiom(IDI::Brawler), // Piercing Attack
    AttributeDesc{ AttributeId::magicarrow, { AF::Missile } },                                  // Fires Magic Arrows
    AttributeDesc{ AttributeId::explosivearrow, { AF::Missile } },                              // Fires Explosive Arrows or Bolts
    AttributeDesc{ AttributeId::dru, { AF::Skills } }.setRange(1, 7).setClass(CH::dru),         // +# to Druid Skill Levels
    AttributeDesc{ AttributeId::ass, { AF::Skills } }.setRange(1, 7).setClass(CH::ass),         // +# to Assassin Skill Levels
    AttributeDesc{ AttributeId::skill, { AF::Skills, AF::SkillPar } }.setRange(1, 7),           // +# to [Skill] ([Class] only)
    AttributeDesc{ AttributeId::skilltab, { AF::Skills, AF::SkillPar } }.setRange(1, 7),        // +# to [Class Skill Tab] Skills
    AttributeDesc{ AttributeId::aura },                                                         // Level # [Skill] Aura When Equipped
    AttributeDesc{ AttributeId::att_skill }.setIdiom(IDI::Brawler),                             // #% Chance to cast level # [Skill] on attack
    AttributeDesc{ AttributeId::hit_skill }.setIdiom(IDI::Brawler),                             // #% Chance to cast level # [Skill] on striking
    AttributeDesc{ AttributeId::gethit_skill },                                                 // #% Chance to cast level # [Skill] when struck

    AttributeDesc{ AttributeId::sock, { AF::Sockets } }.setRange(1, 6), // Socketed (#)

    AttributeDesc{ AttributeId::dmg_fire, { AF::Damage } }.setIdiom(IDI::Brawler), // Adds #-# Fire Damage
    AttributeDesc{ AttributeId::dmg_ltng, { AF::Damage } }.setIdiom(IDI::Brawler), // Adds #-# Lightning Damage
    AttributeDesc{ AttributeId::dmg_mag, { AF::Damage } }.setIdiom(IDI::Brawler),  // Adds #-# Magic Damage
    AttributeDesc{ AttributeId::dmg_cold, { AF::Damage } }.setIdiom(IDI::Brawler), // Adds #-# Cold Damage
    AttributeDesc{ AttributeId::dmg_pois, { AF::Damage } }.setIdiom(IDI::Brawler), // Adds #-# Poison Damage Over # Seconds
    AttributeDesc{ AttributeId::dmg_norm, { AF::Damage } }.setIdiom(IDI::Brawler), // Adds #-# Damage

    AttributeDesc{ AttributeId::rep_dur, { AF::Durability, AF::SinglePar } },             // Repairs 1 durability in # seconds
    AttributeDesc{ AttributeId::rep_quant, { AF::Quantity, AF::SinglePar } },             // Replenishes quantity
    AttributeDesc{ AttributeId::stack, { AF::Quantity } },                                // Increased Stack Size
    AttributeDesc{ AttributeId::pierce_fire }.setRange(1, 255),                           // -#% to Enemy Fire Resistance
    AttributeDesc{ AttributeId::pierce_ltng }.setRange(1, 255),                           // -#% to Enemy Lightning Resistance
    AttributeDesc{ AttributeId::pierce_cold }.setRange(1, 255),                           // -#% to Enemy Cold Resistance
    AttributeDesc{ AttributeId::pierce_pois }.setRange(1, 255),                           // -#% to Enemy Poison Resistance
    AttributeDesc{ AttributeId::indestruct, { AF::Durability } }.setRange(1, 1),          // Indestructible
    AttributeDesc{ AttributeId::charged, { AF::SkillPar } },                              // Level # [Skill] (#/# Charges)  par: [SKill], min:-ChargesCount, max: -Level
    AttributeDesc{ AttributeId::extra_fire }.setRange(-50, 461),                          // +#% to Fire Skill Damage
    AttributeDesc{ AttributeId::extra_ltng }.setRange(-50, 461),                          // +#% to Lightning Skill Damage
    AttributeDesc{ AttributeId::extra_cold }.setRange(-50, 461),                          // +#% to Cold Skill Damage
    AttributeDesc{ AttributeId::extra_pois }.setRange(-50, 461),                          // +#% to Poison Skill Damage
    AttributeDesc{ AttributeId::dmg_elem, { AF::Damage } }.setIdiom(IDI::Brawler),        // Adds #-# Fire_perLightning_perCold Damage
    AttributeDesc{ AttributeId::dmg_elem_min, { AF::Damage } }.setIdiom(IDI::Brawler),    // +# to Minimum Fire/Lightning/Cold Damage
    AttributeDesc{ AttributeId::dmg_elem_max, { AF::Damage } }.setIdiom(IDI::Brawler),    // +# to Maximum Fire/Lightning/Cold Damage
    AttributeDesc{ AttributeId::all_stats, { AF::Stats } }.setRange(-32, 95),             // +# to all Attributes
    AttributeDesc{ AttributeId::addxp, { AF::OP } }.setRange(-50, 461),                   // +#% to Experience Gained
    AttributeDesc{ AttributeId::heal_kill },                                              // +# Life after each Kill
    AttributeDesc{ AttributeId::cheap }.setRange(1, 127),                                 // Reduces all Vendor Prices #%
    AttributeDesc{ AttributeId::rip },                                                    // Slain Monsters Rest in Peace
    AttributeDesc{ AttributeId::att_mon_percent, { AF::Attack } }.setIdiom(IDI::Brawler), // #% to Attack Rating versus [Monster Type]
    AttributeDesc{ AttributeId::dmg_mon_percent, { AF::Damage } }.setIdiom(IDI::Brawler), // #% to Damage versus [Monster Type]
    AttributeDesc{ AttributeId::kill_skill },                                             // #% Chance to cast level # [Skill] when you Kill an Enemy
    AttributeDesc{ AttributeId::death_skill },                                            // #% Chance to cast level # [Skill] when you Die
    AttributeDesc{ AttributeId::levelup_skill },                                          // #% Chance to cast level # [Skill] when you Level_Up
    AttributeDesc{ AttributeId::skill_rand, { AF::Skills } },                             // +# to [Skill] ([Class] only)
    AttributeDesc{ AttributeId::fade, { AF::Keep } },                                     // Visuals Only
    AttributeDesc{ AttributeId::levelreq },                                               // Required Level: #
    AttributeDesc{ AttributeId::ethereal, { AF::Durability } },                           // Ethereal
    AttributeDesc{ AttributeId::oskill, { AF::Skills, AF::SkillPar } }.setRange(1, 63),   // +# to [Skill]   par: [SKill], min:MinLevel, max: MaxLevel
    AttributeDesc{ AttributeId::state, { AF::Keep } },                                    // Applies a State on the unit
    AttributeDesc{ AttributeId::randclassskill, { AF::Skills, AF::NoRange } },            // +# to [Class] Skill Levels

    /// perlevel
    AttributeDesc{ AttributeId::ac_perlvl, { AF::PerLevel, AF::Defense } }.setRange(1, 63),                                // +# Defense (Based on Character Level)
    AttributeDesc{ AttributeId::ac_percent_perlvl, { AF::PerLevel, AF::Defense } }.setRange(1, 63),                        // +#% Enhanced Defense (Based on Character Level)
    AttributeDesc{ AttributeId::hp_perlvl, { AF::PerLevel, AF::Stats } }.setRange(1, 63),                                  // +# to Life (Based on Character Level)
    AttributeDesc{ AttributeId::mana_perlvl, { AF::PerLevel, AF::Stats } }.setRange(1, 63),                                // +# to Mana (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_perlvl, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63),         // +# to Maximum Damage (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_percent_perlvl, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63), // +#% Enhanced Maximum Damage (Based on Character Level)
    AttributeDesc{ AttributeId::str_perlvl, { AF::PerLevel, AF::Stats } }.setRange(1, 63),                                 // +# to Strength (Based on Character Level)
    AttributeDesc{ AttributeId::dex_perlvl, { AF::PerLevel, AF::Stats } }.setRange(1, 63),                                 // +# to Dexterity (Based on Character Level)
    AttributeDesc{ AttributeId::enr_perlvl, { AF::PerLevel, AF::Stats } }.setRange(1, 63),                                 // +# to Energy (Based on Character Level)
    AttributeDesc{ AttributeId::vit_perlvl, { AF::PerLevel, AF::Stats } }.setRange(1, 63),                                 // +# to Vitality (Based on Character Level)
    AttributeDesc{ AttributeId::att_perlvl, { AF::PerLevel, AF::Attack } }.setIdiom(IDI::Brawler).setRange(1, 63),         // +# to Attack Rating (Based on Character Level)
    AttributeDesc{ AttributeId::att_percent_perlvl, { AF::PerLevel, AF::Attack } }.setIdiom(IDI::Brawler).setRange(1, 63), // #% Bonus to Attack Rating (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_cold_perlvl, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63),    // +# to Maximum Cold Damage (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_fire_perlvl, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63),    // +# to Maximum Fire Damage (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_ltng_perlvl, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63),    // +# to Maximum Lightning Damage (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_pois_perlvl, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63),    // +# to Maximum Poison Damage (Based on Character Level)
    AttributeDesc{ AttributeId::res_cold_perlvl, { AF::PerLevel, AF::Resistance } }.setRange(1, 63),                       // Cold Resist +#% (Based on Character Level)
    AttributeDesc{ AttributeId::res_fire_perlvl, { AF::PerLevel, AF::Resistance } }.setRange(1, 63),                       // Fire Resist +#% (Based on Character Level)
    AttributeDesc{ AttributeId::res_ltng_perlvl, { AF::PerLevel, AF::Resistance } }.setRange(1, 63),                       // Lightning Resist +#% (Based on Character Level)
    AttributeDesc{ AttributeId::res_pois_perlvl, { AF::PerLevel, AF::Resistance } }.setRange(1, 63),                       // Poison Resist +#% (Based on Character Level)
    AttributeDesc{ AttributeId::abs_cold_perlvl, { AF::PerLevel, AF::DamageReduction } }.setRange(1, 63),                  // Absorbs Cold Damage (Based on Character Level)
    AttributeDesc{ AttributeId::abs_fire_perlvl, { AF::PerLevel, AF::DamageReduction } }.setRange(1, 63),                  // Absorbs Fire Damage (Based on Character Level)
    AttributeDesc{ AttributeId::abs_ltng_perlvl, { AF::PerLevel, AF::DamageReduction } }.setRange(1, 63),                  // Absorbs Lightning Damage (Based on Character Level)
    AttributeDesc{ AttributeId::abs_pois_perlvl, { AF::PerLevel, AF::DamageReduction } }.setRange(1, 63),                  // Absorbs Poison Damage (Based on Character Level)
    AttributeDesc{ AttributeId::thorns_perlvl, { AF::PerLevel } }.setRange(1, 63),                                         // Attacker Takes Damage of # (Based on Character Level)
    AttributeDesc{ AttributeId::gold_percent_perlvl, { AF::PerLevel } }.setRange(1, 63),                                   // #% Extra Gold from Monsters (Based on Character Level)
    AttributeDesc{ AttributeId::mag_percent_perlvl, { AF::PerLevel } }.setRange(1, 63),                                    // #% Better Chance of Getting Magic Items (Based on Character Level)
    AttributeDesc{ AttributeId::regen_stam_perlvl, { AF::PerLevel } }.setRange(1, 63),                                     // Heal Stamina Plus #% (Based on Character Level)
    AttributeDesc{ AttributeId::stam_perlvl, { AF::PerLevel } }.setRange(1, 63),                                           // +# Maximum Stamina (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_dem_perlvl, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63),     // +#% Damage to Demons (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_und_perlvl, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63),     // +#% Damage to Undead (Based on Character Level)
    AttributeDesc{ AttributeId::att_dem_perlvl, { AF::PerLevel, AF::Attack } }.setIdiom(IDI::Brawler).setRange(1, 63),     // +# to Attack Rating against Demons (Based on Character Level)
    AttributeDesc{ AttributeId::att_und_perlvl, { AF::PerLevel, AF::Attack } }.setIdiom(IDI::Brawler).setRange(1, 63),     // +# to Attack Rating against Undead (Based on Character Level)
    AttributeDesc{ AttributeId::crush_perlvl, { AF::PerLevel, AF::OP } }.setIdiom(IDI::Brawler).setRange(1, 63),           // #% Chance of Crushing Blow (Based on Character Level)
    AttributeDesc{ AttributeId::wounds_perlvl, { AF::PerLevel } }.setIdiom(IDI::Brawler).setRange(1, 63),                  // #% Chance of Open Wounds (Based on Character Level)
    AttributeDesc{ AttributeId::kick_perlvl, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63),        // +# Kick Damage (Based on Character Level)
    AttributeDesc{ AttributeId::deadly_perlvl, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63),      // #% Deadly Strike (Based on Character Level)

    // PD2 maps
    AttributeDesc{ AttributeId::map_glob_monsterrarity, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_extra_fire, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_density, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_addxp, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_extra_cold, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_mag_gold_percent, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_extra_ltng, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_extra_pois, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_extra_mag, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_arealevel, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_att_pierce, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_att_cast_speed, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_hp_percent, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_ed_percent, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_splash, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_openwounds, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_regen, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_crush, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_phys_as_extra_ltng, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_phys_as_extra_cold, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_phys_as_extra_fire, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_phys_as_extra_pois, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_phys_as_extra_mag, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_doll, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_succ, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_vamp, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_cow, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_horde, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_ghost, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_souls, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_fetish, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_ac_percent, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_abs_fire_percent, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_abs_ltng_percent, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_abs_mag_percent, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_abs_cold_percent, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_red_dmg, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_velocity_percent, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_regen, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_lifesteal_hp_percent, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_balance1, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_balance1, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_lightradius, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_curseresist_hp_percent, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_res_all, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_ac_percent, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_block, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_shriek, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_boss_dropcorruptedunique, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_boss_dropfacet, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_boss_droppuzzlebox, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_boss_dropskillers, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_boss_dropubermats, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_bossdropstreasure, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_dropbonus, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_dropcorrupted, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_dropethereal, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_dropsocketed, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_extra_boss, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_cannotbefrozen, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_deadlystrike, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_droparmor, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_dropcharms, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_dropcrafting, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_dropjewels, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_dropweapons, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_speed_all, { AF::PD2Map } },

    // PD2
    AttributeDesc{ AttributeId::inc_splash_radius },
    AttributeDesc{ AttributeId::leapspeed },
    AttributeDesc{ AttributeId::blood_warp_life_reduction },
    AttributeDesc{ AttributeId::extra_skele_war },
    AttributeDesc{ AttributeId::extra_skele_mage },
    AttributeDesc{ AttributeId::heal_hit },
    AttributeDesc{ AttributeId::rep_charge },
    AttributeDesc{ AttributeId::grims_extra_skele_mage },
    AttributeDesc{ AttributeId::inc_splash_radius },
    AttributeDesc{ AttributeId::ias_frw },
    AttributeDesc{ AttributeId::pierce_phys },
    AttributeDesc{ AttributeId::extra_spirits },
    AttributeDesc{ AttributeId::joust_reduction },
    AttributeDesc{ AttributeId::maxcurse },
    AttributeDesc{ AttributeId::extra_revives },
    AttributeDesc{ AttributeId::curse_res },
    AttributeDesc{ AttributeId::randclassskill2, { AF::Skills, AF::NoRange } },
    AttributeDesc{ AttributeId::randclassskill1, { AF::Skills, AF::NoRange } },
    AttributeDesc{ AttributeId::gust_reduction },
    AttributeDesc{ AttributeId::dclone_clout },
    AttributeDesc{ AttributeId::maxlevel_clout },
    AttributeDesc{ AttributeId::dev_clout },
    AttributeDesc{ AttributeId::socketed_text },
    AttributeDesc{ AttributeId::silence_fhr_ias },
    AttributeDesc{ AttributeId::extra_hydra },
    AttributeDesc{ AttributeId::extra_valk },
    AttributeDesc{ AttributeId::extra_magi },
    AttributeDesc{ AttributeId::plague_fcr_pierce },
    AttributeDesc{ AttributeId::infinityspeed },
    AttributeDesc{ AttributeId::extra_golem },
    AttributeDesc{ AttributeId::mana_steal },
    AttributeDesc{ AttributeId::block_skill },
    AttributeDesc{ AttributeId::cast_skill },
    AttributeDesc{ AttributeId::crit_strike_chance },
    AttributeDesc{ AttributeId::crit_strike_multiplier },
    AttributeDesc{ AttributeId::crush_efficiency },
    AttributeDesc{ AttributeId::curse_effectiveness },
    AttributeDesc{ AttributeId::deadly_strike_multiplier },
    AttributeDesc{ AttributeId::deep_wounds },
    AttributeDesc{ AttributeId::dmg_percent_pereth },
    AttributeDesc{ AttributeId::eaglehorn_raven },
    AttributeDesc{ AttributeId::equipped_skill },
    AttributeDesc{ AttributeId::es_efficiency },
    AttributeDesc{ AttributeId::es_efficiency_and_es_when_equip },
    AttributeDesc{ AttributeId::extra_cold_arrows },
    AttributeDesc{ AttributeId::extra_grizzly },
    AttributeDesc{ AttributeId::extra_shadow },
    AttributeDesc{ AttributeId::extra_skele_archer },
    AttributeDesc{ AttributeId::extra_spiritwolf },
    AttributeDesc{ AttributeId::joust_reduction_leorics },
    AttributeDesc{ AttributeId::joust_reduction_zeraes },
    AttributeDesc{ AttributeId::lifesteal_cap },
    AttributeDesc{ AttributeId::max_deadly },
    AttributeDesc{ AttributeId::mindmg_perenergy },
    AttributeDesc{ AttributeId::no_wolves },
    AttributeDesc{ AttributeId::sorc_skill_rand_ctc },
    AttributeDesc{ AttributeId::splash, { AF::Keep } },
    AttributeDesc{ AttributeId::splash_percent_permissinghp_percent },
    AttributeDesc{ AttributeId::str_and_vit },

    // D2R:
    AttributeDesc{ AttributeId::pierce_immunity_cold, { AF::Keep } },
    AttributeDesc{ AttributeId::pierce_immunity_fire, { AF::Keep } },
    AttributeDesc{ AttributeId::pierce_immunity_light, { AF::Keep } },
    AttributeDesc{ AttributeId::pierce_immunity_poison, { AF::Keep } },
    AttributeDesc{ AttributeId::pierce_immunity_damage, { AF::Keep } },
    AttributeDesc{ AttributeId::pierce_immunity_magic, { AF::Keep } },
    AttributeDesc{ AttributeId::charge_noconsume, { AF::Keep } },

    // D2R RotW:
    AttributeDesc{ AttributeId::pierce_mag }.setRange(1, 255),                                     // -#% to Enemy Magic Resistance
    AttributeDesc{ AttributeId::extra_mag }.setRange(-50, 461),                                    // +#% to Magic Skill Damage
    AttributeDesc{ AttributeId::war, { AF::Skills } }.setRange(1, 7).setClass(CH::war),            // +# to Warlock Skill Levels
    AttributeDesc{ AttributeId::skilltab_war, { AF::Skills } }.setRange(1, 7).setClass(CH::war),   // +# to Warlock Skill random tab Levels
    AttributeDesc{ AttributeId::magdam_rand }.setRange(1, 1),                                      // Random magic damage
    AttributeDesc{ AttributeId::roguesbow_affix1, { AF::Keep } }.setRange(1, 1).setClass(CH::ama), // +3 to Cold or Fire arrow

};

const std::string_view s_attrCodes[]{
    "ac",
    "ac-miss",
    "ac-hth",
    "red-dmg",
    "red-dmg%",
    "ac%",
    "red-mag",
    "str",
    "dex",
    "vit",
    "enr",
    "mana",
    "mana%",
    "hp",
    "hp%",
    "att",
    "block",
    "cold-min",
    "cold-max",
    "cold-len",
    "fire-min",
    "fire-max",
    "ltng-min",
    "ltng-max",
    "pois-min",
    "pois-max",
    "pois-len",
    "dmg-min",
    "dmg-max",
    "dmg%",
    "dmg-to-mana",
    "res-fire",
    "res-fire-max",
    "res-ltng",
    "res-ltng-max",
    "res-cold",
    "res-cold-max",
    "res-mag",
    "res-mag-max",
    "res-pois",
    "res-pois-max",
    "res-all",
    "res-all-max",
    "abs-fire%",
    "abs-fire",
    "abs-ltng%",
    "abs-ltng",
    "abs-mag%",
    "abs-mag",
    "abs-cold%",
    "abs-cold",
    "dur",
    "dur%",
    "regen",
    "thorns",
    "swing3",
    "gold%",
    "mag%",
    "knock",
    "regen-stam",
    "regen-mana",
    "stam",
    "manasteal",
    "lifesteal",
    "ama",
    "pal",
    "nec",
    "sor",
    "bar",
    "light",
    "ease",
    "move3",
    "balance3",
    "block3",
    "cast3",
    "res-pois-len",
    "dmg",
    "howl",
    "stupidity",
    "ignore-ac",
    "reduce-ac",
    "noheal",
    "half-freeze",
    "att%",
    "dmg-ac",
    "dmg-demon",
    "dmg-undead",
    "att-demon",
    "att-undead",
    "fireskill",
    "poisskill",
    "coldskill",
    "magskill",
    "ltngskill",
    "allskills",
    "light-thorns",
    "freeze",
    "openwounds",
    "crush",
    "kick",
    "mana-kill",
    "demon-heal",
    "bloody",
    "deadly",
    "slow",
    "nofreeze",
    "stamdrain",
    "reanimate",
    "pierce",
    "magicarrow",
    "explosivearrow",
    "dru",
    "ass",
    "skill",
    "skilltab",
    "aura",
    "att-skill",
    "hit-skill",
    "gethit-skill",
    "sock",
    "dmg-fire",
    "dmg-ltng",
    "dmg-mag",
    "dmg-cold",
    "dmg-pois",
    "dmg-norm",
    "rep-dur",
    "rep-quant",
    "stack",
    "pierce-fire",
    "pierce-ltng",
    "pierce-cold",
    "pierce-pois",
    "indestruct",
    "charged",
    "extra-fire",
    "extra-ltng",
    "extra-cold",
    "extra-pois",
    "dmg-elem",
    "dmg-elem-min",
    "dmg-elem-max",
    "all-stats",
    "addxp",
    "heal-kill",
    "cheap",
    "rip",
    "att-mon%",
    "dmg-mon%",
    "kill-skill",
    "death-skill",
    "levelup-skill",
    "skill-rand",
    "fade",
    "levelreq",
    "ethereal",
    "oskill",
    "state",
    "randclassskill",

    /// perlevel
    "ac/lvl",
    "ac%/lvl",
    "hp/lvl",
    "mana/lvl",
    "dmg/lvl",
    "dmg%/lvl",
    "str/lvl",
    "dex/lvl",
    "enr/lvl",
    "vit/lvl",
    "att/lvl",
    "att%/lvl",
    "dmg-cold/lvl",
    "dmg-fire/lvl",
    "dmg-ltng/lvl",
    "dmg-pois/lvl",
    "res-cold/lvl",
    "res-fire/lvl",
    "res-ltng/lvl",
    "res-pois/lvl",
    "abs-cold/lvl",
    "abs-fire/lvl",
    "abs-ltng/lvl",
    "abs-pois/lvl",
    "thorns/lvl",
    "gold%/lvl",
    "mag%/lvl",
    "regen-stam/lvl",
    "stam/lvl",
    "dmg-dem/lvl",
    "dmg-und/lvl",
    "att-dem/lvl",
    "att-und/lvl",
    "crush/lvl",
    "wounds/lvl",
    "kick/lvl",
    "deadly/lvl",

    /// PD2 Maps
    "map-glob-monsterrarity",
    "map-mon-extra-fire",
    "map-glob-density",
    "map-play-addxp",
    "map-mon-extra-cold",
    "map-play-mag-gold%",
    "map-mon-extra-ltng",
    "map-mon-extra-pois",
    "map-mon-extra-mag",
    "map-glob-arealevel",
    "map-mon-att-pierce",
    "map-mon-att-cast-speed",
    "map-mon-hp%",
    "map-mon-ed%",
    "map-mon-splash",
    "map-mon-openwounds",
    "map-play-regen",
    "map-mon-crush",
    "map-mon-phys-as-extra-ltng",
    "map-mon-phys-as-extra-cold",
    "map-mon-phys-as-extra-fire",
    "map-mon-phys-as-extra-pois",
    "map-mon-phys-as-extra-mag",
    "map-glob-add-mon-doll",
    "map-glob-add-mon-succ",
    "map-glob-add-mon-vamp",
    "map-glob-add-mon-cow",
    "map-glob-add-mon-horde",
    "map-glob-add-mon-ghost",
    "map-glob-add-mon-souls",
    "map-glob-add-mon-fetish",
    "map-mon-ac%",
    "map-mon-abs-fire%",
    "map-mon-abs-ltng%",
    "map-mon-abs-mag%",
    "map-mon-abs-cold%",
    "map-mon-red-dmg",
    "map-mon-velocity%",
    "map-mon-regen",
    "map-mon-lifesteal-hp%",
    "map-mon-balance1",
    "map-play-balance1",
    "map-play-lightradius",
    "map-mon-curseresist-hp%",
    "map-play-res-all",
    "map-play-ac%",
    "map-play-block",
    "map-glob-add-mon-shriek",
    "map-glob-boss-dropcorruptedunique",
    "map-glob-boss-dropfacet",
    "map-glob-boss-droppuzzlebox",
    "map-glob-boss-dropskillers",
    "map-glob-boss-dropubermats",
    "map-glob-bossdropstreasure",
    "map-glob-dropbonus",
    "map-glob-dropcorrupted",
    "map-glob-dropethereal",
    "map-glob-dropsocketed",
    "map-glob-extra-boss",
    "map-mon-cannotbefrozen",
    "map-mon-deadlystrike",
    "map-mon-droparmor",
    "map-mon-dropcharms",
    "map-mon-dropcrafting",
    "map-mon-dropjewels",
    "map-mon-dropweapons",
    "map-play-speed-all",

    /// PD2
    "inc-splash-radius",
    "leapspeed",
    "blood-warp-life-reduction",
    "extra-skele-war",
    "extra-skele-mage",
    "heal-hit",
    "rep-charge",
    "grims-extra-skele-mage",
    "inc_splash_radius",
    "ias-frw",
    "pierce-phys",
    "extra-spirits",
    "joust-reduction",
    "maxcurse",
    "extra-revives",
    "curse-res",
    "randclassskill2",
    "randclassskill1",
    "gust-reduction",
    "dclone-clout",
    "maxlevel-clout",
    "dev-clout",
    "socketed-text",
    "silence-fhr-ias",
    "extra-hydra",
    "extra-valk",
    "extra-magi",
    "plague-fcr-pierce",
    "infinityspeed",
    "extra-golem",
    "mana-steal",
    "block-skill",
    "cast-skill",
    "crit-strike-chance",
    "crit-strike-multiplier",
    "crush-efficiency",
    "curse-effectiveness",
    "deadly-strike-multiplier",
    "deep-wounds",
    "dmg%/eth",
    "eaglehorn-raven",
    "equipped-skill",
    "es-efficiency",
    "es-efficiency-and-es-when-equip",
    "extra-cold-arrows",
    "extra-grizzly",
    "extra-shadow",
    "extra-skele-archer",
    "extra-spiritwolf",
    "joust-reduction-leorics",
    "joust-reduction-zeraes",
    "lifesteal-cap",
    "max-deadly",
    "mindmg/energy",
    "no-wolves",
    "sorc-skill-rand-ctc",
    "splash",
    "splash%/missinghp%",
    "str-and-vit",

    /// D2R
    "pierce-immunity-cold",
    "pierce-immunity-fire",
    "pierce-immunity-light",
    "pierce-immunity-poison",
    "pierce-immunity-damage",
    "pierce-immunity-magic",
    "charge-noconsume",

    /// D2R - ROTW
    "pierce-mag",
    "extra-mag",
    "war",
    "skilltab-war",
    "magdam-rand",
    "roguesbow-affix1",

};

const std::unordered_map<std::string_view, AttributeId> s_attributesIndex = [] {
    std::unordered_map<std::string_view, AttributeId> result;
    int                                               i = 0;
    for (const auto& attr : s_attrCodes) {
        result[attr] = static_cast<AttributeId>(i++);
    }
    return result;
}();

const std::map<std::string, std::string> s_aliased{
    { "swing", "swing3" },
    { "swing1", "swing3" },
    { "swing2", "swing3" },
    { "move", "move3" },
    { "move1", "move3" },
    { "move2", "move3" },
    { "balance", "balance3" },
    { "balance1", "balance3" },
    { "balance2", "balance3" },
    { "block", "block3" },
    { "block1", "block3" },
    { "block2", "block3" },
    { "cast", "cast3" },
    { "cast1", "cast3" },
    { "cast2", "cast3" },
};

AttributeId attributesIndexValue(const std::string& code)
{
    auto it = s_attributesIndex.find(code);
    return it == s_attributesIndex.cend() ? AttributeId::INVALID : (it->second);
}

std::string uniqueKey(AttributeId code, const std::string& par)
{
    if (hasFlag(code, AF::SkillPar))
        return idToString(code) + "-" + par;

    return idToString(code);
}

}

const AttributeDesc& getAttributeDesc(AttributeId code)
{
    static const AttributeDesc s_stub{};
    const int                  index = static_cast<int>(code);
    if (index < 0)
        return s_stub;
    return s_attributes[index];
}

bool UniqueAttributeChecker::add(AttributeId code, const std::string& par)
{
    std::string key = uniqueKey(code, par);
    if (m_data.contains(key))
        return false;
    m_data.insert(key);
    return true;
}

int powerMultiply(AttributeId code, int value, int powerMultPercent)
{
    if (code <= AttributeId::INVALID)
        return value;

    const AttributeDesc& desc     = getAttributeDesc(code);
    const bool           hasRange = desc.flags.contains(AF::HasRange);
    if (!hasRange)
        return value;
    int powerPercent = 100;
    if (desc.flags.contains(AF::Skills)
        || desc.flags.contains(AF::DamageReduction)
        || desc.flags.contains(AF::Speed))
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
bool isMinMaxRange(AttributeId code)
{
    return hasFlag(code, AF::HasRange);
}

bool isPositiveRange(AttributeId code)
{
    return !hasFlag(code, AF::Negative);
}

std::string deAliasCode(std::string code)
{
    code    = Mernel::strToLower(code);
    auto it = s_aliased.find(code);
    return it == s_aliased.cend() ? code : it->second;
}

bool hasFlag(AttributeId code, AttributeFlag flag)
{
    return getAttributeDesc(code).flags.contains(flag);
}

AttributeId getAttributeId(const std::string& code)
{
    if (code.empty() || code.starts_with("*"))
        return AttributeId::SKIP;

    return attributesIndexValue(deAliasCode(code));
}

std::string idToString(AttributeId code)
{
    if (code == AttributeId::SKIP)
        return "SKIP";
    if (code <= AttributeId::INVALID)
        return "INVALID";
    return std::string(s_attrCodes[static_cast<int>(code)]);
}

Character getClassId(const std::string& code)
{
    auto it = s_classes.find(code);
    if (it != s_classes.cend())
        return it->second;
    return Character::none;
}

std::string classTypeName(Character type)
{
    for (auto& [key, val] : s_classes)
        if (val == type)
            return key;
    return "none";
}

std::string idiomTypeName(AttributeIdiom type)
{
    if (type == AttributeIdiom::Brawler)
        return "brawl";
    if (type == AttributeIdiom::Caster)
        return "cast";
    return "any";
}

}
