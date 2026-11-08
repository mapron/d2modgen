/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleExpandedInventory.hpp"

#include "Logger.hpp"

namespace D2ModGen {

namespace {

struct BlobIDDesc {
    std::string name;
    BlobId      orig;
    BlobId      patched;
    bool        controller = false;
    bool        expansion  = false;
    bool        lowres     = false;
};
constexpr const bool fals = false;

std::vector<BlobIDDesc> s_spriteDescs{
    BlobIDDesc{ "MnkbClassicHighre", BlobId::OrignMnkbInventoryClassicSpriteHighre, BlobId::PatchMnkbInventoryClassicSpriteHighre, fals, fals, fals },
    BlobIDDesc{ "MnkbClassicLowres", BlobId::OrignMnkbInventoryClassicSpriteLowres, BlobId::PatchMnkbInventoryClassicSpriteLowres, fals, fals, true },
    BlobIDDesc{ "MnkbExpansnHighre", BlobId::OrignMnkbInventoryExpansnSpriteHighre, BlobId::PatchMnkbInventoryExpansnSpriteHighre, fals, true, fals },
    BlobIDDesc{ "MnkbExpansnLowend", BlobId::OrignMnkbInventoryExpansnSpriteLowend, BlobId::PatchMnkbInventoryExpansnSpriteLowend, fals, true, true },

    BlobIDDesc{ "CtrlClassicHighre", BlobId::OrignCtrlInventoryClassicSpriteHighre, BlobId::PatchCtrlInventoryClassicSpriteHighre, true, fals, fals },
    BlobIDDesc{ "CtrlClassicLowres", BlobId::OrignCtrlInventoryClassicSpriteLowres, BlobId::PatchCtrlInventoryClassicSpriteLowres, true, fals, true },
    BlobIDDesc{ "CtrlExpansnHighre", BlobId::OrignCtrlInventoryExpansnSpriteHighre, BlobId::PatchCtrlInventoryExpansnSpriteHighre, true, true, fals },
    BlobIDDesc{ "CtrlExpansnLowend", BlobId::OrignCtrlInventoryExpansnSpriteLowend, BlobId::PatchCtrlInventoryExpansnSpriteLowend, true, true, true },

};

struct JsonIDDesc {
    JsonId id;
    bool   controller;
    bool   expansion = false;
};

std::vector<JsonIDDesc> s_jsonDescs{
    JsonIDDesc{ JsonId::LayoutMnkbClassic, fals, fals },
    JsonIDDesc{ JsonId::LayoutMnkbExpansn, fals, true },
    JsonIDDesc{ JsonId::LayoutCtrlClassic, true, fals },
    JsonIDDesc{ JsonId::LayoutCtrlExpansn, true, true },

};

struct Point {
    int x{};
    int y{};

    void read(const Mernel::PropertyTree& data)
    {
        x = data.value("x", Mernel::PropertyTreeScalar(0)).toInt();
        y = data.value("y", Mernel::PropertyTreeScalar(0)).toInt();
    }
    void write(Mernel::PropertyTree& data) const
    {
        data["x"] = Mernel::PropertyTreeScalar(x);
        data["y"] = Mernel::PropertyTreeScalar(y);
    }

    Point& operator+=(const Point& r)
    {
        x += r.x;
        y += r.y;
        return *this;
    }
    Point& operator/=(int downscale)
    {
        x /= downscale;
        y /= downscale;
        return *this;
    }
};

struct Size {
    int width{};
    int height{};

    void read(const Mernel::PropertyTree& data)
    {
        width  = data.value("width", Mernel::PropertyTreeScalar(0)).toInt();
        height = data.value("height", Mernel::PropertyTreeScalar(0)).toInt();
    }
    void write(Mernel::PropertyTree& data) const
    {
        data["width"]  = Mernel::PropertyTreeScalar(width);
        data["height"] = Mernel::PropertyTreeScalar(height);
    }
    Size& operator/=(int downscale)
    {
        width /= downscale;
        height /= downscale;
        return *this;
    }
};

struct Rect {
    Point topLeft;
    Size  size;

