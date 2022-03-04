/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "AttributeHelper.hpp"

#include <unordered_map>
#include <vector>

#include <cassert>

namespace D2ModGen {

namespace {

const std::vector<AttributeDesc> s_attributes{
    { "ac", { AttributeFlag::Defense } },                                  // +# Defense
    { "ac-miss", { AttributeFlag::Defense } },                             // +# Defense vs. Missile
    { "ac-hth", { AttributeFlag::Defense } },                              // +# Defense vs. Melee
    { "red-dmg", { AttributeFlag::DamageReduction } },                     // Damage Reduced by #
    { "red-dmg%", { AttributeFlag::Resistance } },                         // Damage Reduced by #%
    { "ac%", { AttributeFlag::Defense } },                                 // +#% Enhanced Defense
    { "red-mag", { AttributeFlag::DamageReduction } },                     // Magic Damage Reduced by #
    { "str", { AttributeFlag::Stats } },                                   // +# to Strength
    { "dex", { AttributeFlag::Stats } },                                   // +# to Dexterity
    { "vit", { AttributeFlag::Stats } },                                   // +# to Vitality
    { "enr", { AttributeFlag::Stats } },                                   // +# to Energy
    { "mana", { AttributeFlag::Stats } },                                  // +# to Mana
    { "mana%", { AttributeFlag::Stats } },                                 // Increase Maximum Mana #%
    { "hp", { AttributeFlag::Stats } },                                    // +# to Life
    { "hp%", { AttributeFlag::Stats } },                                   // Increase Maximum Life #%
    { "att", { AttributeFlag::Attack } },                                  // +# to Attack Rating
    { "block", {}, { AttributeItemReq::Shield } },                         // #% Increased Chance of Blocking
    { "cold-min", { AttributeFlag::Damage } },                             // +# to Minimum Cold Damage
    { "cold-max", { AttributeFlag::Damage } },                             // +# to Maximum Cold Damage
    { "cold-len", { AttributeFlag::Damage } },                             // # of Frames (25 Frames = 1 Second)
    { "fire-min", { AttributeFlag::Damage } },                             // +# to Minimum Fire Damage
    { "fire-max", { AttributeFlag::Damage } },                             // +# to Maximum Fire Damage
    { "ltng-min", { AttributeFlag::Damage } },                             // +# to Minimum Lightning Damage
    { "ltng-max", { AttributeFlag::Damage } },                             // +# to Maximum Lightning Damage
    { "pois-min", { AttributeFlag::Damage } },                             // +# to Minimum Poison Damage
    { "pois-max", { AttributeFlag::Damage } },                             // +# to Maximum Poison Damage
    { "pois-len", { AttributeFlag::Damage } },                             // # of Frames (25 Frames = 1 Second)
    { "dmg-min", { AttributeFlag::Damage } },                              // +# to Minimum Damage
    { "dmg-max", { AttributeFlag::Damage } },                              // +# to Maximum Damage
    { "dmg%", { AttributeFlag::Damage } },                                 // +#% Enhanced Damage
    { "dmg-to-mana", {} },                                                 // #% Damage Taken Goes To Mana
    { "res-fire", { AttributeFlag::Resistance } },                         // Fire Resist +#%
    { "res-fire-max", { AttributeFlag::Resistance } },                     // +#% to Maximum Fire Resist
    { "res-ltng", { AttributeFlag::Resistance } },                         // Lightning Resist +#%
    { "res-ltng-max", { AttributeFlag::Resistance } },                     // +#% to Maximum Lightning Resist
    { "res-cold", { AttributeFlag::Resistance } },                         // Cold Resist +#%
    { "res-cold-max", { AttributeFlag::Resistance } },                     // +#% to Maximum Cold Resist
    { "res-mag", { AttributeFlag::Resistance } },                          // Magic Resist +#%
    { "res-mag-max", { AttributeFlag::Resistance } },                      // +#% to Maximum Magic Resist
    { "res-pois", { AttributeFlag::Resistance } },                         // Poison Resist +#%
    { "res-pois-max", { AttributeFlag::Resistance } },                     // +#% to Maximum Poison Resist
    { "res-all", { AttributeFlag::Resistance } },                          // All Resistances +#
    { "res-all-max", { AttributeFlag::Resistance } },                      // Currently Not Used
    { "abs-fire%", { AttributeFlag::Resistance } },                        // +# Fire Absorb
    { "abs-fire", { AttributeFlag::DamageReduction } },                    // Fire Absorb #%
    { "abs-ltng%", { AttributeFlag::Resistance } },                        // +# Lightning Absorb
    { "abs-ltng", { AttributeFlag::DamageReduction } },                    // Lightning Absorb #%
    { "abs-mag%", { AttributeFlag::Resistance } },                         // +# Magic Absorb
    { "abs-mag", { AttributeFlag::DamageReduction } },                     // Magic Absorb #%
    { "abs-cold%", { AttributeFlag::Resistance } },                        // +# Cold Absorb
    { "abs-cold", { AttributeFlag::DamageReduction } },                    // Cold Absorb #%
    { "dur", { AttributeFlag::Durability } },                              // Durability: # of #
    { "dur%", { AttributeFlag::Durability } },                             // Increase Maximum Durability #%
    { "regen", {} },                                                       // Replenish Life +#
    { "thorns", {} },                                                      // Attacker Takes Damage of #
    { "swing1", { AttributeFlag::Speed } },                                // +#% Increased Attack Speed
    { "swing2", { AttributeFlag::Speed } },                                // +#% Increased Attack Speed
    { "swing3", { AttributeFlag::Speed } },                                // +#% Increased Attack Speed
    { "gold%", {} },                                                       // #% Extra Gold from Monsters
    { "mag%", {} },                                                        // #% Better Chance of Getting Magic Items
    { "knock", {}, { AttributeItemReq::Weapon } },                         // Knockback
    { "regen-stam", {} },                                                  // Heal Stamina Plus #%
    { "regen-mana", {} },                                                  // Regenerate Mana #%
    { "stam", {} },                                                        // +# Maximum Stamina
    { "manasteal", { AttributeFlag::Leech } },                             // #% Mana stolen per hit
    { "lifesteal", { AttributeFlag::Leech } },                             // #% Life stolen per hit
    { "ama", { AttributeFlag::Skills } },                                  // +# to Amazon Skill Levels
    { "pal", { AttributeFlag::Skills } },                                  // +# to Paladin Skill Levels
    { "nec", { AttributeFlag::Skills } },                                  // +# to Necromancer Skill Levels
    { "sor", { AttributeFlag::Skills } },                                  // +# to Sorceress Skill Levels
    { "bar", { AttributeFlag::Skills } },                                  // +#  to Barbarian Skill Levels
    { "light", {} },                                                       // +# to Light Radius
    { "ease", {}, { AttributeItemReq::Weapon, AttributeItemReq::Armor } }, // Requirements -#%
    { "move1", { AttributeFlag::Speed } },                                 // +#% Faster Run/Walk
    { "move2", { AttributeFlag::Speed } },                                 // +#% Faster Run/Walk
    { "move3", { AttributeFlag::Speed } },                                 // +#% Faster Run/Walk
    { "balance1", { AttributeFlag::Speed } },                              // +#% Faster Hit Recovery
    { "balance2", { AttributeFlag::Speed } },                              // +#% Faster Hit Recovery
    { "balance3", { AttributeFlag::Speed } },                              // +#% Faster Hit Recovery
    { "block1", { AttributeFlag::Speed } },                                // +#% Faster Block Rate
    { "block2", { AttributeFlag::Speed } },                                // +#% Faster Block Rate
    { "block3", { AttributeFlag::Speed } },                                // +#% Faster Block Rate
    { "cast1", { AttributeFlag::Speed } },                                 // +#% Faster Cast Rate
    { "cast2", { AttributeFlag::Speed } },                                 // +#% Faster Cast Rate
    { "cast3", { AttributeFlag::Speed } },                                 // +#% Faster Cast Rate
    { "res-pois-len", { AttributeFlag::Resistance } },                     // Poison Length Reduced by #%
    { "dmg", { AttributeFlag::Damage } },                                  // Damage +#
    { "howl", {} },                                                        // Hit Causes Monster to Flee #%
    { "stupidity", {} },                                                   // Hit Blinds Target +#
    { "ignore-ac", { AttributeFlag::Attack } },                            // Ignore Target's Defense
    { "reduce-ac", { AttributeFlag::Attack } },                            // -#% Target Defense
    { "noheal", {} },                                                      // Prevent Monster Heal
    { "half-freeze", {} },                                                 // Half Freeze Duration
    { "att%", { AttributeFlag::Attack } },                                 // #% Bonus to Attack Rating
    { "dmg-ac", { AttributeFlag::Attack } },                               // -# to Monster Defense Per Hit
    { "dmg-demon", { AttributeFlag::Damage } },                            // +#% Damage to Demons
    { "dmg-undead", { AttributeFlag::Damage } },                           // +#% Damage to Undead
    { "att-demon", { AttributeFlag::Attack } },                            // +# to Attack Rating against Demons
    { "att-undead", { AttributeFlag::Attack } },                           // +# to Attack Rating against Undead
    { "fireskill", { AttributeFlag::Skills } },                            // +# to Fire Skills
    { "poisskill", { AttributeFlag::Skills } },
    { "coldskill", { AttributeFlag::Skills } },
    { "magskill", { AttributeFlag::Skills } },
    { "ltngskill", { AttributeFlag::Skills } },
    { "allskills", { AttributeFlag::Skills } },                            // +# to All Skills
    { "light-thorns", {} },                                                // Attacker Takes Lightning Damage of #
    { "freeze", {} },                                                      // Freezes Target +#
    { "openwounds", {} },                                                  // #% Chance of Open Wounds
    { "crush", { AttributeFlag::OP } },                                    // #% Chance of Crushing Blow
    { "kick", { AttributeFlag::Damage } },                                 // +# Kick Damage
    { "mana-kill", {} },                                                   // +# to Mana after each Kill
    { "demon-heal", {} },                                                  // +# Life after each Demon Kill
    { "bloody", {} },                                                      // Visuals Only
    { "deadly", { AttributeFlag::Damage } },                               // #% Deadly Strike
    { "slow", {} },                                                        // Slows Target by #%
    { "nofreeze", { AttributeFlag::OP } },                                 // Cannot Be Frozen
    { "stamdrain", {} },                                                   // #% Slower Stamina Drain
    { "reanimate", {} },                                                   // Reanimate As: [Returned]
    { "pierce", { AttributeFlag::Missile, AttributeFlag::Quantity }, {} }, // Piercing Attack
    { "magicarrow", { AttributeFlag::Missile }, {} },                      // Fires Magic Arrows
    { "explosivearrow", { AttributeFlag::Missile }, {} },                  // Fires Explosive Arrows or Bolts
    { "dru", { AttributeFlag::Skills } },                                  // +# to Druid Skill Levels
    { "ass", { AttributeFlag::Skills } },                                  // +# to Assassin Skill Levels
    { "skill", { AttributeFlag::Skills } },                                // +# to [Skill] ([Class] only)
    { "skilltab", { AttributeFlag::Skills } },                             // +# to [Class Skill Tab] Skills
    { "aura", {} },                                                        // Level # [Skill] Aura When Equipped
    { "att-skill", { AttributeFlag::NoMinMax } },                          // #% Chance to cast level # [Skill] on attack
    { "hit-skill", { AttributeFlag::NoMinMax } },                          // #% Chance to cast level # [Skill] on striking
    { "gethit-skill", { AttributeFlag::NoMinMax } },                       // #% Chance to cast level # [Skill] when struck

    { "sock", { AttributeFlag::Sockets } }, // Socketed (#)

    { "dmg-fire", { AttributeFlag::Damage } }, // Adds #-# Fire Damage
    { "dmg-ltng", { AttributeFlag::Damage } }, // Adds #-# Lightning Damage
    { "dmg-mag", { AttributeFlag::Damage } },  // Adds #-# Magic Damage
    { "dmg-cold", { AttributeFlag::Damage } }, // Adds #-# Cold Damage
    { "dmg-pois", { AttributeFlag::Damage } }, // Adds #-# Poison Damage Over # Seconds
    { "dmg-norm", { AttributeFlag::Damage } }, // Adds #-# Damage

    { "ac/lvl", { AttributeFlag::PerLevel, AttributeFlag::Defense } },               // +# Defense (Based on Character Level)
    { "ac%/lvl", { AttributeFlag::PerLevel, AttributeFlag::Defense } },              // +#% Enhanced Defense (Based on Character Level)
    { "hp/lvl", { AttributeFlag::PerLevel, AttributeFlag::Stats } },                 // +# to Life (Based on Character Level)
    { "mana/lvl", { AttributeFlag::PerLevel, AttributeFlag::Stats } },               // +# to Mana (Based on Character Level)
    { "dmg/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },               // +# to Maximum Damage (Based on Character Level)
    { "dmg%/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },              // +#% Enhanced Maximum Damage (Based on Character Level)
    { "str/lvl", { AttributeFlag::PerLevel, AttributeFlag::Stats } },                // +# to Strength (Based on Character Level)
    { "dex/lvl", { AttributeFlag::PerLevel, AttributeFlag::Stats } },                // +# to Dexterity (Based on Character Level)
    { "enr/lvl", { AttributeFlag::PerLevel, AttributeFlag::Stats } },                // +# to Energy (Based on Character Level)
    { "vit/lvl", { AttributeFlag::PerLevel, AttributeFlag::Stats } },                // +# to Vitality (Based on Character Level)
    { "att/lvl", { AttributeFlag::PerLevel, AttributeFlag::Attack } },               // +# to Attack Rating (Based on Character Level)
    { "att%/lvl", { AttributeFlag::PerLevel, AttributeFlag::Attack } },              // #% Bonus to Attack Rating (Based on Character Level)
    { "dmg-cold/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },          // +# to Maximum Cold Damage (Based on Character Level)
    { "dmg-fire/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },          // +# to Maximum Fire Damage (Based on Character Level)
    { "dmg-ltng/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },          // +# to Maximum Lightning Damage (Based on Character Level)
    { "dmg-pois/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },          // +# to Maximum Poison Damage (Based on Character Level)
    { "res-cold/lvl", { AttributeFlag::PerLevel, AttributeFlag::Resistance } },      // Cold Resist +#% (Based on Character Level)
    { "res-fire/lvl", { AttributeFlag::PerLevel, AttributeFlag::Resistance } },      // Fire Resist +#% (Based on Character Level)
    { "res-ltng/lvl", { AttributeFlag::PerLevel, AttributeFlag::Resistance } },      // Lightning Resist +#% (Based on Character Level)
    { "res-pois/lvl", { AttributeFlag::PerLevel, AttributeFlag::Resistance } },      // Poison Resist +#% (Based on Character Level)
    { "abs-cold/lvl", { AttributeFlag::PerLevel, AttributeFlag::DamageReduction } }, // Absorbs Cold Damage (Based on Character Level)
    { "abs-fire/lvl", { AttributeFlag::PerLevel, AttributeFlag::DamageReduction } }, // Absorbs Fire Damage (Based on Character Level)
    { "abs-ltng/lvl", { AttributeFlag::PerLevel, AttributeFlag::DamageReduction } }, // Absorbs Lightning Damage (Based on Character Level)
    { "abs-pois/lvl", { AttributeFlag::PerLevel, AttributeFlag::DamageReduction } }, // Absorbs Poison Damage (Based on Character Level)
    { "thorns/lvl", { AttributeFlag::PerLevel } },                                   // Attacker Takes Damage of # (Based on Character Level)
    { "gold%/lvl", { AttributeFlag::PerLevel } },                                    // #% Extra Gold from Monsters (Based on Character Level)
    { "mag%/lvl", { AttributeFlag::PerLevel } },                                     // #% Better Chance of Getting Magic Items (Based on Character Level)
    { "regen-stam/lvl", { AttributeFlag::PerLevel } },                               // Heal Stamina Plus #% (Based on Character Level)
    { "stam/lvl", { AttributeFlag::PerLevel } },                                     // +# Maximum Stamina (Based on Character Level)
    { "dmg-dem/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },           // +#% Damage to Demons (Based on Character Level)
    { "dmg-und/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },           // +#% Damage to Undead (Based on Character Level)
    { "att-dem/lvl", { AttributeFlag::PerLevel, AttributeFlag::Attack } },           // +# to Attack Rating against Demons (Based on Character Level)
    { "att-und/lvl", { AttributeFlag::PerLevel, AttributeFlag::Attack } },           // +# to Attack Rating against Undead (Based on Character Level)
    { "crush/lvl", { AttributeFlag::PerLevel, AttributeFlag::OP } },                 // #% Chance of Crushing Blow (Based on Character Level)
    { "wounds/lvl", { AttributeFlag::PerLevel } },                                   // #% Chance of Open Wounds (Based on Character Level)
    { "kick/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },              // +# Kick Damage (Based on Character Level)
    { "deadly/lvl", { AttributeFlag::PerLevel, AttributeFlag::Damage } },            // #% Deadly Strike (Based on Character Level)

    { "rep-dur", { AttributeFlag::Durability, AttributeFlag::NoMinMax } },    // Repairs 1 durability in # seconds
    { "rep-quant", { AttributeFlag::Quantity, AttributeFlag::NoMinMax } },    // Replenishes quantity
    { "stack", { AttributeFlag::Quantity } },                                 // Increased Stack Size
    { "pierce-fire", {} },                                                    // -#% to Enemy Fire Resistance
    { "pierce-ltng", {} },                                                    // -#% to Enemy Lightning Resistance
    { "pierce-cold", {} },                                                    // -#% to Enemy Cold Resistance
    { "pierce-pois", {} },                                                    // -#% to Enemy Poison Resistance
    { "indestruct", { AttributeFlag::Durability } },                          // Indestructible
    { "charged", { AttributeFlag::NoMinMax } },                               // Level # [Skill] (#/# Charges)
    { "extra-fire", {} },                                                     // +#% to Fire Skill Damage
    { "extra-ltng", {} },                                                     // +#% to Lightning Skill Damage
    { "extra-cold", {} },                                                     // +#% to Cold Skill Damage
    { "extra-pois", {} },                                                     // +#% to Poison Skill Damage
    { "dmg-elem", { AttributeFlag::Damage } },                                // Adds #-# Fire/Lightning/Cold Damage
    { "dmg-elem-min", { AttributeFlag::Damage } },                            // +# to Minimum Fire/Lightning/Cold Damage
    { "dmg-elem-max", { AttributeFlag::Damage } },                            // +# to Maximum Fire/Lightning/Cold Damage
    { "all-stats", { AttributeFlag::Stats } },                                // +# to all Attributes
    { "addxp", { AttributeFlag::OP } },                                       // +#% to Experience Gained
    { "heal-kill", {} },                                                      // +# Life after each Kill
    { "cheap", {} },                                                          // Reduces all Vendor Prices #%
    { "rip", {} },                                                            // Slain Monsters Rest in Peace
    { "att-mon%", { AttributeFlag::Attack } },                                // #% to Attack Rating versus [Monster Type]
    { "dmg-mon%", { AttributeFlag::Damage } },                                // #% to Damage versus [Monster Type]
    { "kill-skill", { AttributeFlag::NoMinMax } },                            // #% Chance to cast level # [Skill] when you Kill an Enemy
    { "death-skill", { AttributeFlag::NoMinMax } },                           // #% Chance to cast level # [Skill] when you Die
    { "levelup-skill", { AttributeFlag::NoMinMax } },                         // #% Chance to cast level # [Skill] when you Level-Up
    { "skill-rand", { AttributeFlag::Skills, AttributeFlag::NoMinMax } },     // +# to [Skill] ([Class] only)
    { "fade", {} },                                                           // Visuals Only
    { "levelreq", {} },                                                       // Required Level: #
    { "ethereal", { AttributeFlag::Durability } },                            // Ethereal
    { "oskill", { AttributeFlag::Skills } },                                  // +# to [Skill]
    { "state", {} },                                                          // Applies a State on the unit
    { "randclassskill", { AttributeFlag::Skills, AttributeFlag::NoMinMax } }, // +# to [Class] Skill Levels

    { "map-glob-monsterrarity", { AttributeFlag::PD2Map } },
    { "map-mon-extra-fire", { AttributeFlag::PD2Map } },
    { "map-glob-density", { AttributeFlag::PD2Map } },
    { "map-play-addxp", { AttributeFlag::PD2Map } },
    { "map-mon-extra-cold", { AttributeFlag::PD2Map } },
    { "map-play-mag-gold%", { AttributeFlag::PD2Map } },
    { "map-mon-extra-ltng", { AttributeFlag::PD2Map } },
    { "map-mon-extra-pois", { AttributeFlag::PD2Map } },
    { "map-mon-extra-mag", { AttributeFlag::PD2Map } },
    { "map-glob-arealevel", { AttributeFlag::PD2Map } },
    { "map-mon-att-pierce", { AttributeFlag::PD2Map } },
    { "map-mon-att-cast-speed", { AttributeFlag::PD2Map } },
    { "map-mon-hp%", { AttributeFlag::PD2Map } },
    { "map-mon-ed%", { AttributeFlag::PD2Map } },
    { "map-mon-splash", { AttributeFlag::PD2Map } },
    { "map-mon-openwounds", { AttributeFlag::PD2Map } },
    { "map-play-regen", { AttributeFlag::PD2Map } },
    { "map-mon-crush", { AttributeFlag::PD2Map } },
    { "map-mon-phys-as-extra-ltng", { AttributeFlag::PD2Map } },
    { "map-mon-phys-as-extra-cold", { AttributeFlag::PD2Map } },
    { "map-mon-phys-as-extra-fire", { AttributeFlag::PD2Map } },
    { "map-mon-phys-as-extra-pois", { AttributeFlag::PD2Map } },
    { "map-mon-phys-as-extra-mag", { AttributeFlag::PD2Map } },
    { "map-glob-add-mon-doll", { AttributeFlag::PD2Map } },
    { "map-glob-add-mon-succ", { AttributeFlag::PD2Map } },
    { "map-glob-add-mon-vamp", { AttributeFlag::PD2Map } },
    { "map-glob-add-mon-cow", { AttributeFlag::PD2Map } },
    { "map-glob-add-mon-horde", { AttributeFlag::PD2Map } },
    { "map-glob-add-mon-ghost", { AttributeFlag::PD2Map } },
    { "map-glob-add-mon-souls", { AttributeFlag::PD2Map } },
    { "map-glob-add-mon-fetish", { AttributeFlag::PD2Map } },
    { "map-mon-ac%", { AttributeFlag::PD2Map } },
    { "map-mon-abs-fire%", { AttributeFlag::PD2Map } },
    { "map-mon-abs-ltng%", { AttributeFlag::PD2Map } },
    { "map-mon-abs-mag%", { AttributeFlag::PD2Map } },
    { "map-mon-abs-cold%", { AttributeFlag::PD2Map } },
    { "map-mon-red-dmg", { AttributeFlag::PD2Map } },
    { "map-mon-velocity%", { AttributeFlag::PD2Map } },
    { "map-mon-regen", { AttributeFlag::PD2Map } },
    { "map-mon-lifesteal-hp%", { AttributeFlag::PD2Map } },
    { "map-mon-balance1", { AttributeFlag::PD2Map } },
    { "map-play-balance1", { AttributeFlag::PD2Map } },
    { "map-play-lightradius", { AttributeFlag::PD2Map } },
    { "map-mon-curseresist-hp%", { AttributeFlag::PD2Map } },
    { "map-play-res-all", { AttributeFlag::PD2Map } },
    { "map-play-ac%", { AttributeFlag::PD2Map } },
    { "map-play-block", { AttributeFlag::PD2Map } },

    { "inc-splash-radius", {} },
    { "leapspeed", {} },
    { "blood-warp-life-reduction", {} },
    { "extra-skele-war", {} },
    { "extra-skele-mage", {} },
    { "heal-hit", {} },
    { "rep-charge", {} },
    { "grims-extra-skele-mage", {} },
    { "cast", { AttributeFlag::Speed } },
    { "inc_splash_radius", {} },
    { "ias-frw", {} },
    { "pierce-phys", {} },
    { "extra-spirits", {} },
    { "joust-reduction", {} },
    { "maxcurse", {} },
    { "extra-revives", {} },
    { "curse-res", {} },
    { "randclassskill2", {} },
    { "randclassskill1", {} },
    { "gust-reduction", {} },
    { "dclone-clout", {} },
    { "maxlevel-clout", {} },
    { "dev-clout", {} },
    { "socketed-text", {} },
    { "silence-fhr-ias", {} },
    { "extra-hydra", {} },
    { "extra-valk", {} },
    { "extra-magi", {} },
    { "plague-fcr-pierce", {} },
    { "infinityspeed", {} },
    { "extra-golem", {} },
    { "mana-steal", {} },

    { "Light", {} },
    { "Thorns", {} },
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
    const bool           noMinMax = desc.flags.contains(AttributeFlag::NoMinMax);
    const bool           byLevel  = desc.flags.contains(AttributeFlag::PerLevel);
    const bool           isMap    = desc.flags.contains(AttributeFlag::PD2Map);
    return !noMinMax && !byLevel && !isMap;
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

}
