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
#include <map>

#include <cassert>

namespace D2ModGen {

namespace {
using AF  = AttributeFlag;
using AG  = AttributeGroup;
using CH  = Character;
using IDI = AttributeIdiom;
using SL  = SlotType;
using P   = ParMode;
using R   = RangeMode;
using E   = Element;

const AttributeDesc s_stub_invalid{ .code = AttributeId::INVALID };
const AttributeDesc s_stub_skip{ .code = AttributeId::SKIP };

struct AttributeDescStr {
    AttributeId      id;
    std::string_view code;
    std::string_view enumStr;
};

struct CharacterDesc {
    Character        clas;
    std::string_view code;
    std::string_view enumStr;
};

const CharacterDesc s_classes[]{
    { Character::Ama, "ama", "Ama" },
    { Character::Sor, "sor", "Sor" },
    { Character::Nec, "nec", "Nec" },
    { Character::Pal, "pal", "Pal" },
    { Character::Bar, "bar", "Bar" },
    { Character::Dru, "dru", "Dru" },
    { Character::Ass, "ass", "Ass" },
    { Character::War, "war", "War" },
};

struct IdiomDesc {
    AttributeIdiom   idi;
    std::string_view code;
};

const IdiomDesc s_idioms[]{
    { AttributeIdiom::None, "None" },

    { AttributeIdiom::Caster, "Caster" },
    { AttributeIdiom::Brawler, "Brawler" },
};

struct ElementDesc {
    Element          elem;
    std::string_view code;
    std::string_view enumStr;
};

const ElementDesc s_elements[]{
    { Element::None, "none", "None" },

    { Element::Ltng, "ltng", "Ltng" },
    { Element::Fire, "fire", "Fire" },
    { Element::Cold, "cold", "Cold" },
    { Element::Mag, "mag", "Mag" },
    { Element::Pois, "pois", "Pois" },
    { Element::Phys, "phys", "Phys" },
    { Element::Burn, "burn", "Burn" },
    { Element::Frze, "frze", "Frze" },
};

struct SlotTypeDesc {
    SlotType         slot;
    std::string_view code;
};

const SlotTypeDesc s_slotTypes[]{
    { SlotType::None, "None" },
    { SlotType::MainArmor, "MainArmor" },
    { SlotType::OtherArmor, "OtherArmor" },
    { SlotType::Ring, "Ring" },
    { SlotType::Amulet, "Amulet" },
    { SlotType::Weapon, "Weapon" },
    { SlotType::Charm, "Charm" },
    { SlotType::Socketable, "Socketable" },
    { SlotType::Set, "Set" },
    { SlotType::PD2Map, "PD2Map" },
};

struct ParModeDesc {
    ParMode          mode;
    std::string_view code;
};

const ParModeDesc s_parTypes[]{
    { ParMode::None, "None" },
    { ParMode::PureValue, "PureValue" },
    { ParMode::Skill, "Skill" },
    { ParMode::SkillTab, "SkillTab" },
    { ParMode::Monster, "Monster" },
};
struct RangeModeDesc {
    RangeMode        mode;
    std::string_view code;
};

const RangeModeDesc s_rangeTypes[]{
    { RangeMode::None, "None" },
    { RangeMode::Range, "Range" },
    { RangeMode::DoubleParam, "DoubleParam" },
    { RangeMode::Flag, "Flag" },
};

struct AttributeGroupDesc {
    AttributeGroup   group;
    std::string_view code;
};

const AttributeGroupDesc s_groupTypes[]{
    { AttributeGroup::None, "None" },

    { AttributeGroup::AllClassSkills, "AllClassSkills" },
    { AttributeGroup::Res, "Res" },
    { AttributeGroup::ResMax, "ResMax" },
    { AttributeGroup::ResPerLvl, "ResPerLvl" },
    { AttributeGroup::Absorb, "Absorb" },
    { AttributeGroup::AbsorbPercent, "AbsorbPercent" },
    { AttributeGroup::EleSkill, "EleSkill" },
    { AttributeGroup::ElePierce, "ElePierce" },
    { AttributeGroup::EleExtra, "EleExtra" },
    { AttributeGroup::EleDmgPerlvl, "EleDmgPerlvl" },
    { AttributeGroup::AbsorbPerLvl, "AbsorbPerLvl" },
    { AttributeGroup::StrDex, "StrDex" },
    { AttributeGroup::VitEnr, "VitEnr" },
    { AttributeGroup::AcFlat, "AcFlat" },
    { AttributeGroup::DmgUndeadDemon, "DmgUndeadDemon" },
    { AttributeGroup::DmgUndeadDemonPerLvl, "DmgUndeadDemonPerLvl" },
    { AttributeGroup::StatPerLvl, "StatPerLvl" },
    { AttributeGroup::HpManaPerLvl, "HpManaPerLvl" },
    { AttributeGroup::AttUndeadDemon, "AttUndeadDemon" },
    { AttributeGroup::AttUndeadDemonPerLvl, "AttUndeadDemonPerLvl" },
    { AttributeGroup::DeadlyOWCrush, "DeadlyOWCrush" },
    { AttributeGroup::DeadlyOWCrushPerLvl, "DeadlyOWCrushPerLvl" },
    { AttributeGroup::ManaLifePerKill, "ManaLifePerKill" },
    { AttributeGroup::ManaLifeSteal, "ManaLifeSteal" },
    { AttributeGroup::PdrMdrFlat, "PdrMdrFlat" },
    { AttributeGroup::Sunder, "Sunder" },
    { AttributeGroup::OffensiveSkillProc, "OffensiveSkillProc" },
    { AttributeGroup::DefensiveSkillProc, "DefensiveSkillProc" },
};

const AttributeDesc s_attributes[]{

    AttributeDesc{ AttributeId::ac, P::None, R::Range, { AF::Defense } }.setRange(-10, 2037).setGroup(AG::AcFlat),                                               // +# Defense
    AttributeDesc{ AttributeId::ac_miss, P::None, R::Range, { AF::Defense } }.setRange(1, 511).setGroup(AG::AcFlat),                                             // +# Defense vs. Missile
    AttributeDesc{ AttributeId::ac_hth, P::None, R::Range, { AF::Defense } }.setRange(1, 255).setGroup(AG::AcFlat),                                              // +# Defense vs. Melee
    AttributeDesc{ AttributeId::red_dmg, P::None, R::Range, { AF::DamageReduction } }.setRange(1, 63).setGroup(AG::PdrMdrFlat),                                  // Damage Reduced by #
    AttributeDesc{ AttributeId::red_dmg_percent, P::None, R::Range, { AF::Resistance } }.setRange(-25, 255).setGroup(AG::None),                                  // Damage Reduced by #%
    AttributeDesc{ AttributeId::ac_percent, P::None, R::Range, { AF::Defense } }.setRange(1, 511).setGroup(AG::None),                                            // +#% Enhanced Defense
    AttributeDesc{ AttributeId::red_mag, P::None, R::Range, { AF::DamageReduction } }.setRange(1, 63).setGroup(AG::PdrMdrFlat),                                  // Magic Damage Reduced by #
    AttributeDesc{ AttributeId::str, P::None, R::Range, { AF::Stats } }.setRange(-32, 223).setGroup(AG::StrDex),                                                 // +# to Strength
    AttributeDesc{ AttributeId::dex, P::None, R::Range, { AF::Stats } }.setRange(-32, 95).setGroup(AG::StrDex),                                                  // +# to Dexterity
    AttributeDesc{ AttributeId::vit, P::None, R::Range, { AF::Stats } }.setRange(-32, 95).setGroup(AG::VitEnr),                                                  // +# to Vitality
    AttributeDesc{ AttributeId::enr, P::None, R::Range, { AF::Stats } }.setRange(-32, 95).setGroup(AG::VitEnr),                                                  // +# to Energy
    AttributeDesc{ AttributeId::mana, P::None, R::Range, { AF::Stats } }.setRange(-32, 223).setGroup(AG::None),                                                  // +# to Mana
    AttributeDesc{ AttributeId::mana_percent, P::None, R::Range, { AF::Stats } }.setRange(-10, 53).setGroup(AG::None),                                           // Increase Maximum Mana #%
    AttributeDesc{ AttributeId::hp, P::None, R::Range, { AF::Stats } }.setRange(-32, 479).setGroup(AG::None),                                                    // +# to Life
    AttributeDesc{ AttributeId::hp_percent, P::None, R::Range, { AF::Stats } }.setRange(-10, 53).setGroup(AG::None),                                             // Increase Maximum Life #%
    AttributeDesc{ AttributeId::att, P::None, R::Range, { AF::Attack } }.setIdiom(IDI::Brawler).setRange(1, 1023).setGroup(AG::None),                            // +# to Attack Rating
    AttributeDesc{ AttributeId::block, P::None, R::Range }.setGroup(AG::None),                                                                                   // #% Increased Chance of Blocking
    AttributeDesc{ AttributeId::cold_min, P::None, R::Range, { AF::Damage } }.setElement(E::Cold).setIdiom(IDI::Brawler).setRange(1, 255).setGroup(AG::None),    // +# to Minimum Cold Damage
    AttributeDesc{ AttributeId::cold_max, P::None, R::Range, { AF::Damage } }.setElement(E::Cold).setIdiom(IDI::Brawler).setRange(1, 511).setGroup(AG::None),    // +# to Maximum Cold Damage
    AttributeDesc{ AttributeId::cold_len, P::None, R::Range, { AF::Damage } }.setElement(E::Cold).setIdiom(IDI::Brawler).setRange(1, 255).setGroup(AG::None),    // # of Frames (25 Frames = 1 Second)
    AttributeDesc{ AttributeId::fire_min, P::None, R::Range, { AF::Damage } }.setElement(E::Fire).setIdiom(IDI::Brawler).setRange(1, 255).setGroup(AG::None),    // +# to Minimum Fire Damage
    AttributeDesc{ AttributeId::fire_max, P::None, R::Range, { AF::Damage } }.setElement(E::Fire).setIdiom(IDI::Brawler).setRange(1, 511).setGroup(AG::None),    // +# to Maximum Fire Damage
    AttributeDesc{ AttributeId::ltng_min, P::None, R::Range, { AF::Damage } }.setElement(E::Ltng).setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::None),     // +# to Minimum Lightning Damage
    AttributeDesc{ AttributeId::ltng_max, P::None, R::Range, { AF::Damage } }.setElement(E::Ltng).setIdiom(IDI::Brawler).setRange(1, 1023).setGroup(AG::None),   // +# to Maximum Lightning Damage
    AttributeDesc{ AttributeId::pois_min, P::None, R::Range, { AF::Damage } }.setElement(E::Pois).setIdiom(IDI::Brawler).setRange(1, 1023).setGroup(AG::None),   // +# to Minimum Poison Damage
    AttributeDesc{ AttributeId::pois_max, P::None, R::Range, { AF::Damage } }.setElement(E::Pois).setIdiom(IDI::Brawler).setRange(1, 1023).setGroup(AG::None),   // +# to Maximum Poison Damage
    AttributeDesc{ AttributeId::pois_len, P::None, R::Range, { AF::Damage } }.setElement(E::Pois).setIdiom(IDI::Brawler).setRange(1, 255).setGroup(AG::None),    // # of Frames (25 Frames = 1 Second)
    AttributeDesc{ AttributeId::dmg_min, P::None, R::Range, { AF::Damage } }.setElement(E::Phys).setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::None),      // +# to Minimum Damage
    AttributeDesc{ AttributeId::dmg_max, P::None, R::Range, { AF::Damage } }.setElement(E::Phys).setIdiom(IDI::Brawler).setRange(1, 127).setGroup(AG::None),     // +# to Maximum Damage
    AttributeDesc{ AttributeId::dmg_percent, P::None, R::Range, { AF::Damage } }.setElement(E::Phys).setIdiom(IDI::Brawler).setRange(1, 511).setGroup(AG::None), // +#% Enhanced Damage
    AttributeDesc{ AttributeId::dmg_to_mana, P::None, R::Range }.setGroup(AG::None),                                                                             // #% Damage Taken Goes To Mana
    AttributeDesc{ AttributeId::res_fire, P::None, R::Range, { AF::Resistance } }.setRange(-90, 205).setGroup(AG::Res),                                          // Fire Resist +#%
    AttributeDesc{ AttributeId::res_fire_max, P::None, R::Range, { AF::Resistance } }.setRange(1, 31).setGroup(AG::ResMax),                                      // +#% to Maximum Fire Resist
    AttributeDesc{ AttributeId::res_ltng, P::None, R::Range, { AF::Resistance } }.setRange(-90, 205).setGroup(AG::Res),                                          // Lightning Resist +#%
    AttributeDesc{ AttributeId::res_ltng_max, P::None, R::Range, { AF::Resistance } }.setRange(1, 31).setGroup(AG::ResMax),                                      // +#% to Maximum Lightning Resist
    AttributeDesc{ AttributeId::res_cold, P::None, R::Range, { AF::Resistance } }.setRange(-90, 205).setGroup(AG::Res),                                          // Cold Resist +#%
    AttributeDesc{ AttributeId::res_cold_max, P::None, R::Range, { AF::Resistance } }.setRange(1, 31).setGroup(AG::ResMax),                                      // +#% to Maximum Cold Resist
    AttributeDesc{ AttributeId::res_mag, P::None, R::Range, { AF::Resistance } }.setRange(-90, 255).setGroup(AG::Res),                                           // Magic Resist +#%
    AttributeDesc{ AttributeId::res_mag_max, P::None, R::Range, { AF::Resistance } }.setRange(1, 31).setGroup(AG::ResMax),                                       // +#% to Maximum Magic Resist
    AttributeDesc{ AttributeId::res_pois, P::None, R::Range, { AF::Resistance } }.setRange(-90, 205).setGroup(AG::Res),                                          // Poison Resist +#%
    AttributeDesc{ AttributeId::res_pois_max, P::None, R::Range, { AF::Resistance } }.setRange(1, 31).setGroup(AG::ResMax),                                      // +#% to Maximum Poison Resist
    AttributeDesc{ AttributeId::res_all, P::None, R::Range, { AF::Resistance } }.setRange(-50, 205).setGroup(AG::None),                                          // All Resistances +#
    AttributeDesc{ AttributeId::res_all_max, P::None, R::Range, { AF::Resistance } }.setRange(1, 31).setGroup(AG::None),                                         // Currently Not Used
    AttributeDesc{ AttributeId::abs_fire_percent, P::None, R::Range, { AF::Resistance } }.setRange(1, 127).setGroup(AG::AbsorbPercent),                          // +# Fire Absorb
    AttributeDesc{ AttributeId::abs_fire, P::None, R::Range, { AF::DamageReduction } }.setRange(1, 127).setGroup(AG::Absorb),                                    // Fire Absorb #%
    AttributeDesc{ AttributeId::abs_ltng_percent, P::None, R::Range, { AF::Resistance } }.setRange(1, 127).setGroup(AG::AbsorbPercent),                          // +# Lightning Absorb
    AttributeDesc{ AttributeId::abs_ltng, P::None, R::Range, { AF::DamageReduction } }.setRange(1, 127).setGroup(AG::Absorb),                                    // Lightning Absorb #%
    AttributeDesc{ AttributeId::abs_mag_percent, P::None, R::Range, { AF::Resistance } }.setRange(1, 127).setGroup(AG::AbsorbPercent),                           // +# Magic Absorb
    AttributeDesc{ AttributeId::abs_mag, P::None, R::Range, { AF::DamageReduction } }.setRange(1, 127).setGroup(AG::Absorb),                                     // Magic Absorb #%
    AttributeDesc{ AttributeId::abs_cold_percent, P::None, R::Range, { AF::Resistance } }.setRange(1, 127).setGroup(AG::AbsorbPercent),                          // +# Cold Absorb
    AttributeDesc{ AttributeId::abs_cold, P::None, R::Range, { AF::DamageReduction } }.setRange(1, 127).setGroup(AG::Absorb),                                    // Cold Absorb #%
    AttributeDesc{ AttributeId::dur, P::None, R::Range, { AF::Durability } }.setGroup(AG::None),                                                                 // Durability: # of #
    AttributeDesc{ AttributeId::dur_percent, P::None, R::Range, { AF::Durability } }.setGroup(AG::None),                                                         // Increase Maximum Durability #%
    AttributeDesc{ AttributeId::regen, P::None, R::Range }.setRange(-30, 33).setGroup(AG::None),                                                                 // Replenish Life +#
    AttributeDesc{ AttributeId::thorns, P::None, R::Range }.setRange(1, 127).setGroup(AG::None),                                                                 // Attacker Takes Damage of #
    AttributeDesc{ AttributeId::swing3, P::None, R::Range, { AF::Speed } }.setRange(-20, 100).setRounding(5).setGroup(AG::None),                                 // +#% Increased Attack Speed
    AttributeDesc{ AttributeId::gold_percent, P::None, R::Range }.setRange(-100, 411).setGroup(AG::None),                                                        // #% Extra Gold from Monsters
    AttributeDesc{ AttributeId::mag_percent, P::None, R::Range }.setRange(-100, 155).setGroup(AG::None),                                                         // #% Better Chance of Getting Magic Items
    AttributeDesc{ AttributeId::knock, P::None, R::Flag }.setRequiredSlots(SlotType::Weapon).setGroup(AG::None),                                                 // Knockback
    AttributeDesc{ AttributeId::regen_stam, P::None, R::Range }.setGroup(AG::None),                                                                              // Heal Stamina Plus #%
    AttributeDesc{ AttributeId::regen_mana, P::None, R::Range }.setGroup(AG::None),                                                                              // Regenerate Mana #%
    AttributeDesc{ AttributeId::stam, P::None, R::Range }.setGroup(AG::None),                                                                                    // +# Maximum Stamina
    AttributeDesc{ AttributeId::manasteal, P::None, R::Range, { AF::Leech } }.setIdiom(IDI::Brawler).setRange(1, 127).setGroup(AG::ManaLifeSteal),               // #% Mana stolen per hit
    AttributeDesc{ AttributeId::lifesteal, P::None, R::Range, { AF::Leech } }.setIdiom(IDI::Brawler).setRange(1, 127).setGroup(AG::ManaLifeSteal),               // #% Life stolen per hit
    AttributeDesc{ AttributeId::ama, P::None, R::Range, { AF::Skills } }.setRange(1, 7).setClass(CH::Ama).setGroup(AG::AllClassSkills),                          // +# to Amazon Skill Levels
    AttributeDesc{ AttributeId::pal, P::None, R::Range, { AF::Skills } }.setRange(1, 7).setClass(CH::Pal).setGroup(AG::AllClassSkills),                          // +# to Paladin Skill Levels
    AttributeDesc{ AttributeId::nec, P::None, R::Range, { AF::Skills } }.setRange(1, 7).setClass(CH::Nec).setGroup(AG::AllClassSkills),                          // +# to Necromancer Skill Levels
    AttributeDesc{ AttributeId::sor, P::None, R::Range, { AF::Skills } }.setRange(1, 7).setClass(CH::Sor).setGroup(AG::AllClassSkills),                          // +# to Sorceress Skill Levels
    AttributeDesc{ AttributeId::bar, P::None, R::Range, { AF::Skills } }.setRange(1, 7).setClass(CH::Bar).setGroup(AG::AllClassSkills),                          // +#  to Barbarian Skill Levels
    AttributeDesc{ AttributeId::light, P::None, R::Range }.setRange(-4, 11).setGroup(AG::None),                                                                  // +# to Light Radius
    AttributeDesc{ AttributeId::ease, P::None, R::Range, { AF::Negative } }
        .setRequiredSlots({ SL::Weapon, SL::MainArmor, SL::OtherArmor })
        .setGroup(AG::None),                                                                                                                                    // Requirements -#%
    AttributeDesc{ AttributeId::move3, P::None, R::Range, { AF::Speed } }.setRange(-20, 100).setRounding(5).setGroup(AG::None),                                 // +#% Faster Run/Walk
    AttributeDesc{ AttributeId::balance3, P::None, R::Range, { AF::Speed } }.setRange(-20, 100).setRounding(5).setGroup(AG::None),                              // +#% Faster Hit Recovery
    AttributeDesc{ AttributeId::block3, P::None, R::Range, { AF::Speed } }.setRange(-20, 100).setRounding(5).setGroup(AG::None),                                // +#% Faster Block Rate
    AttributeDesc{ AttributeId::cast3, P::None, R::Range, { AF::Speed } }.setIdiom(IDI::Caster).setRange(-20, 100).setRounding(5).setGroup(AG::None),           // +#% Faster Cast Rate
    AttributeDesc{ AttributeId::res_pois_len, P::None, R::Range, { AF::Resistance } }.setRange(-20, 235).setGroup(AG::None),                                    // Poison Length Reduced by #%
    AttributeDesc{ AttributeId::dmg, P::None, R::Range, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(-20, 491).setGroup(AG::None),                          // Damage +#
    AttributeDesc{ AttributeId::howl, P::None, R::DoubleParam }.setGroup(AG::None),                                                                             // Hit Causes Monster to Flee #%
    AttributeDesc{ AttributeId::stupidity, P::None, R::DoubleParam }.setGroup(AG::None),                                                                        // Hit Blinds Target +#
    AttributeDesc{ AttributeId::ignore_ac, P::None, R::Flag, { AF::Attack } }.setIdiom(IDI::Brawler).setGroup(AG::None),                                        // Ignore Target's Defense
    AttributeDesc{ AttributeId::reduce_ac, P::None, R::Range, { AF::Attack } }.setIdiom(IDI::Brawler).setGroup(AG::None),                                       // #% Target Defense
    AttributeDesc{ AttributeId::noheal, P::None, R::Flag }.setIdiom(IDI::Brawler).setGroup(AG::None),                                                           // Prevent Monster Heal
    AttributeDesc{ AttributeId::half_freeze, P::None, R::Flag }.setGroup(AG::None),                                                                             // Half Freeze Duration
    AttributeDesc{ AttributeId::att_percent, P::None, R::Range, { AF::Attack } }.setIdiom(IDI::Brawler).setGroup(AG::None),                                     // #% Bonus to Attack Rating
    AttributeDesc{ AttributeId::dmg_ac, P::None, R::Range, { AF::Attack, AF::Negative } }.setIdiom(IDI::Brawler).setGroup(AG::None),                            // -# to Monster Defense Per Hit
    AttributeDesc{ AttributeId::dmg_demon, P::None, R::Range, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(-20, 491).setGroup(AG::DmgUndeadDemon),          // +#% Damage to Demons
    AttributeDesc{ AttributeId::dmg_undead, P::None, R::Range, { AF::Damage } }.setIdiom(IDI::Brawler).setRange(-20, 491).setGroup(AG::DmgUndeadDemon),         // +#% Damage to Undead
    AttributeDesc{ AttributeId::att_demon, P::None, R::Range, { AF::Attack } }.setIdiom(IDI::Brawler).setGroup(AG::AttUndeadDemon),                             // +# to Attack Rating against Demons
    AttributeDesc{ AttributeId::att_undead, P::None, R::Range, { AF::Attack } }.setIdiom(IDI::Brawler).setGroup(AG::AttUndeadDemon),                            // +# to Attack Rating against Undead
    AttributeDesc{ AttributeId::fireskill, P::None, R::Range, { AF::Skills } }.setElement(E::Fire).setIdiom(IDI::Caster).setRange(1, 7).setGroup(AG::EleSkill), // +# to Fire Skills
    AttributeDesc{ AttributeId::poisskill, P::None, R::Range, { AF::Skills } }.setElement(E::Pois).setIdiom(IDI::Caster).setRange(1, 7).setGroup(AG::EleSkill),
    AttributeDesc{ AttributeId::coldskill, P::None, R::Range, { AF::Skills } }.setElement(E::Cold).setIdiom(IDI::Caster).setRange(1, 7).setGroup(AG::EleSkill),
    AttributeDesc{ AttributeId::magskill, P::None, R::Range, { AF::Skills } }.setElement(E::Mag).setIdiom(IDI::Caster).setRange(1, 7).setGroup(AG::EleSkill),
    AttributeDesc{ AttributeId::ltngskill, P::None, R::Range, { AF::Skills } }.setElement(E::Ltng).setIdiom(IDI::Caster).setRange(1, 7).setGroup(AG::EleSkill),
    AttributeDesc{ AttributeId::allskills, P::None, R::Range, { AF::Skills } }.setRange(1, 7).setGroup(AG::None),                           // +# to All Skills
    AttributeDesc{ AttributeId::light_thorns, P::None, R::Range }.setGroup(AG::None),                                                       // Attacker Takes Lightning Damage of #
    AttributeDesc{ AttributeId::freeze, P::None, R::Range }.setIdiom(IDI::Brawler).setGroup(AG::None),                                      // Freezes Target +#
    AttributeDesc{ AttributeId::openwounds, P::None, R::Range }.setIdiom(IDI::Brawler).setRange(1, 127).setGroup(AG::DeadlyOWCrush),        // #% Chance of Open Wounds
    AttributeDesc{ AttributeId::crush, P::None, R::Range, { AF::OP } }.setIdiom(IDI::Brawler).setRange(1, 127).setGroup(AG::DeadlyOWCrush), // #% Chance of Crushing Blow
    AttributeDesc{ AttributeId::kick, P::None, R::Range, { AF::Damage } }.setIdiom(IDI::Brawler).setGroup(AG::None),                        // +# Kick Damage
    AttributeDesc{ AttributeId::mana_kill, P::None, R::Range }.setRange(1, 127).setGroup(AG::ManaLifePerKill),                              // +# to Mana after each Kill
    AttributeDesc{ AttributeId::demon_heal, P::None, R::Range }.setGroup(AG::ManaLifePerKill),                                              // +# Life after each Demon Kill
    AttributeDesc{ AttributeId::bloody, P::None, R::DoubleParam }.setGroup(AG::None),                                                       // Visuals Only
    AttributeDesc{ AttributeId::deadly, P::None, R::Range, { AF::Damage } }.setIdiom(IDI::Brawler).setGroup(AG::DeadlyOWCrush),             // #% Deadly Strike
    AttributeDesc{ AttributeId::slow, P::None, R::Range }.setIdiom(IDI::Brawler).setRange(1, 127).setGroup(AG::None),                       // Slows Target by #%
    AttributeDesc{ AttributeId::nofreeze, P::None, R::Flag, { AF::OP } }.setGroup(AG::None),                                                // Cannot Be Frozen
    AttributeDesc{ AttributeId::stamdrain, P::None, R::Range }.setGroup(AG::None),                                                          // #% Slower Stamina Drain
    AttributeDesc{ AttributeId::reanimate, P::Monster, R::Range, {} }.setGroup(AG::None),                                                   // Reanimate As: [Returned]
    AttributeDesc{ AttributeId::pierce, P::None, R::Range, { AF::ThrowableOrBow } }.setIdiom(IDI::Brawler).setGroup(AG::None),              // Piercing Attack
    AttributeDesc{ AttributeId::magicarrow, P::None, R::Range, { AF::Bow } }.setGroup(AG::None),                                            // Fires Magic Arrows
    AttributeDesc{ AttributeId::explosivearrow, P::None, R::Range, { AF::Bow } }.setGroup(AG::None),                                        // Fires Explosive Arrows or Bolts
    AttributeDesc{ AttributeId::dru, P::None, R::Range, { AF::Skills } }.setRange(1, 7).setClass(CH::Dru).setGroup(AG::AllClassSkills),     // +# to Druid Skill Levels
    AttributeDesc{ AttributeId::ass, P::None, R::Range, { AF::Skills } }.setRange(1, 7).setClass(CH::Ass).setGroup(AG::AllClassSkills),     // +# to Assassin Skill Levels
    AttributeDesc{ AttributeId::skill, P::Skill, R::Range, { AF::Skills } }.setRange(1, 7).setGroup(AG::None),                              // +# to [Skill] ([Class] only)
    AttributeDesc{ AttributeId::skilltab, P::SkillTab, R::Range, { AF::Skills } }.setRange(1, 7).setGroup(AG::None),                        // +# to [Class Skill Tab] Skills
    AttributeDesc{ AttributeId::aura, P::Skill, R::Range, { AF::Skills } }.setGroup(AG::None),                                              // Level # [Skill] Aura When Equipped
    AttributeDesc{ AttributeId::att_skill, P::Skill, R::Range, {} }.setIdiom(IDI::Brawler).setGroup(AG::OffensiveSkillProc),                // #% Chance to cast level # [Skill] on attack
    AttributeDesc{ AttributeId::hit_skill, P::Skill, R::Range, {} }.setIdiom(IDI::Brawler).setGroup(AG::OffensiveSkillProc),                // #% Chance to cast level # [Skill] on striking
    AttributeDesc{ AttributeId::gethit_skill, P::Skill, R::Range, {} }.setGroup(AG::DefensiveSkillProc),                                    // #% Chance to cast level # [Skill] when struck

    AttributeDesc{ AttributeId::sock, P::None, R::Range, { AF::Sockets } }.setRange(1, 6).setGroup(AG::None), // Socketed (#)

    AttributeDesc{ AttributeId::dmg_fire, P::None, R::DoubleParam, { AF::Damage } }.setRange(1, 512).setElement(E::Fire).setIdiom(IDI::Brawler).setGroup(AG::None),                            // Adds #-# Fire Damage
    AttributeDesc{ AttributeId::dmg_ltng, P::None, R::DoubleParam, { AF::Damage } }.setRange(1, 1024).setElement(E::Ltng).setIdiom(IDI::Brawler).setGroup(AG::None),                           // Adds #-# Lightning Damage
    AttributeDesc{ AttributeId::dmg_mag, P::None, R::DoubleParam, { AF::Damage } }.setRange(1, 256).setElement(E::Mag).setIdiom(IDI::Brawler).setGroup(AG::None),                              // Adds #-# Magic Damage
    AttributeDesc{ AttributeId::dmg_cold, P::PureValue, R::DoubleParam, { AF::Damage } }.setRange(1, 256).setElement(E::Cold).setIdiom(IDI::Brawler).setDefaultParam(100).setGroup(AG::None),  // Adds #-# Cold Damage
    AttributeDesc{ AttributeId::dmg_pois, P::PureValue, R::DoubleParam, { AF::Damage } }.setRange(1, 1024).setElement(E::Pois).setIdiom(IDI::Brawler).setDefaultParam(100).setGroup(AG::None), // Adds #-# Poison Damage Over # Seconds
    AttributeDesc{ AttributeId::dmg_norm, P::None, R::DoubleParam, { AF::Damage } }.setRange(1, 128).setElement(E::Phys).setIdiom(IDI::Brawler).setGroup(AG::None),                            // Adds #-# Damage

    AttributeDesc{ AttributeId::rep_dur, P::PureValue, R::None, { AF::Durability } }.setGroup(AG::None),                             // Repairs 1 durability in # seconds
    AttributeDesc{ AttributeId::rep_quant, P::PureValue, R::None, { AF::Throwable } }.setGroup(AG::None),                            // Replenishes quantity
    AttributeDesc{ AttributeId::stack, P::None, R::Range, { AF::Throwable } }.setGroup(AG::None),                                    // Increased Stack Size
    AttributeDesc{ AttributeId::pierce_fire, P::None, R::Range }.setElement(E::Fire).setRange(1, 255).setGroup(AG::ElePierce),       // -#% to Enemy Fire Resistance
    AttributeDesc{ AttributeId::pierce_ltng, P::None, R::Range }.setElement(E::Ltng).setRange(1, 255).setGroup(AG::ElePierce),       // -#% to Enemy Lightning Resistance
    AttributeDesc{ AttributeId::pierce_cold, P::None, R::Range }.setElement(E::Cold).setRange(1, 255).setGroup(AG::ElePierce),       // -#% to Enemy Cold Resistance
    AttributeDesc{ AttributeId::pierce_pois, P::None, R::Range }.setElement(E::Pois).setRange(1, 255).setGroup(AG::ElePierce),       // -#% to Enemy Poison Resistance
    AttributeDesc{ AttributeId::indestruct, P::None, R::Flag, { AF::Durability } }.setGroup(AG::None),                               // Indestructible
    AttributeDesc{ AttributeId::charged, P::Skill, R::DoubleParam, {} }.setGroup(AG::None),                                          // Level # [Skill] (#/# Charges)  par: [SKill], min:-ChargesCount, max: -Level
    AttributeDesc{ AttributeId::charged_neg, P::Skill, R::DoubleParam, {} }.setGroup(AG::None),                                      // Level # [Skill] (#/# Charges)  par: [SKill], min:-ChargesCount, max: -Level
    AttributeDesc{ AttributeId::charged_neg1, P::Skill, R::DoubleParam, {} }.setGroup(AG::None),                                     // Level # [Skill] (#/# Charges)  par: [SKill], min:-ChargesCount, max: -Level
    AttributeDesc{ AttributeId::charged_neg2, P::Skill, R::DoubleParam, {} }.setGroup(AG::None),                                     // Level # [Skill] (#/# Charges)  par: [SKill], min:-ChargesCount, max: -Level
    AttributeDesc{ AttributeId::extra_fire, P::None, R::Range }.setRange(-50, 461).setGroup(AG::EleExtra),                           // +#% to Fire Skill Damage
    AttributeDesc{ AttributeId::extra_ltng, P::None, R::Range }.setRange(-50, 461).setGroup(AG::EleExtra),                           // +#% to Lightning Skill Damage
    AttributeDesc{ AttributeId::extra_cold, P::None, R::Range }.setRange(-50, 461).setGroup(AG::EleExtra),                           // +#% to Cold Skill Damage
    AttributeDesc{ AttributeId::extra_pois, P::None, R::Range }.setRange(-50, 461).setGroup(AG::EleExtra),                           // +#% to Poison Skill Damage
    AttributeDesc{ AttributeId::dmg_elem, P::PureValue, R::DoubleParam, { AF::Damage } }.setIdiom(IDI::Brawler).setGroup(AG::None),  // Adds #-# Fire_perLightning_perCold Damage
    AttributeDesc{ AttributeId::dmg_elem_min, P::None, R::Range, { AF::Damage } }.setIdiom(IDI::Brawler).setGroup(AG::None),         // +# to Minimum Fire/Lightning/Cold Damage
    AttributeDesc{ AttributeId::dmg_elem_max, P::None, R::Range, { AF::Damage } }.setIdiom(IDI::Brawler).setGroup(AG::None),         // +# to Maximum Fire/Lightning/Cold Damage
    AttributeDesc{ AttributeId::all_stats, P::None, R::Range, { AF::Stats } }.setRange(-32, 95).setGroup(AG::None),                  // +# to all Attributes
    AttributeDesc{ AttributeId::addxp, P::None, R::Range, { AF::OP } }.setRange(-50, 461).setGroup(AG::None),                        // +#% to Experience Gained
    AttributeDesc{ AttributeId::heal_kill, P::None, R::Range }.setGroup(AG::ManaLifePerKill),                                        // +# Life after each Kill
    AttributeDesc{ AttributeId::cheap, P::None, R::Range }.setRange(1, 127).setGroup(AG::None),                                      // Reduces all Vendor Prices #%
    AttributeDesc{ AttributeId::rip, P::None, R::Flag }.setGroup(AG::None),                                                          // Slain Monsters Rest in Peace
    AttributeDesc{ AttributeId::att_mon_percent, P::PureValue, R::Range, { AF::Attack } }.setIdiom(IDI::Brawler).setGroup(AG::None), // #% to Attack Rating versus [Monster Type]
    AttributeDesc{ AttributeId::dmg_mon_percent, P::PureValue, R::Range, { AF::Damage } }.setIdiom(IDI::Brawler).setGroup(AG::None), // #% to Damage versus [Monster Type]
    AttributeDesc{ AttributeId::kill_skill, P::Skill, R::DoubleParam }.setGroup(AG::OffensiveSkillProc),                             // #% Chance to cast level # [Skill] when you Kill an Enemy
    AttributeDesc{ AttributeId::death_skill, P::Skill, R::DoubleParam }.setGroup(AG::DefensiveSkillProc),                            // #% Chance to cast level # [Skill] when you Die
    AttributeDesc{ AttributeId::levelup_skill, P::Skill, R::DoubleParam }.setGroup(AG::DefensiveSkillProc),                          // #% Chance to cast level # [Skill] when you Level_Up
    AttributeDesc{ AttributeId::skill_rand, P::PureValue, R::DoubleParam, { AF::Skills } }.setGroup(AG::None),                       // +# to [Skill] ([Class] only)
    AttributeDesc{ AttributeId::fade, P::None, R::Flag, { AF::Keep } }.setGroup(AG::None),                                           // Visuals Only
    AttributeDesc{ AttributeId::levelreq, P::None, R::Range }.setGroup(AG::None),                                                    // Required Level: #
    AttributeDesc{ AttributeId::ethereal, P::None, R::Flag, { AF::Durability } }.setGroup(AG::None),                                 // Ethereal
    AttributeDesc{ AttributeId::oskill, P::Skill, R::Range, { AF::Skills } }.setRange(1, 63).setGroup(AG::None),                     // +# to [Skill]   par: [SKill], min:MinLevel, max: MaxLevel
    AttributeDesc{ AttributeId::state, P::PureValue, R::Flag, { AF::Keep } }.setGroup(AG::None),                                     // Applies a State on the unit
    AttributeDesc{ AttributeId::randclassskill, P::None, R::Range, { AF::Skills, AF::ZeroIsValid } }.setGroup(AG::None),             // +# to [Class] Skill Levels

    /// perlevel
    AttributeDesc{ AttributeId::ac_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Defense } }.setRange(1, 63).setGroup(AG::None),                                            // +# Defense (Based on Character Level)
    AttributeDesc{ AttributeId::ac_percent_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Defense } }.setRange(1, 63).setGroup(AG::None),                                    // +#% Enhanced Defense (Based on Character Level)
    AttributeDesc{ AttributeId::hp_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Stats } }.setRange(1, 63).setGroup(AG::HpManaPerLvl),                                      // +# to Life (Based on Character Level)
    AttributeDesc{ AttributeId::mana_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Stats } }.setRange(1, 63).setGroup(AG::HpManaPerLvl),                                    // +# to Mana (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::None),                     // +# to Maximum Damage (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_percent_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::None),             // +#% Enhanced Maximum Damage (Based on Character Level)
    AttributeDesc{ AttributeId::str_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Stats } }.setRange(1, 63).setGroup(AG::StatPerLvl),                                       // +# to Strength (Based on Character Level)
    AttributeDesc{ AttributeId::dex_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Stats } }.setRange(1, 63).setGroup(AG::StatPerLvl),                                       // +# to Dexterity (Based on Character Level)
    AttributeDesc{ AttributeId::enr_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Stats } }.setRange(1, 63).setGroup(AG::StatPerLvl),                                       // +# to Energy (Based on Character Level)
    AttributeDesc{ AttributeId::vit_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Stats } }.setRange(1, 63).setGroup(AG::StatPerLvl),                                       // +# to Vitality (Based on Character Level)
    AttributeDesc{ AttributeId::att_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Attack } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::None),                     // +# to Attack Rating (Based on Character Level)
    AttributeDesc{ AttributeId::att_percent_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Attack } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::None),             // #% Bonus to Attack Rating (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_cold_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::EleDmgPerlvl),        // +# to Maximum Cold Damage (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_fire_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::EleDmgPerlvl),        // +# to Maximum Fire Damage (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_ltng_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::EleDmgPerlvl),        // +# to Maximum Lightning Damage (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_pois_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::EleDmgPerlvl),        // +# to Maximum Poison Damage (Based on Character Level)
    AttributeDesc{ AttributeId::res_cold_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Resistance } }.setRange(1, 63).setGroup(AG::ResPerLvl),                              // Cold Resist +#% (Based on Character Level)
    AttributeDesc{ AttributeId::res_fire_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Resistance } }.setRange(1, 63).setGroup(AG::ResPerLvl),                              // Fire Resist +#% (Based on Character Level)
    AttributeDesc{ AttributeId::res_ltng_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Resistance } }.setRange(1, 63).setGroup(AG::ResPerLvl),                              // Lightning Resist +#% (Based on Character Level)
    AttributeDesc{ AttributeId::res_pois_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Resistance } }.setRange(1, 63).setGroup(AG::ResPerLvl),                              // Poison Resist +#% (Based on Character Level)
    AttributeDesc{ AttributeId::abs_cold_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::DamageReduction } }.setRange(1, 63).setGroup(AG::AbsorbPerLvl),                      // Absorbs Cold Damage (Based on Character Level)
    AttributeDesc{ AttributeId::abs_fire_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::DamageReduction } }.setRange(1, 63).setGroup(AG::AbsorbPerLvl),                      // Absorbs Fire Damage (Based on Character Level)
    AttributeDesc{ AttributeId::abs_ltng_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::DamageReduction } }.setRange(1, 63).setGroup(AG::AbsorbPerLvl),                      // Absorbs Lightning Damage (Based on Character Level)
    AttributeDesc{ AttributeId::abs_pois_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::DamageReduction } }.setRange(1, 63).setGroup(AG::AbsorbPerLvl),                      // Absorbs Poison Damage (Based on Character Level)
    AttributeDesc{ AttributeId::thorns_perlvl, P::PureValue, R::None, { AF::PerLevel } }.setRange(1, 63).setGroup(AG::None),                                                     // Attacker Takes Damage of # (Based on Character Level)
    AttributeDesc{ AttributeId::gold_percent_perlvl, P::PureValue, R::None, { AF::PerLevel } }.setRange(1, 63).setGroup(AG::None),                                               // #% Extra Gold from Monsters (Based on Character Level)
    AttributeDesc{ AttributeId::mag_percent_perlvl, P::PureValue, R::None, { AF::PerLevel } }.setRange(1, 63).setGroup(AG::None),                                                // #% Better Chance of Getting Magic Items (Based on Character Level)
    AttributeDesc{ AttributeId::regen_stam_perlvl, P::PureValue, R::None, { AF::PerLevel } }.setRange(1, 63).setGroup(AG::None),                                                 // Heal Stamina Plus #% (Based on Character Level)
    AttributeDesc{ AttributeId::stam_perlvl, P::PureValue, R::None, { AF::PerLevel } }.setRange(1, 63).setGroup(AG::None),                                                       // +# Maximum Stamina (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_dem_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::DmgUndeadDemonPerLvl), // +#% Damage to Demons (Based on Character Level)
    AttributeDesc{ AttributeId::dmg_und_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::DmgUndeadDemonPerLvl), // +#% Damage to Undead (Based on Character Level)
    AttributeDesc{ AttributeId::att_dem_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Attack } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::AttUndeadDemonPerLvl), // +# to Attack Rating against Demons (Based on Character Level)
    AttributeDesc{ AttributeId::att_und_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Attack } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::AttUndeadDemonPerLvl), // +# to Attack Rating against Undead (Based on Character Level)
    AttributeDesc{ AttributeId::crush_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::OP } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::DeadlyOWCrushPerLvl),        // #% Chance of Crushing Blow (Based on Character Level)
    AttributeDesc{ AttributeId::wounds_perlvl, P::PureValue, R::None, { AF::PerLevel } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::DeadlyOWCrushPerLvl),               // #% Chance of Open Wounds (Based on Character Level)
    AttributeDesc{ AttributeId::kick_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::None),                    // +# Kick Damage (Based on Character Level)
    AttributeDesc{ AttributeId::deadly_perlvl, P::PureValue, R::None, { AF::PerLevel, AF::Damage } }.setIdiom(IDI::Brawler).setRange(1, 63).setGroup(AG::DeadlyOWCrushPerLvl),   // #% Deadly Strike (Based on Character Level)

    // PD2 maps
    AttributeDesc{ AttributeId::map_glob_monsterrarity, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_extra_fire, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_density, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_addxp, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_extra_cold, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_mag_gold_percent, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_extra_ltng, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_extra_pois, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_extra_mag, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_arealevel, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_att_pierce, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_att_cast_speed, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_hp_percent, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_ed_percent, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_splash, P::None, R::Flag, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_openwounds, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_regen, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_crush, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_phys_as_extra_ltng, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_phys_as_extra_cold, P::PureValue, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_phys_as_extra_fire, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_phys_as_extra_pois, P::PureValue, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_phys_as_extra_mag, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_doll, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_succ, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_vamp, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_cow, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_horde, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_ghost, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_souls, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_fetish, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_ac_percent, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_abs_fire_percent, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_abs_ltng_percent, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_abs_mag_percent, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_abs_cold_percent, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_red_dmg, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_velocity_percent, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_regen, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_lifesteal_hp_percent, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_balance1, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_balance1, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_lightradius, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_curseresist_hp_percent, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_res_all, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_ac_percent, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_block, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_add_mon_shriek, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_boss_dropcorruptedunique, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_boss_dropfacet, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_boss_droppuzzlebox, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_boss_dropskillers, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_boss_dropubermats, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_bossdropstreasure, P::None, R::Flag, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_dropbonus, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_dropcorrupted, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_dropethereal, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_dropsocketed, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_glob_extra_boss, P::None, R::Flag, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_cannotbefrozen, P::None, R::Flag, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_deadlystrike, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_droparmor, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_dropcharms, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_dropcrafting, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_dropjewels, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_mon_dropweapons, P::None, R::Range, { AF::PD2Map } },
    AttributeDesc{ AttributeId::map_play_speed_all, P::None, R::Range, { AF::PD2Map } },

    // PD2
    AttributeDesc{ AttributeId::inc_splash_radius, P::None, R::Range },
    AttributeDesc{ AttributeId::leapspeed, P::None, R::Range },
    AttributeDesc{ AttributeId::blood_warp_life_reduction, P::None, R::Range },
    AttributeDesc{ AttributeId::extra_skele_war, P::None, R::Range },
    AttributeDesc{ AttributeId::extra_skele_mage, P::None, R::Range },
    AttributeDesc{ AttributeId::heal_hit, P::None, R::Range }.setIdiom(IDI::Brawler),
    AttributeDesc{ AttributeId::rep_charge, P::PureValue, R::None },
    AttributeDesc{ AttributeId::grims_extra_skele_mage, P::None, R::Range },
    AttributeDesc{ AttributeId::inc_splash_radius2, P::None, R::Range },
    AttributeDesc{ AttributeId::ias_frw, P::None, R::Range },
    AttributeDesc{ AttributeId::pierce_phys, P::None, R::Range }.setIdiom(IDI::Brawler).setElement(E::Phys),
    AttributeDesc{ AttributeId::extra_spirits, P::None, R::Range },
    AttributeDesc{ AttributeId::joust_reduction, P::None, R::Range },
    AttributeDesc{ AttributeId::maxcurse, P::None, R::Range },
    AttributeDesc{ AttributeId::extra_revives, P::None, R::Range },
    AttributeDesc{ AttributeId::curse_res, P::None, R::Range },
    AttributeDesc{ AttributeId::randclassskill2, P::None, R::Range, { AF::Skills, AF::ZeroIsValid } },
    AttributeDesc{ AttributeId::randclassskill1, P::None, R::Range, { AF::Skills, AF::ZeroIsValid } },
    AttributeDesc{ AttributeId::gust_reduction, P::None, R::Range },
    AttributeDesc{ AttributeId::dclone_clout, P::None, R::Range },
    AttributeDesc{ AttributeId::maxlevel_clout, P::None, R::Range },
    AttributeDesc{ AttributeId::dev_clout, P::None, R::Range },
    AttributeDesc{ AttributeId::socketed_text, P::None, R::Range },
    AttributeDesc{ AttributeId::silence_fhr_ias, P::None, R::Range },
    AttributeDesc{ AttributeId::extra_hydra, P::None, R::Range },
    AttributeDesc{ AttributeId::extra_valk, P::None, R::Range },
    AttributeDesc{ AttributeId::extra_magi, P::None, R::Range },
    AttributeDesc{ AttributeId::plague_fcr_pierce, P::None, R::Range },
    AttributeDesc{ AttributeId::infinityspeed, P::None, R::Range },
    AttributeDesc{ AttributeId::extra_golem, P::None, R::Range },
    AttributeDesc{ AttributeId::mana_steal, P::None, R::Range },
    AttributeDesc{ AttributeId::block_skill, P::Skill, R::DoubleParam }.setGroup(AG::DefensiveSkillProc),
    AttributeDesc{ AttributeId::cast_skill, P::Skill, R::DoubleParam }.setGroup(AG::OffensiveSkillProc).setIdiom(IDI::Caster),
    AttributeDesc{ AttributeId::crit_strike_chance, P::None, R::Range }.setIdiom(IDI::Brawler),
    AttributeDesc{ AttributeId::crit_strike_multiplier, P::None, R::Range }.setIdiom(IDI::Brawler),
    AttributeDesc{ AttributeId::crush_efficiency, P::None, R::Range }.setIdiom(IDI::Brawler),
    AttributeDesc{ AttributeId::curse_effectiveness, P::None, R::Range }.setIdiom(IDI::Brawler),
    AttributeDesc{ AttributeId::deadly_strike_multiplier, P::None, R::Range },
    AttributeDesc{ AttributeId::deep_wounds, P::None, R::Range },
    AttributeDesc{ AttributeId::dmg_percent_pereth, P::None, R::Range },
    AttributeDesc{ AttributeId::eaglehorn_raven, P::None, R::Range },
    AttributeDesc{ AttributeId::equipped_skill, P::Skill, R::DoubleParam },
    AttributeDesc{ AttributeId::es_efficiency, P::None, R::Range },
    AttributeDesc{ AttributeId::es_efficiency_and_es_when_equip, P::None, R::Range },
    AttributeDesc{ AttributeId::extra_cold_arrows, P::None, R::Range },
    AttributeDesc{ AttributeId::extra_grizzly, P::None, R::Range },
    AttributeDesc{ AttributeId::extra_shadow, P::None, R::Range },
    AttributeDesc{ AttributeId::extra_skele_archer, P::None, R::Range },
    AttributeDesc{ AttributeId::extra_spiritwolf, P::None, R::Range },
    AttributeDesc{ AttributeId::joust_reduction_leorics, P::None, R::Range },
    AttributeDesc{ AttributeId::joust_reduction_zeraes, P::None, R::Range },
    AttributeDesc{ AttributeId::lifesteal_cap, P::None, R::Range },
    AttributeDesc{ AttributeId::max_deadly, P::None, R::Range }.setIdiom(IDI::Brawler),
    AttributeDesc{ AttributeId::mindmg_perenergy, P::None, R::Range },
    AttributeDesc{ AttributeId::no_wolves, P::None, R::Range },
    AttributeDesc{ AttributeId::sorc_skill_rand_ctc, P::None, R::Range },
    AttributeDesc{ AttributeId::splash, P::PureValue, R::DoubleParam, { AF::Keep } },
    AttributeDesc{ AttributeId::splash_percent_permissinghp_percent, P::None, R::Range },
    AttributeDesc{ AttributeId::str_and_vit, P::None, R::Range },

    // D2R:
    AttributeDesc{ AttributeId::pierce_immunity_cold, P::None, R::Range, { AF::Keep } }.setElement(E::Cold).setGroup(AG::Sunder),
    AttributeDesc{ AttributeId::pierce_immunity_fire, P::None, R::Range, { AF::Keep } }.setElement(E::Fire).setGroup(AG::Sunder),
    AttributeDesc{ AttributeId::pierce_immunity_light, P::None, R::Range, { AF::Keep } }.setElement(E::Ltng).setGroup(AG::Sunder),
    AttributeDesc{ AttributeId::pierce_immunity_poison, P::None, R::Range, { AF::Keep } }.setElement(E::Pois).setGroup(AG::Sunder),
    AttributeDesc{ AttributeId::pierce_immunity_damage, P::None, R::Range, { AF::Keep } }.setElement(E::Phys).setGroup(AG::Sunder),
    AttributeDesc{ AttributeId::pierce_immunity_magic, P::None, R::Range, { AF::Keep } }.setElement(E::Mag).setGroup(AG::Sunder),

    AttributeDesc{ AttributeId::charge_noconsume, P::None, R::Range, { AF::Keep } }.setGroup(AG::None),

    // D2R RotW:
    AttributeDesc{ AttributeId::pierce_mag, P::None, R::Range }.setRange(1, 255).setElement(E::Mag).setGroup(AG::ElePierce),            // -#% to Enemy Magic Resistance
    AttributeDesc{ AttributeId::extra_mag, P::None, R::Range }.setRange(-50, 461).setElement(E::Mag).setGroup(AG::EleExtra),            // +#% to Magic Skill Damage
    AttributeDesc{ AttributeId::war, P::None, R::Range, { AF::Skills } }.setRange(1, 7).setClass(CH::War).setGroup(AG::AllClassSkills), // +# to Warlock Skill Levels
    AttributeDesc{ AttributeId::skilltab_war, P::None, R::Range, { AF::Skills } }.setRange(1, 7).setClass(CH::War).setGroup(AG::None),  // +# to Warlock Skill random tab Levels
    AttributeDesc{ AttributeId::magdam_rand, P::None, R::Flag }.setGroup(AG::None),                                                     // Random magic damage
    AttributeDesc{ AttributeId::roguesbow_affix1, P::None, R::Flag, { AF::Keep } }.setClass(CH::Ama).setGroup(AG::None),                // +3 to Cold or Fire arrow
    AttributeDesc{ AttributeId::eternity_update, P::None, R::Flag, { AF::Keep } }.setGroup(AG::None),                                   //

};

