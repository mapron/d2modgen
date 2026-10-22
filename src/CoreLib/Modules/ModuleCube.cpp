/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleCube.hpp"

namespace D2ModGen {

namespace {
const std::map<std::string, std::string> s_craftedTypeReplace{
    { "\"fhl,mag,upg\"", "\"helm,mag\"" },
    { "\"mbt,mag,upg\"", "\"boot,mag\"" },
    { "\"mgl,mag,upg\"", "\"glov,mag\"" },
    { "\"tbl,mag,upg\"", "\"belt,mag\"" },
    { "\"gts,mag,upg\"", "\"shld,mag\"" },
    { "\"fld,mag,upg\"", "\"tors,mag\"" },
    { "\"blun,mag\"", "\"weap,mag\"" },

    { "\"hlm,mag,upg\"", "\"helm,mag\"" },
    { "\"tbt,mag,upg\"", "\"boot,mag\"" },
    { "\"vgl,mag,upg\"", "\"glov,mag\"" },
    { "\"mbl,mag,upg\"", "\"belt,mag\"" },
    { "\"spk,mag,upg\"", "\"shld,mag\"" },
    { "\"plt,mag,upg\"", "\"tors,mag\"" },
    { "\"axe,mag\"", "\"weap,mag\"" },

    { "\"msk,mag,upg\"", "\"helm,mag\"" },
    { "\"lbt,mag,upg\"", "\"boot,mag\"" },
    { "\"lgl,mag,upg\"", "\"glov,mag\"" },
    { "\"vbl,mag,upg\"", "\"belt,mag\"" },
    { "\"sml,mag,upg\"", "\"shld,mag\"" },
    { "\"ltp,mag,upg\"", "\"tors,mag\"" },
    { "\"rod,mag\"", "\"weap,mag\"" },

    { "\"crn,mag,upg\"", "\"helm,mag\"" },
    { "\"hbt,mag,upg\"", "\"boot,mag\"" },
    { "\"hgl,mag,upg\"", "\"glov,mag\"" },
    { "\"lbl,mag,upg\"", "\"belt,mag\"" },
    { "\"kit,mag,upg\"", "\"shld,mag\"" },
    { "\"brs,mag,upg\"", "\"tors,mag\"" },
    { "\"spea,mag\"", "\"miss,mag\"" },
};

const std::map<std::string, std::string> s_craftedGemReplace{
    { "gpb", "gems" },
    { "gpr", "gemr" },
    { "gpv", "gema" },
    { "gpg", "geme" },
    { "gpy", "gemt" },
    { "skz", "gemz" },
};

}

ModuleCube::ModuleCube()
    : PluginModule(Mernel::PropertyTreeMap{
          { "cheatCharms", Mernel::PropertyTreeScalar{ false } },
          { "craftHighIlvl", Mernel::PropertyTreeScalar{ false } },
          { "craftNoRunes", Mernel::PropertyTreeScalar{ false } },
          { "craftNoStrict", Mernel::PropertyTreeScalar{ false } },
          { "noGemUpgrade", Mernel::PropertyTreeScalar{ false } },
          { "quickPortals", Mernel::PropertyTreeScalar{ true } },
          { "quickQuests", Mernel::PropertyTreeScalar{ true } },
          { "socketing", Mernel::PropertyTreeScalar{ false } },
          { "upgrading", Mernel::PropertyTreeScalar{ false } },
      })
{}

void ModuleCube::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    const bool noGemUpgrade  = input.getInt("noGemUpgrade");
    const bool quickPortals  = input.getInt("quickPortals");
    const bool quickQuests   = input.getInt("quickQuests");
    const bool newSocketing  = input.getInt("socketing");
    const bool upgrading     = input.getInt("upgrading");
    const bool craftNoStrict = input.getInt("craftNoStrict");
    const bool craftNoRunes  = input.getInt("craftNoRunes");
    const bool craftHighIlvl = input.getInt("craftHighIlvl");
    const bool cheatCharms   = input.getInt("cheatCharms");

