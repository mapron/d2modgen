/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleExpandedInventory.hpp"

#include "Logger.hpp"
#include "Image.hpp"

namespace D2ModGen {
using namespace Image;

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

struct Field {
    std::string name;
    Rect        rect;

    bool read(const Mernel::PropertyTree& data)
    {
        rect = {};
        if (!data.contains("fields"))
            return false;
        if (!data["fields"].contains("rect"))
            return false;
        auto& rect_ = data["fields"]["rect"];

        return rect.read(rect_);
    }
    void write(Mernel::PropertyTree& data, bool writeSize = false) const
    {
        auto& f = data["fields"];
        f.convertToMap();
        auto& rect_ = f["rect"];
        if (!rect_.isMap()) {
            rect_ = {};
            rect_.convertToMap();
        }
        rect.write(rect_, writeSize);
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

// Controller backgrounds are anchored at the panel center, so their sprites do not start at the panel origin.
const Point s_ctrlPanelCenter{ 1408 / 2, 1486 / 2 };
const Point s_ctrlBackgroundRectOrig{ -508, -700 };
const Point s_ctrlBackgroundRectExpanded{ -789, -860 };
const Point s_ctrlSpriteOffsetOrig{ -(s_ctrlPanelCenter.x + s_ctrlBackgroundRectOrig.x), -(s_ctrlPanelCenter.y + s_ctrlBackgroundRectOrig.y) };
const Point s_ctrlSpriteOffsetExpanded{ -(s_ctrlPanelCenter.x + s_ctrlBackgroundRectExpanded.x), -(s_ctrlPanelCenter.y + s_ctrlBackgroundRectExpanded.y) };

// Stone area of the biggest panel in controller mainpanelbg.sprite, without its frame and corner ornaments.
const Rect s_panelBgStoneRect{ .topLeft = { 645, 202 }, .size = { 1590, 1366 } };
const int  s_panelBgStoneRepeatRow = 1056;

const int  s_ctrlFrameMargin = 12; // controller frames extend past the widget rect
const Size s_ctrlBeltSize{ 505, 118 };

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

    { "Belt", { .x = +223, .y = 595 } },
    { "gold_amount", { .x = -256, .y = 20 } },
    { "gold_button", { .x = -256, .y = 20 } },

    { "background_right_arm", ctrl1 },
    { "background_right_arm_selected", ctrl1 },
    { "WeaponSwapRightLegend", ctrl1 },
    { "text_i_left", ctrl1 },
    { "text_ii_left", ctrl1 },

    { "background_left_arm", ctrl2 },
    { "background_left_arm_selected", ctrl2 },
    { "WeaponSwapLeftLegend", ctrl2 },
    { "text_i_right", ctrl2 },
    { "text_ii_right", ctrl2 },
};

struct DebugOverlay : public Field {
    bool controller = false;

    bool expansion = false;

    Color color{ .r = 255, .a = 255 };

    void paintTo(Bitmap& bitmap)
    {
        bitmap.brush = color;
        bitmap.linedRect(rect);
    }

    DebugOverlay& operator/=(int downscale)
    {
        rect /= downscale;
        return *this;
    }
    std::string toString() const { return name + ": " + rect.toString() + " " + (expansion ? "EXP " : "CLA ") + (controller ? "CTRL " : "MKB "); }
};
using DebugOverlayList = std::vector<DebugOverlay>;

const std::vector<Color> s_colors{
    Color(255, 0, 0, 255),
    Color(0, 255, 0, 255),
    Color(0, 0, 255, 255),
    Color(255, 255, 0, 255),

    Color(255, 0, 255, 255),
    Color(0, 255, 255, 255),
    Color(128, 128, 128, 255),
    Color(255, 255, 255, 255),
};

struct BlitDesc {
    Rect  rect;
    Point moveTo;
    int   repeatX = 1;
    int   repeatY = 1;

    bool controller = false;
    bool expansion  = false;