const AttributeDescStr s_attrCodes[]{
    { AttributeId::ac, "ac", "ac" },
    { AttributeId::ac_miss, "ac-miss", "ac_miss" },
    { AttributeId::ac_hth, "ac-hth", "ac_hth" },
    { AttributeId::red_dmg, "red-dmg", "red_dmg" },
    { AttributeId::red_dmg_percent, "red-dmg%", "red_dmg_percent" },
    { AttributeId::ac_percent, "ac%", "ac_percent" },
    { AttributeId::red_mag, "red-mag", "red_mag" },
    { AttributeId::str, "str", "str" },
    { AttributeId::dex, "dex", "dex" },
    { AttributeId::vit, "vit", "vit" },
    { AttributeId::enr, "enr", "enr" },
    { AttributeId::mana, "mana", "mana" },
    { AttributeId::mana_percent, "mana%", "mana_percent" },
    { AttributeId::hp, "hp", "hp" },
    { AttributeId::hp_percent, "hp%", "hp_percent" },
    { AttributeId::att, "att", "att" },
    { AttributeId::block, "block", "block" },
    { AttributeId::cold_min, "cold-min", "cold_min" },
    { AttributeId::cold_max, "cold-max", "cold_max" },
    { AttributeId::cold_len, "cold-len", "cold_len" },
    { AttributeId::fire_min, "fire-min", "fire_min" },
    { AttributeId::fire_max, "fire-max", "fire_max" },
    { AttributeId::ltng_min, "ltng-min", "ltng_min" },
    { AttributeId::ltng_max, "ltng-max", "ltng_max" },
    { AttributeId::pois_min, "pois-min", "pois_min" },
    { AttributeId::pois_max, "pois-max", "pois_max" },
    { AttributeId::pois_len, "pois-len", "pois_len" },
    { AttributeId::dmg_min, "dmg-min", "dmg_min" },
    { AttributeId::dmg_max, "dmg-max", "dmg_max" },
    { AttributeId::dmg_percent, "dmg%", "dmg_percent" },

    { AttributeId::dmg_to_mana, "dmg-to-mana", "dmg_to_mana" },
    { AttributeId::res_fire, "res-fire", "res_fire" },
    { AttributeId::res_fire_max, "res-fire-max", "res_fire_max" },
    { AttributeId::res_ltng, "res-ltng", "res_ltng" },
    { AttributeId::res_ltng_max, "res-ltng-max", "res_ltng_max" },
    { AttributeId::res_cold, "res-cold", "res_cold" },
    { AttributeId::res_cold_max, "res-cold-max", "res_cold_max" },
    { AttributeId::res_mag, "res-mag", "res_mag" },
    { AttributeId::res_mag_max, "res-mag-max", "res_mag_max" },
    { AttributeId::res_pois, "res-pois", "res_pois" },
    { AttributeId::res_pois_max, "res-pois-max", "res_pois_max" },
    { AttributeId::res_all, "res-all", "res_all" },
    { AttributeId::res_all_max, "res-all-max", "res_all_max" },
    { AttributeId::abs_fire_percent, "abs-fire%", "abs_fire_percent" },
    { AttributeId::abs_fire, "abs-fire", "abs_fire" },
    { AttributeId::abs_ltng_percent, "abs-ltng%", "abs_ltng_percent" },
    { AttributeId::abs_ltng, "abs-ltng", "abs_ltng" },
    { AttributeId::abs_mag_percent, "abs-mag%", "abs_mag_percent" },
    { AttributeId::abs_mag, "abs-mag", "abs_mag" },
    { AttributeId::abs_cold_percent, "abs-cold%", "abs_cold_percent" },
    { AttributeId::abs_cold, "abs-cold", "abs_cold" },
    { AttributeId::dur, "dur", "dur" },
    { AttributeId::dur_percent, "dur%", "dur_percent" },
    { AttributeId::regen, "regen", "regen" },
    { AttributeId::thorns, "thorns", "thorns" },
    { AttributeId::swing3, "swing3", "swing3" },
    { AttributeId::gold_percent, "gold%", "gold_percent" },
    { AttributeId::mag_percent, "mag%", "mag_percent" },
    { AttributeId::knock, "knock", "knock" },
    { AttributeId::regen_stam, "regen-stam", "regen_stam" },
    { AttributeId::regen_mana, "regen-mana", "regen_mana" },
    { AttributeId::stam, "stam", "stam" },
    { AttributeId::manasteal, "manasteal", "manasteal" },
    { AttributeId::lifesteal, "lifesteal", "lifesteal" },

    { AttributeId::ama, "ama", "ama" },
    { AttributeId::pal, "pal", "pal" },
    { AttributeId::nec, "nec", "nec" },
    { AttributeId::sor, "sor", "sor" },
    { AttributeId::bar, "bar", "bar" },
    { AttributeId::light, "light", "light" },
    { AttributeId::ease, "ease", "ease" },
    { AttributeId::move3, "move3", "move3" },
    { AttributeId::balance3, "balance3", "balance3" },
    { AttributeId::block3, "block3", "block3" },
    { AttributeId::cast3, "cast3", "cast3" },
    { AttributeId::res_pois_len, "res-pois-len", "res_pois_len" },
    { AttributeId::dmg, "dmg", "dmg" },
    { AttributeId::howl, "howl", "howl" },
    { AttributeId::stupidity, "stupidity", "stupidity" },
    { AttributeId::ignore_ac, "ignore-ac", "ignore_ac" },
    { AttributeId::reduce_ac, "reduce-ac", "reduce_ac" },
    { AttributeId::noheal, "noheal", "noheal" },
    { AttributeId::half_freeze, "half-freeze", "half_freeze" },
    { AttributeId::att_percent, "att%", "att_percent" },
    { AttributeId::dmg_ac, "dmg-ac", "dmg_ac" },
    { AttributeId::dmg_demon, "dmg-demon", "dmg_demon" },
    { AttributeId::dmg_undead, "dmg-undead", "dmg_undead" },
    { AttributeId::att_demon, "att-demon", "att_demon" },
    { AttributeId::att_undead, "att-undead", "att_undead" },
    { AttributeId::fireskill, "fireskill", "fireskill" },
    { AttributeId::poisskill, "poisskill", "poisskill" },
    { AttributeId::coldskill, "coldskill", "coldskill" },
    { AttributeId::magskill, "magskill", "magskill" },
    { AttributeId::ltngskill, "ltngskill", "ltngskill" },
    { AttributeId::allskills, "allskills", "allskills" },
    { AttributeId::light_thorns, "light-thorns", "light_thorns" },
    { AttributeId::freeze, "freeze", "freeze" },
    { AttributeId::openwounds, "openwounds", "openwounds" },

    { AttributeId::crush, "crush", "crush" },
    { AttributeId::kick, "kick", "kick" },
    { AttributeId::mana_kill, "mana-kill", "mana_kill" },
    { AttributeId::demon_heal, "demon-heal", "demon_heal" },
    { AttributeId::bloody, "bloody", "bloody" },
    { AttributeId::deadly, "deadly", "deadly" },
    { AttributeId::slow, "slow", "slow" },
    { AttributeId::nofreeze, "nofreeze", "nofreeze" },
    { AttributeId::stamdrain, "stamdrain", "stamdrain" },
    { AttributeId::reanimate, "reanimate", "reanimate" },
    { AttributeId::pierce, "pierce", "pierce" },
    { AttributeId::magicarrow, "magicarrow", "magicarrow" },
    { AttributeId::explosivearrow, "explosivearrow", "explosivearrow" },
    { AttributeId::dru, "dru", "dru" },
    { AttributeId::ass, "ass", "ass" },
    { AttributeId::skill, "skill", "skill" },
    { AttributeId::skilltab, "skilltab", "skilltab" },
    { AttributeId::aura, "aura", "aura" },
    { AttributeId::att_skill, "att-skill", "att_skill" },
    { AttributeId::hit_skill, "hit-skill", "hit_skill" },
    { AttributeId::gethit_skill, "gethit-skill", "gethit_skill" },
    { AttributeId::sock, "sock", "sock" },
    { AttributeId::dmg_fire, "dmg-fire", "dmg_fire" },
    { AttributeId::dmg_ltng, "dmg-ltng", "dmg_ltng" },
    { AttributeId::dmg_mag, "dmg-mag", "dmg_mag" },
    { AttributeId::dmg_cold, "dmg-cold", "dmg_cold" },
    { AttributeId::dmg_pois, "dmg-pois", "dmg_pois" },
    { AttributeId::dmg_norm, "dmg-norm", "dmg_norm" },
    { AttributeId::rep_dur, "rep-dur", "rep_dur" },
    { AttributeId::rep_quant, "rep-quant", "rep_quant" },
    { AttributeId::stack, "stack", "stack" },
    { AttributeId::pierce_fire, "pierce-fire", "pierce_fire" },
    { AttributeId::pierce_ltng, "pierce-ltng", "pierce_ltng" },
    { AttributeId::pierce_cold, "pierce-cold", "pierce_cold" },
    { AttributeId::pierce_pois, "pierce-pois", "pierce_pois" },
    { AttributeId::indestruct, "indestruct", "indestruct" },
    { AttributeId::charged, "charged", "charged" },
    { AttributeId::charged_neg, "charged", "charged_neg" },
    { AttributeId::charged_neg1, "charged", "charged_neg1" },
    { AttributeId::charged_neg2, "charged", "charged_neg2" },

    { AttributeId::extra_fire, "extra-fire", "extra_fire" },
    { AttributeId::extra_ltng, "extra-ltng", "extra_ltng" },
    { AttributeId::extra_cold, "extra-cold", "extra_cold" },
    { AttributeId::extra_pois, "extra-pois", "extra_pois" },
    { AttributeId::dmg_elem, "dmg-elem", "dmg_elem" },
    { AttributeId::dmg_elem_min, "dmg-elem-min", "dmg_elem_min" },
    { AttributeId::dmg_elem_max, "dmg-elem-max", "dmg_elem_max" },
    { AttributeId::all_stats, "all-stats", "all_stats" },
    { AttributeId::addxp, "addxp", "addxp" },
    { AttributeId::heal_kill, "heal-kill", "heal_kill" },
    { AttributeId::cheap, "cheap", "cheap" },
    { AttributeId::rip, "rip", "rip" },
    { AttributeId::att_mon_percent, "att-mon%", "att_mon_percent" },
    { AttributeId::dmg_mon_percent, "dmg-mon%", "dmg_mon_percent" },
    { AttributeId::kill_skill, "kill-skill", "kill_skill" },
    { AttributeId::death_skill, "death-skill", "death_skill" },
    { AttributeId::levelup_skill, "levelup-skill", "levelup_skill" },
    { AttributeId::skill_rand, "skill-rand", "skill_rand" },
    { AttributeId::fade, "fade", "fade" },
    { AttributeId::levelreq, "levelreq", "levelreq" },
    { AttributeId::ethereal, "ethereal", "ethereal" },
    { AttributeId::oskill, "oskill", "oskill" },
    { AttributeId::state, "state", "state" },
    { AttributeId::randclassskill, "randclassskill", "randclassskill" },

    /// perlevel
    { AttributeId::ac_perlvl, "ac/lvl", "ac_perlvl" },
    { AttributeId::ac_percent_perlvl, "ac%/lvl", "ac_percent_perlvl" },
    { AttributeId::hp_perlvl, "hp/lvl", "hp_perlvl" },
    { AttributeId::mana_perlvl, "mana/lvl", "mana_perlvl" },
    { AttributeId::dmg_perlvl, "dmg/lvl", "dmg_perlvl" },
    { AttributeId::dmg_percent_perlvl, "dmg%/lvl", "dmg_percent_perlvl" },
    { AttributeId::str_perlvl, "str/lvl", "str_perlvl" },
    { AttributeId::dex_perlvl, "dex/lvl", "dex_perlvl" },
    { AttributeId::enr_perlvl, "enr/lvl", "enr_perlvl" },
    { AttributeId::vit_perlvl, "vit/lvl", "vit_perlvl" },
    { AttributeId::att_perlvl, "att/lvl", "att_perlvl" },
    { AttributeId::att_percent_perlvl, "att%/lvl", "att_percent_perlvl" },
    { AttributeId::dmg_cold_perlvl, "dmg-cold/lvl", "dmg_cold_perlvl" },
    { AttributeId::dmg_fire_perlvl, "dmg-fire/lvl", "dmg_fire_perlvl" },
    { AttributeId::dmg_ltng_perlvl, "dmg-ltng/lvl", "dmg_ltng_perlvl" },
    { AttributeId::dmg_pois_perlvl, "dmg-pois/lvl", "dmg_pois_perlvl" },
    { AttributeId::res_cold_perlvl, "res-cold/lvl", "res_cold_perlvl" },
    { AttributeId::res_fire_perlvl, "res-fire/lvl", "res_fire_perlvl" },
    { AttributeId::res_ltng_perlvl, "res-ltng/lvl", "res_ltng_perlvl" },
    { AttributeId::res_pois_perlvl, "res-pois/lvl", "res_pois_perlvl" },
    { AttributeId::abs_cold_perlvl, "abs-cold/lvl", "abs_cold_perlvl" },
    { AttributeId::abs_fire_perlvl, "abs-fire/lvl", "abs_fire_perlvl" },
    { AttributeId::abs_ltng_perlvl, "abs-ltng/lvl", "abs_ltng_perlvl" },
    { AttributeId::abs_pois_perlvl, "abs-pois/lvl", "abs_pois_perlvl" },
    { AttributeId::thorns_perlvl, "thorns/lvl", "thorns_perlvl" },
    { AttributeId::gold_percent_perlvl, "gold%/lvl", "gold_percent_perlvl" },
    { AttributeId::mag_percent_perlvl, "mag%/lvl", "mag_percent_perlvl" },
    { AttributeId::regen_stam_perlvl, "regen-stam/lvl", "regen_stam_perlvl" },
    { AttributeId::stam_perlvl, "stam/lvl", "stam_perlvl" },
    { AttributeId::dmg_dem_perlvl, "dmg-dem/lvl", "dmg_dem_perlvl" },
    { AttributeId::dmg_und_perlvl, "dmg-und/lvl", "dmg_und_perlvl" },
    { AttributeId::att_dem_perlvl, "att-dem/lvl", "att_dem_perlvl" },
    { AttributeId::att_und_perlvl, "att-und/lvl", "att_und_perlvl" },
    { AttributeId::crush_perlvl, "crush/lvl", "crush_perlvl" },
    { AttributeId::wounds_perlvl, "wounds/lvl", "wounds_perlvl" },
    { AttributeId::kick_perlvl, "kick/lvl", "kick_perlvl" },
    { AttributeId::deadly_perlvl, "deadly/lvl", "deadly_perlvl" },

    /// PD2 Maps
    { AttributeId::map_glob_monsterrarity, "map-glob-monsterrarity", "map_glob_monsterrarity" },
    { AttributeId::map_mon_extra_fire, "map-mon-extra-fire", "map_mon_extra_fire" },
    { AttributeId::map_glob_density, "map-glob-density", "map_glob_density" },
    { AttributeId::map_play_addxp, "map-play-addxp", "map_play_addxp" },
    { AttributeId::map_mon_extra_cold, "map-mon-extra-cold", "map_mon_extra_cold" },
    { AttributeId::map_play_mag_gold_percent, "map-play-mag-gold%", "map_play_mag_gold_percent" },
    { AttributeId::map_mon_extra_ltng, "map-mon-extra-ltng", "map_mon_extra_ltng" },
    { AttributeId::map_mon_extra_pois, "map-mon-extra-pois", "map_mon_extra_pois" },
    { AttributeId::map_mon_extra_mag, "map-mon-extra-mag", "map_mon_extra_mag" },
    { AttributeId::map_glob_arealevel, "map-glob-arealevel", "map_glob_arealevel" },
    { AttributeId::map_mon_att_pierce, "map-mon-att-pierce", "map_mon_att_pierce" },
    { AttributeId::map_mon_att_cast_speed, "map-mon-att-cast-speed", "map_mon_att_cast_speed" },
    { AttributeId::map_mon_hp_percent, "map-mon-hp%", "map_mon_hp_percent" },
    { AttributeId::map_mon_ed_percent, "map-mon-ed%", "map_mon_ed_percent" },
    { AttributeId::map_mon_splash, "map-mon-splash", "map_mon_splash" },
    { AttributeId::map_mon_openwounds, "map-mon-openwounds", "map_mon_openwounds" },
    { AttributeId::map_play_regen, "map-play-regen", "map_play_regen" },
    { AttributeId::map_mon_crush, "map-mon-crush", "map_mon_crush" },
    { AttributeId::map_mon_phys_as_extra_ltng, "map-mon-phys-as-extra-ltng", "map_mon_phys_as_extra_ltng" },
    { AttributeId::map_mon_phys_as_extra_cold, "map-mon-phys-as-extra-cold", "map_mon_phys_as_extra_cold" },
    { AttributeId::map_mon_phys_as_extra_fire, "map-mon-phys-as-extra-fire", "map_mon_phys_as_extra_fire" },
    { AttributeId::map_mon_phys_as_extra_pois, "map-mon-phys-as-extra-pois", "map_mon_phys_as_extra_pois" },
    { AttributeId::map_mon_phys_as_extra_mag, "map-mon-phys-as-extra-mag", "map_mon_phys_as_extra_mag" },
    { AttributeId::map_glob_add_mon_doll, "map-glob-add-mon-doll", "map_glob_add_mon_doll" },
    { AttributeId::map_glob_add_mon_succ, "map-glob-add-mon-succ", "map_glob_add_mon_succ" },
    { AttributeId::map_glob_add_mon_vamp, "map-glob-add-mon-vamp", "map_glob_add_mon_vamp" },
    { AttributeId::map_glob_add_mon_cow, "map-glob-add-mon-cow", "map_glob_add_mon_cow" },
    { AttributeId::map_glob_add_mon_horde, "map-glob-add-mon-horde", "map_glob_add_mon_horde" },
    { AttributeId::map_glob_add_mon_ghost, "map-glob-add-mon-ghost", "map_glob_add_mon_ghost" },
    { AttributeId::map_glob_add_mon_souls, "map-glob-add-mon-souls", "map_glob_add_mon_souls" },
    { AttributeId::map_glob_add_mon_fetish, "map-glob-add-mon-fetish", "map_glob_add_mon_fetish" },
    { AttributeId::map_mon_ac_percent, "map-mon-ac%", "map_mon_ac_percent" },
    { AttributeId::map_mon_abs_fire_percent, "map-mon-abs-fire%", "map_mon_abs_fire_percent" },
    { AttributeId::map_mon_abs_ltng_percent, "map-mon-abs-ltng%", "map_mon_abs_ltng_percent" },
    { AttributeId::map_mon_abs_mag_percent, "map-mon-abs-mag%", "map_mon_abs_mag_percent" },
    { AttributeId::map_mon_abs_cold_percent, "map-mon-abs-cold%", "map_mon_abs_cold_percent" },
    { AttributeId::map_mon_red_dmg, "map-mon-red-dmg", "map_mon_red_dmg" },
    { AttributeId::map_mon_velocity_percent, "map-mon-velocity%", "map_mon_velocity_percent" },
    { AttributeId::map_mon_regen, "map-mon-regen", "map_mon_regen" },
    { AttributeId::map_mon_lifesteal_hp_percent, "map-mon-lifesteal-hp%", "map_mon_lifesteal_hp_percent" },
    { AttributeId::map_mon_balance1, "map-mon-balance1", "map_mon_balance1" },
    { AttributeId::map_play_balance1, "map-play-balance1", "map_play_balance1" },
    { AttributeId::map_play_lightradius, "map-play-lightradius", "map_play_lightradius" },
    { AttributeId::map_mon_curseresist_hp_percent, "map-mon-curseresist-hp%", "map_mon_curseresist_hp_percent" },
    { AttributeId::map_play_res_all, "map-play-res-all", "map_play_res_all" },
    { AttributeId::map_play_ac_percent, "map-play-ac%", "map_play_ac_percent" },
    { AttributeId::map_play_block, "map-play-block", "map_play_block" },
    { AttributeId::map_glob_add_mon_shriek, "map-glob-add-mon-shriek", "map_glob_add_mon_shriek" },
    { AttributeId::map_glob_boss_dropcorruptedunique, "map-glob-boss-dropcorruptedunique", "map_glob_boss_dropcorruptedunique" },
    { AttributeId::map_glob_boss_dropfacet, "map-glob-boss-dropfacet", "map_glob_boss_dropfacet" },
    { AttributeId::map_glob_boss_droppuzzlebox, "map-glob-boss-droppuzzlebox", "map_glob_boss_droppuzzlebox" },
    { AttributeId::map_glob_boss_dropskillers, "map-glob-boss-dropskillers", "map_glob_boss_dropskillers" },
    { AttributeId::map_glob_boss_dropubermats, "map-glob-boss-dropubermats", "map_glob_boss_dropubermats" },
    { AttributeId::map_glob_bossdropstreasure, "map-glob-bossdropstreasure", "map_glob_bossdropstreasure" },
    { AttributeId::map_glob_dropbonus, "map-glob-dropbonus", "map_glob_dropbonus" },
    { AttributeId::map_glob_dropcorrupted, "map-glob-dropcorrupted", "map_glob_dropcorrupted" },
    { AttributeId::map_glob_dropethereal, "map-glob-dropethereal", "map_glob_dropethereal" },
    { AttributeId::map_glob_dropsocketed, "map-glob-dropsocketed", "map_glob_dropsocketed" },
    { AttributeId::map_glob_extra_boss, "map-glob-extra-boss", "map_glob_extra_boss" },
    { AttributeId::map_mon_cannotbefrozen, "map-mon-cannotbefrozen", "map_mon_cannotbefrozen" },
    { AttributeId::map_mon_deadlystrike, "map-mon-deadlystrike", "map_mon_deadlystrike" },
    { AttributeId::map_mon_droparmor, "map-mon-droparmor", "map_mon_droparmor" },
    { AttributeId::map_mon_dropcharms, "map-mon-dropcharms", "map_mon_dropcharms" },
    { AttributeId::map_mon_dropcrafting, "map-mon-dropcrafting", "map_mon_dropcrafting" },
    { AttributeId::map_mon_dropjewels, "map-mon-dropjewels", "map_mon_dropjewels" },
    { AttributeId::map_mon_dropweapons, "map-mon-dropweapons", "map_mon_dropweapons" },
    { AttributeId::map_play_speed_all, "map-play-speed-all", "map_play_speed_all" },

    /// PD2
    { AttributeId::inc_splash_radius, "inc-splash-radius", "inc_splash_radius" },
    { AttributeId::leapspeed, "leapspeed", "leapspeed" },
    { AttributeId::blood_warp_life_reduction, "blood-warp-life-reduction", "blood_warp_life_reduction" },
    { AttributeId::extra_skele_war, "extra-skele-war", "extra_skele_war" },
    { AttributeId::extra_skele_mage, "extra-skele-mage", "extra_skele_mage" },
    { AttributeId::heal_hit, "heal-hit", "heal_hit" },
    { AttributeId::rep_charge, "rep-charge", "rep_charge" },
    { AttributeId::grims_extra_skele_mage, "grims-extra-skele-mage", "grims_extra_skele_mage" },
    { AttributeId::inc_splash_radius2, "inc_splash_radius", "inc_splash_radius2" },
    { AttributeId::ias_frw, "ias-frw", "ias_frw" },
    { AttributeId::pierce_phys, "pierce-phys", "pierce_phys" },
    { AttributeId::extra_spirits, "extra-spirits", "extra_spirits" },
    { AttributeId::joust_reduction, "joust-reduction", "joust_reduction" },
    { AttributeId::maxcurse, "maxcurse", "maxcurse" },
    { AttributeId::extra_revives, "extra-revives", "extra_revives" },
    { AttributeId::curse_res, "curse-res", "curse_res" },
    { AttributeId::randclassskill2, "randclassskill2", "randclassskill2" },
    { AttributeId::randclassskill1, "randclassskill1", "randclassskill1" },
    { AttributeId::gust_reduction, "gust-reduction", "gust_reduction" },
    { AttributeId::dclone_clout, "dclone-clout", "dclone_clout" },
    { AttributeId::maxlevel_clout, "maxlevel-clout", "maxlevel_clout" },
    { AttributeId::dev_clout, "dev-clout", "dev_clout" },
    { AttributeId::socketed_text, "socketed-text", "socketed_text" },
    { AttributeId::silence_fhr_ias, "silence-fhr-ias", "silence_fhr_ias" },
    { AttributeId::extra_hydra, "extra-hydra", "extra_hydra" },
    { AttributeId::extra_valk, "extra-valk", "extra_valk" },
    { AttributeId::extra_magi, "extra-magi", "extra_magi" },
    { AttributeId::plague_fcr_pierce, "plague-fcr-pierce", "plague_fcr_pierce" },
    { AttributeId::infinityspeed, "infinityspeed", "infinityspeed" },
    { AttributeId::extra_golem, "extra-golem", "extra_golem" },
    { AttributeId::mana_steal, "mana-steal", "mana_steal" },
    { AttributeId::block_skill, "block-skill", "block_skill" },
    { AttributeId::cast_skill, "cast-skill", "cast_skill" },
    { AttributeId::crit_strike_chance, "crit-strike-chance", "crit_strike_chance" },
    { AttributeId::crit_strike_multiplier, "crit-strike-multiplier", "crit_strike_multiplier" },
    { AttributeId::crush_efficiency, "crush-efficiency", "crush_efficiency" },
    { AttributeId::curse_effectiveness, "curse-effectiveness", "curse_effectiveness" },
    { AttributeId::deadly_strike_multiplier, "deadly-strike-multiplier", "deadly_strike_multiplier" },

    { AttributeId::deep_wounds, "deep-wounds", "deep_wounds" },
    { AttributeId::dmg_percent_pereth, "dmg%/eth", "dmg_percent_pereth" },
    { AttributeId::eaglehorn_raven, "eaglehorn-raven", "eaglehorn_raven" },
    { AttributeId::equipped_skill, "equipped-skill", "equipped_skill" },
    { AttributeId::es_efficiency, "es-efficiency", "es_efficiency" },
    { AttributeId::es_efficiency_and_es_when_equip, "es-efficiency-and-es-when-equip", "es_efficiency_and_es_when_equip" },
    { AttributeId::extra_cold_arrows, "extra-cold-arrows", "extra_cold_arrows" },
    { AttributeId::extra_grizzly, "extra-grizzly", "extra_grizzly" },
    { AttributeId::extra_shadow, "extra-shadow", "extra_shadow" },
    { AttributeId::extra_skele_archer, "extra-skele-archer", "extra_skele_archer" },
    { AttributeId::extra_spiritwolf, "extra-spiritwolf", "extra_spiritwolf" },
    { AttributeId::joust_reduction_leorics, "joust-reduction-leorics", "joust_reduction_leorics" },
    { AttributeId::joust_reduction_zeraes, "joust-reduction-zeraes", "joust_reduction_zeraes" },
    { AttributeId::lifesteal_cap, "lifesteal-cap", "lifesteal_cap" },
    { AttributeId::max_deadly, "max-deadly", "max_deadly" },
    { AttributeId::mindmg_perenergy, "mindmg/energy", "mindmg_perenergy" },
    { AttributeId::no_wolves, "no-wolves", "no_wolves" },
    { AttributeId::sorc_skill_rand_ctc, "sorc-skill-rand-ctc", "sorc_skill_rand_ctc" },
    { AttributeId::splash, "splash", "splash" },
    { AttributeId::splash_percent_permissinghp_percent, "splash%/missinghp%", "splash_percent_permissinghp_percent" },
    { AttributeId::str_and_vit, "str-and-vit", "str_and_vit" },

    /// D2R
    { AttributeId::pierce_immunity_cold, "pierce-immunity-cold", "pierce_immunity_cold" },
    { AttributeId::pierce_immunity_fire, "pierce-immunity-fire", "pierce_immunity_fire" },
    { AttributeId::pierce_immunity_light, "pierce-immunity-light", "pierce_immunity_light" },
    { AttributeId::pierce_immunity_poison, "pierce-immunity-poison", "pierce_immunity_poison" },
    { AttributeId::pierce_immunity_damage, "pierce-immunity-damage", "pierce_immunity_damage" },
    { AttributeId::pierce_immunity_magic, "pierce-immunity-magic", "pierce_immunity_magic" },
    { AttributeId::charge_noconsume, "charge-noconsume", "charge_noconsume" },

    /// D2R - ROTW
    { AttributeId::pierce_mag, "pierce-mag", "pierce_mag" },
    { AttributeId::extra_mag, "extra-mag", "extra_mag" },
    { AttributeId::war, "war", "war" },
    { AttributeId::skilltab_war, "skilltab-war", "skilltab_war" },
    { AttributeId::magdam_rand, "magdam-rand", "magdam_rand" },
    { AttributeId::roguesbow_affix1, "roguesbow-affix1", "roguesbow_affix1" },
    { AttributeId::eternity_update, "eternity-update", "eternity_update" },
};