    bool read(const Mernel::PropertyTree& data)
    {
        if (!data.contains("fields"))
            return false;
        if (!data["fields"].contains("rect"))
            return false;
        auto& rect = data["fields"]["rect"];
        topLeft.read(rect);
        size.read(rect);
        return true;
    }
    void write(Mernel::PropertyTree& data, bool writeSize = false) const
    {
        auto& f = data["fields"];
        f.convertToMap();
        auto& rect = f["rect"];
        if (!rect.isMap()) {
            rect = {};
            rect.convertToMap();
        }
        topLeft.write(rect);
        if (writeSize)
            size.write(rect);
    }
    Rect& operator/=(int downscale)
    {
        topLeft /= downscale;
        size /= downscale;
        return *this;
    }
};
const Point mkb1{ -14, 12 };
const Point mkb2{ 227, 12 };

const auto s_rectsCorrectionMKBD = std::map<std::string, Point>{
    { "slot_right_arm", { .x = -14, .y = 12 } },
    { "slot_left_arm", { .x = 227, .y = 12 } },
    { "slot_torso", { .x = 101, .y = -229 } },
    { "slot_head", { .x = -144, .y = 12 } },
    { "slot_gloves", { .x = 231, .y = -233 } },
    { "slot_feet", { .x = -26, .y = -231 } },
    { "slot_belt", { .x = 101, .y = -234 } },
    { "slot_neck", { .x = 99, .y = -182 } },
    { "slot_right_hand", { .x = 474, .y = -466 } },
    { "slot_left_hand", { .x = 232, .y = -466 } },

    { "close", { .x = (1382 - 1162), .y = 0 } },
    { "gold_amount", { .x = -291, .y = -1267 } },
    { "gold_button", { .x = -291, .y = -1267 } },

    { "background_right_arm", mkb1 },
    { "background_right_arm_selected", mkb1 },
    { "weaponswap_right_arm", mkb1 },
    { "text_i_left", mkb1 },
    { "text_ii_left", mkb1 },

    { "background_left_arm", mkb2 },
    { "background_left_arm_selected", mkb2 },
    { "weaponswap_left_arm", mkb2 },
    { "text_i_right", mkb2 },
    { "text_ii_right", mkb2 },

};

const Point ctrl1{ -99, -60 };
const Point ctrl2{ 123, -62 };

const auto s_rectsCorrectionCTRL = std::map<std::string, Point>{

    { "slot_right_arm", { .x = -99, .y = -60 } },
    { "slot_left_arm", { .x = 123, .y = -62 } },
    { "slot_torso", { .x = 6, .y = -199 } },

    { "slot_head", { .x = -239, .y = 21 } },
    { "slot_gloves", { .x = 146, .y = -282 } },

    { "slot_feet", { .x = -130, .y = -281 } },
    { "slot_belt", { .x = 7, .y = -185 } },
    { "slot_neck", { .x = -3, .y = -167 } },
    { "slot_right_hand", { .x = 389, .y = -417 } },
    { "slot_left_hand", { .x = 126, .y = -417 } },

    { "Belt", { .x = +15, .y = 595 } },
    { "gold_amount", { .x = -464, .y = 20 } },
    { "gold_button", { .x = -464, .y = 20 } },

    { "background_right_arm", ctrl1 },
    { "background_right_arm_selected", ctrl1 },
    { "weaponswap_right_arm", ctrl1 },
    { "text_i_left", ctrl1 },
    { "text_ii_left", ctrl1 },

    { "background_left_arm", ctrl2 },
    { "background_left_arm_selected", ctrl2 },
    { "weaponswap_left_arm", ctrl2 },
    { "text_i_right", ctrl2 },
    { "text_ii_right", ctrl2 },
};

struct DebugOverlay {
    bool controller = false;

    bool expansion = false;

    Bitmap::Color color{ .r = 255, .a = 255 };

    Rect rect;

    void paintTo(Bitmap& bitmap)
    {
        bitmap.brush = color;
        bitmap.linedRect(rect.topLeft.x, rect.topLeft.y, rect.size.width, rect.size.height);
    }