    for (auto& row : output.cubemain.rows) {
        auto& description = row.description;

        if (noGemUpgrade && description.find("->") != std::string::npos && description.ends_with(" Rune") && row.inputs.size() >= 2) {
            row.numinputs = row.numinputs.value() - 1;
            row.inputs.pop_back();
        }
        if (craftHighIlvl && (row.isCrafted() || row.hasLvls())) {
            row.ilvl.reset();
            row.plvl.reset();
            row.lvl = 99;
        }
        if (row.isCrafted()) {
            if (craftNoStrict)
                row.inputs[0] = mapValue(s_craftedTypeReplace, row.inputs[0], row.inputs[0]);

            if (craftNoRunes && row.inputs.size() == 4) {
                row.inputs[1] = row.inputs[3];
                row.inputs.resize(2);
                row.numinputs = 2;
                row.inputs[1] = mapValue(s_craftedGemReplace, row.inputs[1], row.inputs[1]);
            }
        }
    }
    if (quickPortals) {
        const DataRow_cubemain base{
            .inputs      = std::vector<std::string>{ "tbk", "" },
            .description = "Quick Portal",
            .enabled     = true,
            .version     = 100,
            .op          = 28,

        };
        DataRow_cubemain cows = base;
        cows.numinputs        = 2;
        cows.inputs[1]        = "isc";
        cows.output           = "Cow Portal";

        DataRow_cubemain uber1 = base;
        uber1.numinputs        = 3;
        uber1.inputs[1]        = "\"isc,qty=2\"";
        uber1.output           = "Pandemonium Portal";

        DataRow_cubemain uber2 = base;
        uber2.numinputs        = 4;
        uber2.inputs[1]        = "\"isc,qty=3\"";
        uber2.output           = "Pandemonium Finale Portal";

        DataRow_cubemain uber3 = base;
        uber3.numinputs        = 5;
        uber3.inputs[1]        = "\"isc,qty=4\"";
        uber3.output           = "\"Red Portal,lvl=137,qty=1\"";

        output.cubemain.rows.push_back(cows);
        output.cubemain.rows.push_back(uber1);
        output.cubemain.rows.push_back(uber2);
        output.cubemain.rows.push_back(uber3);
    }
    if (quickQuests) {
        const DataRow_cubemain base{
            .inputs      = std::vector<std::string>{ "key", "" },
            .description = "Quick Quest",
            .enabled     = true,
            .version     = 0,
            .op          = 28,
            .numinputs   = 2,

        };

        DataRow_cubemain staff = base;
        staff.inputs[1]        = "isc";
        staff.output           = "hst";

        DataRow_cubemain khalim = base;
        khalim.inputs[1]        = "tsc";
        khalim.output           = "qf2";

        output.cubemain.rows.push_back(staff);
        output.cubemain.rows.push_back(khalim);
    }
    if (newSocketing) {
        const DataRow_cubemain base{
            .inputs      = std::vector<std::string>{ "\"any,nor,nos\"", "", "" },
            .description = "Quick Sockets",
            .enabled     = true,
            .version     = 100,

        };

        DataRow_cubemain socket3 = base;
        socket3.numinputs        = 3;
        socket3.inputs[1]        = "tsc";
        socket3.inputs[2]        = "isc";
        socket3.output           = "\"useitem,sock=3\"";

        DataRow_cubemain socket4 = base;
        socket4.numinputs        = 4;
        socket4.inputs[1]        = "tsc";
        socket4.inputs[2]        = "\"isc,qty=2\"";
        socket4.output           = "\"useitem,sock=4\"";

        DataRow_cubemain socket5 = base;
        socket5.numinputs        = 4;
        socket5.inputs[1]        = "\"tsc,qty=2\"";
        socket5.inputs[2]        = "isc";
        socket5.output           = "\"useitem,sock=5\"";

        DataRow_cubemain socket6 = base;
        socket6.numinputs        = 5;
        socket6.inputs[1]        = "\"tsc,qty=2\"";
        socket6.inputs[2]        = "\"isc,qty=2\"";
        socket6.output           = "\"useitem,sock=6\"";

        DataRow_cubemain socket1 = base;
        socket1.numinputs        = 3;
        socket1.inputs[0]        = "\"any,uni,nos\"";
        socket1.inputs[1]        = "tsc";
        socket1.inputs[2]        = "isc";
        socket1.output           = "\"useitem,sock=1\"";

        DataRow_cubemain socket1a = socket1;
        socket1a.inputs[0]        = "\"any,rar,nos\"";

        DataRow_cubemain socket1b = socket1;
        socket1b.inputs[0]        = "\"any,set,nos\"";

        DataRow_cubemain socketClear = base;
        socketClear.numinputs        = 2;
        socketClear.inputs.resize(2);
        socketClear.inputs[0] = "\"any,sock\"";
        socketClear.inputs[1] = "tsc";
        socketClear.output    = "\"useitem,uns\"";

        output.cubemain.rows.push_back(socket3);
        output.cubemain.rows.push_back(socket4);
        output.cubemain.rows.push_back(socket5);
        output.cubemain.rows.push_back(socket6);
        output.cubemain.rows.push_back(socket1);
        output.cubemain.rows.push_back(socket1a);
        output.cubemain.rows.push_back(socket1b);
        output.cubemain.rows.push_back(socketClear);
    }