    BlitDesc& operator/=(int downscale)
    {
        rect /= downscale;
        moveTo /= downscale;
        return *this;
    }
};
using BlitDescList = std::vector<BlitDesc>;

}

void ModuleExpandedInventory::generate()
{
    if (env.isLegacy)
        return;

    const auto& module = config.m_expandedInventory;
    if (module.m_inventoryHeight == 4)
        return;

    auto& jsonProfileHd = output.jsonFiles.at(JsonId::ProfileHd);
    auto& jsonProfileLv = output.jsonFiles.at(JsonId::ProfileLv);

    {
        Mernel::PropertyTree rect;

        rect["height"] = Mernel::PropertyTreeScalar(1507);
        rect["scale"]  = Mernel::PropertyTreeScalar(1.16);
        rect["width"]  = Mernel::PropertyTreeScalar(1382 + 200);
        rect["x"]      = Mernel::PropertyTreeScalar(-1601.2 - 200);
        rect["y"]      = Mernel::PropertyTreeScalar(-856);

        jsonProfileLv["RightPanelRect_ExpandedInventory"] = rect;
    }
    {
        Mernel::PropertyTree rect;

        rect["height"] = Mernel::PropertyTreeScalar(1507);
        rect["width"]  = Mernel::PropertyTreeScalar(1382 + 200);
        rect["x"]      = Mernel::PropertyTreeScalar(-1614 - 200);
        rect["y"]      = Mernel::PropertyTreeScalar(-651);

        jsonProfileHd["RightPanelRect_ExpandedInventory"] = rect;
    }
    {
        Mernel::PropertyTree rect;

        rect["x"] = Mernel::PropertyTreeScalar(1316 + 200);
        rect["y"] = Mernel::PropertyTreeScalar(630);

        jsonProfileHd["RightHingeRect_ExpandedInventory"] = rect;
        jsonProfileHd["RightHingeRect"]["x"]              = Mernel::PropertyTreeScalar(1096);
    }
    {
        Mernel::PropertyTree rect;

        rect["height"] = Mernel::PropertyTreeScalar(1495);
        rect["width"]  = Mernel::PropertyTreeScalar(1093 + 200);
        rect["x"]      = Mernel::PropertyTreeScalar(0);
        rect["y"]      = Mernel::PropertyTreeScalar(45);

        jsonProfileHd["PanelClickCatcherRect_ExpandedInventory"] = rect;
    }
    {
        Mernel::PropertyTree rect;

        rect["height"] = Mernel::PropertyTreeScalar(71);
        rect["width"]  = Mernel::PropertyTreeScalar(972 + 200);
        rect["x"]      = Mernel::PropertyTreeScalar(201);
        rect["y"]      = Mernel::PropertyTreeScalar(64);

        jsonProfileHd["TitleRect_ExpandedInventory"] = rect;
    }
    const int cellWidth  = jsonProfileHd["ItemCellSize"]["x"].getScalar().toInt();
    const int cellHeight = jsonProfileHd["ItemCellSize"]["y"].getScalar().toInt();

    const int cellCountX    = module.m_inventoryWidth;
    const int cellCountY    = module.m_inventoryHeight;
    const int cellCountXmax = 15;
    const int cellCountYmax = 8;

    int cellCountXold = 0;
    int cellCountYold = 0;

    for (auto& row : output.inventory.rows) {
        if (row.isPlayable) {
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

    const auto gridOffset = Point{ (cellCountXmax - cellCountX) * cellWidth / 2, (cellCountYmax - cellCountY) * cellHeight / 2 };

    DebugOverlayList debugOverlays;
    BlitDescList     blitList;

    for (auto& desc : s_jsonDescs) {
        auto& json = output.jsonFiles.at(desc.id);

        const auto&  adjustements = desc.controller ? s_rectsCorrectionCTRL : s_rectsCorrectionMKBD;
        DebugOverlay d;
        d.expansion  = desc.expansion;
        d.controller = desc.controller;
        BlitDesc bd;
        bd.expansion  = desc.expansion;
        bd.controller = desc.controller;

        const Point sourceOffset = desc.controller ? s_ctrlSpriteOffsetOrig : Point{};
        const Point targetOffset = desc.controller ? s_ctrlSpriteOffsetExpanded : Point{};

        const Point widgetsOffset = desc.controller ? Point{ -15, 0 } : Point{ 100, 0 };

        if (!desc.expansion && !desc.controller)
            json["fields"]["rect"] = Mernel::PropertyTreeScalar("$RightPanelRect_ExpandedInventory");

        for (Mernel::PropertyTree& child : json["children"].getList()) {
            d.name     = child["name"].getScalar().toString();
            d.color    = s_colors[0];
            bd.repeatX = 1;
            bd.repeatY = 1;

            if (auto it = adjustements.find(d.name); it != adjustements.cend()) {
                const Point correction = it->second;

                if (!d.read(child))
                    continue;

                if (desc.controller && d.name == "Belt")
                    d.rect.size = s_ctrlBeltSize; // the layout only stores its position

                bd.rect = d.rect;
                d.rect.topLeft += correction;
                d.rect.topLeft += widgetsOffset; // @todo: future uncollapsing
                bd.moveTo = d.rect.topLeft + targetOffset;

                if (bd.rect.size.width > 0) {
                    const int margin = desc.controller ? s_ctrlFrameMargin : 10; // extra borders
                    bd.rect.expand(margin);
                    bd.moveTo += Point{ -margin, -margin };
                }
                bd.rect.topLeft += sourceOffset;

                d.write(child);
                blitList.push_back(bd);

                //Logger() << d.name << " " << bd.rect.toString() << " -> " << bd.moveTo.toString();

            } else if (d.name == "grid") {
                Point correction = desc.controller ? Point{ -247, -344 } : Point{ -37, -229 };
                if (!d.read(child))
                    continue;

                bd.rect = d.rect;
                bd.rect.topLeft += sourceOffset;
                d.rect.topLeft += correction;
                d.rect.topLeft += gridOffset;

                bd.rect.size.width  = cellWidth;
                bd.rect.size.height = cellHeight;
                bd.moveTo           = d.rect.topLeft + targetOffset;
                bd.repeatX          = cellCountX;
                bd.repeatY          = cellCountY;

                d.write(child);
                blitList.push_back(bd);

                d.rect.size.width  = cellCountX * cellWidth;
                d.rect.size.height = cellCountY * cellHeight;

                child["fields"]["cellCount"]["x"] = Mernel::PropertyTreeScalar(cellCountX);
                child["fields"]["cellCount"]["y"] = Mernel::PropertyTreeScalar(cellCountY);

                d.color = s_colors[1];
            } else if (d.name == "click_catcher") {
                if (desc.controller) {
                    //
                } else {
                    child["fields"]["rect"] = Mernel::PropertyTreeScalar("$PanelClickCatcherRect_ExpandedInventory");
                }

            } else if (d.name == "background") {
                if (desc.controller) {
                    child["fields"]["filename"] = Mernel::PropertyTreeScalar("Controller/Panel/InventoryPanel/V2/InventoryBG_Expanded");
                    if (!desc.expansion) {
                        child["fields"]["filename"]  = Mernel::PropertyTreeScalar("Controller/Panel/InventoryPanel/V2/InventoryBG_Classic_Expanded");
                        child["fields"]["rect"]["x"] = Mernel::PropertyTreeScalar(s_ctrlBackgroundRectExpanded.x);
                        child["fields"]["rect"]["y"] = Mernel::PropertyTreeScalar(s_ctrlBackgroundRectExpanded.y);
                    }
                } else {
                    child["fields"]["filename"] = Mernel::PropertyTreeScalar("PANEL\\Inventory\\Background_Expanded");
                    if (!desc.expansion) {
                        child["fields"]["filename"] = Mernel::PropertyTreeScalar("PANEL\\Inventory\\Classic_Background_Expanded");
                    }
                }

            } else if (d.name == "RightHinge") {
                child["fields"]["rect"] = Mernel::PropertyTreeScalar("$RightHingeRect_ExpandedInventory");
            } else if (d.name == "title") {
                if (!desc.expansion && !desc.controller)
                    child["fields"]["rect"] = Mernel::PropertyTreeScalar("$TitleRect_ExpandedInventory");
            } else {
                continue;
            }
            debugOverlays.push_back(d);
        }
    }

    Bitmap panelBgHighre, panelBgLowend;
    panelBgHighre.readFromSprite(output.blobs[BlobId::OrignCtrlMainPanelBgSpriteHighre]);
    panelBgLowend.readFromSprite(output.blobs[BlobId::OrignCtrlMainPanelBgSpriteLowend]);

    for (const auto& desc : s_spriteDescs) {
        const auto& origSpriteBlob    = output.blobs[desc.orig];
        auto&       patchedSpriteBlob = output.blobs[desc.patched];
        const int   downscale         = desc.lowres ? 2 : 1;

        Bitmap& origBmp = output.bitmaps[desc.name + "_orig"];
        Bitmap& bmp     = output.bitmaps[desc.name];
        bmp.readFromSprite(origSpriteBlob);

        origBmp = bmp;

        Size newSize    = desc.controller ? Size{ 1381 + 200, 1582 } : Size{ 1381 + 200, 1507 };
        Size cornerSize = Size{ 65, 65 };
        newSize /= downscale;
        cornerSize /= downscale;
        bmp.resize(newSize);
        bmp.frameWidth = newSize.width;
        if (desc.controller) {
            bmp.rgba.assign(bmp.rgba.size(), 0); // the game draws its own panel behind the controller background
        } else {
            const Bitmap& panelBg   = desc.lowres ? panelBgLowend : panelBgHighre;
            Rect          stoneRect = s_panelBgStoneRect;
            stoneRect /= downscale;
            const int repeatRow   = s_panelBgStoneRepeatRow / downscale;
            const int extraHeight = newSize.height - stoneRect.size.height;
            bmp.draw({}, panelBg, stoneRect.topLeft, { newSize.width, repeatRow + extraHeight });
            bmp.draw({ 0, repeatRow + extraHeight }, panelBg, stoneRect.topLeft + Point{ 0, repeatRow }, { newSize.width, stoneRect.size.height - repeatRow });
            bmp.copyBorders(origBmp, cornerSize);
        }

        for (BlitDesc bd : blitList) {
            if (bd.controller != desc.controller)
                continue;
            if (!desc.expansion && bd.expansion)
                continue;
            bd /= downscale;
            for (int x = 0, offsetX = 0; x < bd.repeatX; x++, offsetX += bd.rect.size.width) {
                for (int y = 0, offsetY = 0; y < bd.repeatY; y++, offsetY += bd.rect.size.height) {
                    bmp.draw(bd.moveTo + Point(offsetX, offsetY), origBmp, bd.rect.topLeft, bd.rect.size);
                }
            }
        }

        bmp.writeToSprite(patchedSpriteBlob);

        for (auto overlay : debugOverlays) {
            if (overlay.controller != desc.controller)
                continue;
            if (!desc.expansion && overlay.expansion)
                continue;

            if (overlay.controller)
                overlay.rect.topLeft += s_ctrlSpriteOffsetExpanded;
            overlay /= desc.lowres ? 2 : 1;
            overlay.paintTo(bmp);
        }
    }
}

}