    DebugOverlay& operator/=(int downscale)
    {
        rect /= downscale;
        return *this;
    }
};
using DebugOverlayList = std::vector<DebugOverlay>;

const std::vector<Bitmap::Color> s_colors{
    Bitmap::Color(255, 0, 0, 255),
    Bitmap::Color(0, 255, 0, 255),
    Bitmap::Color(0, 0, 255, 255),
    Bitmap::Color(255, 255, 0, 255),

    Bitmap::Color(255, 0, 255, 255),
    Bitmap::Color(0, 255, 255, 255),
    Bitmap::Color(128, 128, 128, 255),
    Bitmap::Color(255, 255, 255, 255),
};

}

void ModuleExpandedInventory::generate()
{
    if (env.isLegacy)
        return;

    const auto& module = config.m_expandedInventory;
    if (module.m_inventoryHeight == 4)
        return;

    if (1) {
        return;
    }

    auto& jsonProfileHd = output.jsonFiles.at(JsonId::ProfileHd);
    auto& jsonProfileLv = output.jsonFiles.at(JsonId::ProfileLv);

    const int cellWidth  = jsonProfileHd["ItemCellSize"]["x"].getScalar().toInt();
    const int cellHeight = jsonProfileHd["ItemCellSize"]["y"].getScalar().toInt();

    const int cellCountX = module.m_inventoryWidth;
    const int cellCountY = module.m_inventoryHeight;

    int cellCountXold = 0;
    int cellCountYold = 0;

    for (auto& row : output.inventory.rows) {
        if (row.isChar2) {
            cellCountXold = row.gridX.value();
            cellCountYold = row.gridY.value();
            row.gridX     = cellCountX;
            row.gridY     = cellCountY;
        }
    }

    const int growCellX  = cellCountX - cellCountXold;
    const int growCellY  = cellCountY - cellCountYold;
    const int growWidth  = growCellX * cellWidth;
    const int growHeight = growCellY * cellHeight;

    DebugOverlayList debugOverlays;

    for (auto& desc : s_jsonDescs) {
        auto& json = output.jsonFiles.at(desc.id);

        const auto&  adjustements = desc.controller ? s_rectsCorrectionCTRL : s_rectsCorrectionMKBD;
        DebugOverlay d;
        d.expansion  = desc.expansion;
        d.controller = desc.controller;

        for (Mernel::PropertyTree& child : json["children"].getList()) {
            std::string name = child["name"].getScalar().toString();
            Rect        value;

            if (auto it = adjustements.find(name); it != adjustements.cend()) {
                const Point correction = it->second;

                if (!value.read(child))
                    continue;
                value.topLeft += correction;
                value.write(child);

                d.color = s_colors[0];
                debugOverlays.push_back(d);
            } else if (name == "grid") {
                Point correction = desc.controller ? Point{ -132, -344 } : Point{ -37, -229 };
                if (!value.read(child))
                    continue;
                value.topLeft += correction;
                value.write(child);
                value.size.width  = cellCountX * cellWidth;
                value.size.height = cellCountY * cellHeight;

                child["fields"]["cellCount"]["x"] = Mernel::PropertyTreeScalar(cellCountX);
                child["fields"]["cellCount"]["y"] = Mernel::PropertyTreeScalar(cellCountY);

                d.rect  = value;
                d.color = s_colors[1];
                debugOverlays.push_back(d);
            } else {
                continue;
            }
        }
    }

    for (const auto& desc : s_spriteDescs) {
        const auto& origSpriteBlob    = output.blobs[desc.orig];
        auto&       patchedSpriteBlob = output.blobs[desc.patched];

        Bitmap& bmp = output.bitmaps[desc.name];
        bmp.readFromSprite(origSpriteBlob);

        const Bitmap origBmp = bmp;

        Size newSize = desc.controller ? Size{ 1381, 1582 } : Size{ 1381, 1507 };
        newSize /= desc.lowres ? 2 : 1;
        bmp.resize(newSize.width, newSize.height);
        bmp.brush = Bitmap::Color{ 20, 20, 20, 255 };
        bmp.fill();
        Bitmap::blit(origBmp, 0, 0, bmp, 0, 0, origBmp.width, origBmp.height);

        bmp.writeToSprite(patchedSpriteBlob);

        for (auto overlay : debugOverlays) {
            if (overlay.controller != desc.controller)
                continue;
            if (!desc.expansion && overlay.expansion)
                continue;

            overlay /= desc.lowres ? 2 : 1;
            overlay.paintTo(bmp);
        }
    }
}

}