    if (upgrading) {
        DataRow_cubemain base{
            .inputs      = std::vector<std::string>{ "", "yps" },
            .description = "Quick Upgrading",
            .enabled     = true,
            .version     = 100,
            .numinputs   = 2,
            .ilvl        = 100,

        };

        if (craftHighIlvl) {
            base.ilvl.reset();
            base.plvl.reset();
            base.lvl = 99;
        }
        DataRow_cubemain armo1 = base;
        armo1.inputs[0]        = "\"armo,bas\"";
        armo1.output           = "\"useitem,mod,exc\"";

        DataRow_cubemain weap1 = armo1;
        weap1.inputs[0]        = "\"weap,bas\"";

        DataRow_cubemain armo2 = base;
        armo2.inputs[0]        = "\"armo,exc\"";
        armo2.output           = "\"useitem,mod,eli\"";

        DataRow_cubemain weap2 = armo2;
        weap2.inputs[0]        = "\"weap,exc\"";

        DataRow_cubemain armoNor = base;
        armoNor.inputs[0]        = "armo";
        armoNor.inputs[1]        = "vps";
        armoNor.output           = "\"usetype,nor\"";

        DataRow_cubemain weapNor = armoNor;
        weapNor.inputs[0]        = "weap";

        output.cubemain.rows.push_back(armo1);
        output.cubemain.rows.push_back(weap1);
        output.cubemain.rows.push_back(armo2);
        output.cubemain.rows.push_back(weap2);
        output.cubemain.rows.push_back(armoNor);
        output.cubemain.rows.push_back(weapNor);
    }