const std::unordered_map<std::string_view, AttributeId> s_attributesIndex = [] {
    std::unordered_map<std::string_view, AttributeId> result;
    for (int i = 0, cnt = int(AttributeId::count__); i < cnt; ++i) {
        AttributeId id = static_cast<AttributeId>(i);
        if (id >= AttributeId::charged_neg && id <= AttributeId::charged_neg2)
            continue;
        auto& attrCode = s_attrCodes[i];
        auto& attrDesc = s_attributes[i];
        assert(attrDesc.code == id);
        assert(attrCode.id == id);
        result[attrCode.code] = id;
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

}

const AttributeDesc& getAttributeDesc(AttributeId code)
{
    if (code == AttributeId::INVALID)
        return s_stub_invalid;
    if (code == AttributeId::SKIP)
        return s_stub_skip;
    return s_attributes[static_cast<int>(code)];
}

int powerMultiply(AttributeId code, int value, Percent powerMult)
{
    const AttributeDesc& desc     = getAttributeDesc(code);
    const bool           hasRange = desc.hasLimits();
    if (!hasRange)
        return value;

    Percent powerPercent = 100_pct;
    if (desc.flags.contains(AF::Skills)
        || desc.flags.contains(AF::DamageReduction)
        || desc.flags.contains(AF::Speed))
        powerPercent = 50_pct;

    if (powerMult > 100_pct) {
        const Percent diff = (powerMult - 100_pct) * powerPercent;
        value              = value * (100_pct + diff);
    } else if (powerMult < 100_pct) {
        value = value * powerMult;
    }
    value = std::clamp(value, desc.minValue.value(), desc.maxValue.value());
    return value;
}

bool isMinMaxRange(AttributeId code)
{
    return getAttributeDesc(code).rangeMode == RangeMode::Range;
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

Character getClassId(const std::string& code)
{
    for (auto& desc : s_classes)
        if (desc.code == code)
            return desc.clas;

    return Character::None;
}
Element getElementId(const std::string& code)
{
    for (auto& desc : s_elements)
        if (desc.code == code)
            return desc.elem;

    return Element::None;
}

std::string enumToString(Character type, bool asEnum)
{
    const std::string suffix = asEnum ? "Character::" : "";
    if (type == Character::None)
        return suffix + "None";
    const auto& desc = s_classes[static_cast<int>(type)];
    return suffix + std::string(asEnum ? desc.enumStr : desc.code);
}
std::string enumToString(AttributeIdiom type, bool asEnum)
{
    const std::string suffix = asEnum ? "AttributeIdiom::" : "";
    return suffix + std::string(s_idioms[static_cast<int>(type)].code);
}
std::string enumToString(Element type, bool asEnum)
{
    const std::string suffix = asEnum ? "Element::" : "";
    return suffix + std::string(s_elements[static_cast<int>(type)].code);
}
std::string enumToString(AttributeId id, bool asEnum)
{
    const std::string suffix = asEnum ? "AttributeId::" : "";
    if (id == AttributeId::SKIP)
        return suffix + "SKIP";
    if (id <= AttributeId::INVALID)
        return suffix + "INVALID";
    const auto& desc = s_attrCodes[static_cast<int>(id)];
    return suffix + std::string(asEnum ? desc.enumStr : desc.code);
}
std::string enumToString(SlotType slotType, bool asEnum)
{
    const std::string suffix = asEnum ? "SlotType::" : "";
    return suffix + std::string(s_slotTypes[static_cast<int>(slotType)].code);
}

std::string enumToString(RangeMode mode, bool asEnum)
{
    const std::string suffix = asEnum ? "RangeMode::" : "";
    return suffix + std::string(s_rangeTypes[static_cast<int>(mode)].code);
}
std::string enumToString(ParMode mode, bool asEnum)
{
    const std::string suffix = asEnum ? "ParMode::" : "";
    return suffix + std::string(s_parTypes[static_cast<int>(mode)].code);
}
std::string enumToString(AttributeGroup group, bool asEnum)
{
    const std::string suffix = asEnum ? "AttributeGroup::" : "";
    return suffix + std::string(s_groupTypes[static_cast<int>(group)].code);
}

void AttributeDesc::clampLimits(std::optional<int>& value, bool checkMax) const
{
    if (!value.has_value())
        return;
    if (checkMax && maxValue.has_value())
        value = std::min(value.value(), maxValue.value());

    if (minValue.has_value())
        value = std::max(value.value(), minValue.value());
    else if (value.value() >= 0) {
        const int absMinLimit = flags.contains(AttributeFlag::ZeroIsValid) ? 0 : 1;

        value = std::max(value.value(), absMinLimit);
    }
}

}