    if (cheatCharms) {
        struct Affix {
            std::string m_name;
            int         m_value;
        };
        struct CraftResult {
            std::string        m_name;
            std::string        m_i1;
            std::string        m_i2;
            std::vector<Affix> m_affixes;
        };
        const std::vector<CraftResult> crs{
            //----------------
            CraftResult{
                .m_name    = "Cheat SC: key + stamina = all skills + all stats",
                .m_i1      = "key",
                .m_i2      = "vps",
                .m_affixes = { { "allskills", 7 }, { "all-stats", 20 } },
            },
            CraftResult{
                .m_name    = "Cheat SC: key + antidote = damage reduction",
                .m_i1      = "key",
                .m_i2      = "yps",
                .m_affixes = { { "red-dmg", 15 }, { "red-mag", 15 }, { "red-dmg%", 10 } },
            },
            CraftResult{
                .m_name    = "Cheat SC: key + thawing = absorb",
                .m_i1      = "key",
                .m_i2      = "wms",
                .m_affixes = { { "abs-fire%", 10 }, { "abs-ltng%", 10 }, { "abs-mag%", 10 }, { "abs-cold%", 10 } },
            },

            CraftResult{
                .m_name    = "Cheat SC: key + any hp = HP and vitality",
                .m_i1      = "key",
                .m_i2      = "hpot",
                .m_affixes = { { "hp", 300 }, { "hp%", 10 }, { "vit", 20 } },
            },
            CraftResult{
                .m_name    = "Cheat SC: key + any mana = Mana and energy",
                .m_i1      = "key",
                .m_i2      = "mpot",
                .m_affixes = { { "mana", 150 }, { "mana%", 20 }, { "enr", 20 } },
            },

            //----------------
            CraftResult{
                .m_name    = "Cheat SC: TP + stamina = faster frames",
                .m_i1      = "tsc",
                .m_i2      = "vps",
                .m_affixes = { { "move2", 30 }, { "cast2", 80 }, { "balance2", 80 }, { "block2", 50 } },
            },
            CraftResult{
                .m_name    = "Cheat SC: TP + antidote = max res + poison len",
                .m_i1      = "tsc",
                .m_i2      = "yps",
                .m_affixes = { { "res-fire-max", 8 }, { "res-ltng-max", 8 }, { "res-cold-max", 8 }, { "res-pois-max", 8 }, { "res-pois-len", 50 } },
            },
            CraftResult{
                .m_name    = "Cheat SC: TP + thawing = all res",
                .m_i1      = "tsc",
                .m_i2      = "wms",
                .m_affixes = { { "res-all", 50 } },
            },
            CraftResult{
                .m_name    = "Cheat SC: TP + any hp = hp regen",
                .m_i1      = "tsc",
                .m_i2      = "hpot",
                .m_affixes = { { "lifesteal", 10 }, { "regen", 30 }, { "heal-kill", 20 }, { "regen-stam", 50 } },
            },
            CraftResult{
                .m_name    = "Cheat SC: TP + any mana = mana regen",
                .m_i1      = "tsc",
                .m_i2      = "mpot",
                .m_affixes = { { "manasteal", 5 }, { "regen-mana", 50 }, { "mana-kill", 5 }, { "dmg-to-mana", 20 } },
            },

            //----------------
            CraftResult{
                .m_name    = "Cheat SC: ID + stamina = pierce res",
                .m_i1      = "isc",
                .m_i2      = "vps",
                .m_affixes = { { "pierce-pois", 20 }, { "pierce-fire", 20 }, { "pierce-ltng", 20 }, { "pierce-cold", 20 } },
            },
            CraftResult{
                .m_name    = "Cheat SC: ID + antidote = +elemental dmg",
                .m_i1      = "isc",
                .m_i2      = "yps",
                .m_affixes = { { "extra-ltng", 50 }, { "extra-cold", 50 }, { "extra-pois", 50 }, { "extra-fire", 50 } },
            },
            CraftResult{
                .m_name    = "Cheat SC: ID + thawing = OW/CB/DS",
                .m_i1      = "isc",
                .m_i2      = "wms",
                .m_affixes = { { "crush", 20 }, { "openwounds", 20 }, { "deadly", 20 } },
            },
            CraftResult{
                .m_name    = "Cheat SC: ID + any hp = IAS and Enh Dmg",
                .m_i1      = "isc",
                .m_i2      = "hpot",
                .m_affixes = { { "swing2", 30 }, { "dmg-max", 20 }, { "dmg%", 100 }, { "dmg-demon", 100 }, { "dmg-undead", 100 } },
            },
            CraftResult{
                .m_name    = "Cheat SC: ID + any mana = attack rating and defence",
                .m_i1      = "isc",
                .m_i2      = "mpot",
                .m_affixes = { { "att%", 20 }, { "att", 200 }, { "ac", 100 } },
            },
        };
        for (const CraftResult& c : crs) {
            DataRow_cubemain data{
                .inputs      = std::vector<std::string>{ "cm1", c.m_i1, c.m_i2 },
                .output      = "\"usetype,tmp\"",
                .description = c.m_name,
                .enabled     = true,
                .version     = 100,
                .numinputs   = 3,
                .lvl         = 1,
            };

            for (auto&& af : c.m_affixes) {
                data.affixes_a.push_back(AffixProp{ .code = af.m_name, .par = "", .min = af.m_value, .max = af.m_value, .chance = 100 });
            }
            output.cubemain.rows.push_back(data);
        }
    }
}

}
