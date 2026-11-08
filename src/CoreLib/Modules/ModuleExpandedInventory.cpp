/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "ModuleExpandedInventory.hpp"

#include "Logger.hpp"

namespace D2ModGen {

namespace {

void blitFast(const Bitmap& src, int srcX, int srcY, Bitmap& dest, int destX, int destY, int width, int height)
{
    int startX = std::max(0, -destX);
    int startY = std::max(0, -destY);

    int endX = std::min(width, static_cast<int>(src.width) - srcX);
    endX     = std::min(endX, static_cast<int>(dest.width) - destX);

    int endY = std::min(height, static_cast<int>(src.height) - srcY);
    endY     = std::min(endY, static_cast<int>(dest.height) - destY);

    for (int y = startY; y < endY; ++y) {
        const uint8_t* srcRow  = src.rgba.data() + ((srcY + y) * src.width + (srcX + startX)) * 4;
        uint8_t*       destRow = dest.rgba.data() + ((destY + y) * dest.width + (destX + startX)) * 4;

        for (int x = startX; x < endX; ++x) {
            if (srcRow[3] == 255) {
                memcpy(destRow, srcRow, 4);
            }
            srcRow += 4;
            destRow += 4;
        }
    }
}

struct Point {
    int x{};
    int y{};

    void read(const Mernel::PropertyTree& data)
    {
        x = data["x"].getScalar().toInt();
        y = data["y"].getScalar().toInt();
    }
    void write(Mernel::PropertyTree& data) const
    {
        data["x"] = Mernel::PropertyTreeScalar(x);
        data["y"] = Mernel::PropertyTreeScalar(y);
    }
};

struct InvSlot {
    Bitmap::Color color{ .r = 255, .a = 255 };

    std::string name; // "slot_something";

    void scale(int mult, int div)
    {
        x  = x * mult / div;
        y  = y * mult / div;
        wi = wi * mult / div;
        he = he * mult / div;
    }

    int x{};
    int y{};
    int wi{};
    int he{};

    bool haveStoredWidthHeight = true;

    void read(const Mernel::PropertyTree& data)
    {
        auto& rect = data["fields"]["rect"];
        x          = rect["x"].getScalar().toInt();
        y          = rect["y"].getScalar().toInt();
        if (rect.contains("width")) {
            haveStoredWidthHeight = false;
            wi                    = rect["width"].getScalar().toInt();
            he                    = rect["height"].getScalar().toInt();
        }
    }
    void write(Mernel::PropertyTree& data) const
    {
        auto& rect = data["fields"]["rect"];
        rect["x"]  = Mernel::PropertyTreeScalar(x);
        rect["y"]  = Mernel::PropertyTreeScalar(y);
        if (haveStoredWidthHeight) {
            rect["width"]  = Mernel::PropertyTreeScalar(wi);
            rect["height"] = Mernel::PropertyTreeScalar(he);
        }
    }

    std::string toString() const
    {
        return "{ x: " + std::to_string(x) + ", y: " + std::to_string(x) + ", wi: " + std::to_string(wi) + ", he: " + std::to_string(he) + " }";
    }

    void paintTo(Bitmap& bitmap, int offsetX, int offsetY)
    {
        bitmap.brush = color;
        const int x1 = x + offsetX;
        const int y1 = y + offsetY;
        const int x2 = x + wi - 1 + offsetX;
        const int y2 = y + he - 1 + offsetY;
        bitmap.paintLineH(x1, x2, y1);
        bitmap.paintLineH(x1, x2, y2);
        bitmap.paintLineV(x1, y1, y2);
        bitmap.paintLineV(x2, y1, y2);
    }
};
using InvSlotList = std::vector<InvSlot>;

struct Rect {
    std::string name;
    int         x{};
    int         y{};
    int         w{};
    int         h{};
};

const auto s_rects = std::vector<Rect>{

    { .name = "background", .x = -166, .y = -160 },

    { .name = "grid", .x = -132, .y = -344 },
    { .name = "slot_right_arm", .x = -99, .y = -60 },
    { .name = "slot_left_arm", .x = 123, .y = -62 },
    { .name = "slot_torso", .x = 6, .y = -199 },
    { .name = "slot_head", .x = -239, .y = 21 },
    { .name = "slot_gloves", .x = 146, .y = -282 },
    { .name = "slot_feet", .x = -130, .y = -281 },
    { .name = "slot_belt", .x = 7, .y = -185 },
    { .name = "slot_neck", .x = -3, .y = -167 },
    { .name = "slot_right_hand", .x = 389, .y = -417 },
    { .name = "slot_left_hand", .x = 126, .y = -417 },
    { .name = "Belt", .x = 15, .y = 595 },
    { .name = "gold_amount", .x = -464, .y = 20 },
    { .name = "gold_button", .x = -464, .y = 20 }
};

const auto s_rects_set2 = std::vector<Rect>{
    { .name = "grid", .x = -37, .y = -229 },
    { .name = "slot_right_arm", .x = -14, .y = 12 },
    { .name = "slot_left_arm", .x = 227, .y = 12 },
    { .name = "slot_torso", .x = 101, .y = -229 },
    { .name = "slot_head", .x = -144, .y = 12 },
    { .name = "slot_gloves", .x = 231, .y = -233 },
    { .name = "slot_feet", .x = -26, .y = -231 },
    { .name = "slot_belt", .x = 101, .y = -234 },
    { .name = "slot_neck", .x = 99, .y = -182 },
    { .name = "slot_right_hand", .x = 474, .y = -466 },
    { .name = "slot_left_hand", .x = 232, .y = -466 },
    { .name = "gold_amount", .x = -291, .y = -1267 },
    { .name = "gold_button", .x = -291, .y = -1267 },

    /*
     
 if (child.name === 'background') {
      child.fields.filename = 'PANEL\\Inventory\\Classic_Background_Expanded';
    }
    if (child.name === 'click_catcher') {
      child.fields.rect = { x: 0, y: 45, width: 1093, height: 1495 };
    }
    if (child.name === 'RightHinge') {
      child.fields.rect = '$RightHingeRect_ExpandedInventory';
    }
    if (child.name === 'title') {
      child.fields.rect = {
        x: 91 + (1382 - 1162) / 2,
        y: 64,
        width: 972,
        height: 71,
      };
    }
    if (child.name === 'close') {
      child.fields.rect.x = child.fields.rect.x + (1382 - 1162);
    }
    if (child.name === 'grid') {
      child.fields.cellCount.x = 13;
      child.fields.cellCount.y = 8;
      child.fields.rect.x = child.fields.rect.x - 37;
      child.fields.rect.y = child.fields.rect.y - 229;
    }
*/
};

struct ASprite {
    BlobId      id;
    std::string bitmapKey;

    std::string blob;

    Bitmap bmp;

    Bitmap::Color dbgColor;

    ASprite() = default;
    ASprite(BlobId id_, std::string bitmapKey_)
        : id(id_)
        , bitmapKey(bitmapKey_)
    {}

    void read(DataContext& output)
    {
        if (output.blobs.contains(id)) {
            blob = output.blobs.at(id);
            bmp.readFromSprite(blob);
        }
    }
    void write(DataContext& output)
    {
        bmp.writeToSprite(blob);
        output.blobs[id]          = blob;
        output.bitmaps[bitmapKey] = bmp;
    }

    void fillWithDebugColor()
    {
        //bmp.fillRect(0, 0, bmp.width, bmp.height, dbgColor);

        bmp.brush = dbgColor;
        bmp.linedRect(0, 0, bmp.width, bmp.height);
        bmp.linedRect(10, 10, bmp.width - 20, bmp.height - 20);
        //bmp.fillRect(0, 0, bmp.width, bmp.height, dbgColor);
    }
};

}

void ModuleExpandedInventory::generate()
{
    if (env.isLegacy)
        return;

    const auto& module = config.m_expandedInventory;
    if (module.m_inventoryHeight == 4)
        return;

    auto& jsonProfileHd   = output.jsonFiles.at(JsonId::ProfileHd);
    auto& jsonProfileLv   = output.jsonFiles.at(JsonId::ProfileLv);
    auto& jsonLCClassic   = output.jsonFiles.at(JsonId::LayoutControllerClassic);
    auto& jsonLCExpansion = output.jsonFiles.at(JsonId::LayoutControllerExpansion);
    auto& jsonClassic     = output.jsonFiles.at(JsonId::LayoutClassic);
    auto& jsonExpansion   = output.jsonFiles.at(JsonId::LayoutExpansion);

    std::vector<Bitmap::Color> colors{
        // Bitmap::Color(255, 0, 0, 255),
        //Bitmap::Color(0, 255, 0, 255),
        //Bitmap::Color(0, 0, 255, 255),
        Bitmap::Color(255, 255, 0, 255),

        Bitmap::Color(255, 0, 255, 255),
        Bitmap::Color(0, 255, 255, 255),
        Bitmap::Color(128, 128, 128, 255),
        Bitmap::Color(255, 255, 255, 255),
    };
    size_t color_index = 0;

    std::map<BlobId, ASprite> sprites;
    auto                      add = [&sprites, &colors, &color_index, this](BlobId b, std::string s) {
        ASprite& sprite = sprites[b];
        sprite          = ASprite(b, s);

        sprite.read(output);
        sprite.dbgColor = colors[color_index++];
    };

    //add(BlobId::InventorySprite, "InventorySprite");
    //add(BlobId::InventorySpriteLowend, "InventorySpriteLowend");
    //add(BlobId::SidePanelRSprite, "SidePanelRSprite");
    //add(BlobId::SidePanelRSpriteLowend, "SidePanelRSpriteLowend");
    add(BlobId::BackgroundSprite, "BackgroundSprite");
    add(BlobId::BackgroundSpriteLowend, "BackgroundSpriteLowend");

    {
        Point rightHingeRect;
        rightHingeRect.read(jsonProfileHd["RightHingeRect"]);
        rightHingeRect.x += 25;
        rightHingeRect.write(jsonProfileHd["RightHingeRect"]);
    }

    //add(BlobId::ModdedBackgroundSprite, "BackgroundSprite");
    // add(BlobId::ModdedBackgroundSpriteLowend, "BackgroundSpriteLowend");

    // sprites[BlobId::ModdedBackgroundSprite]       = sprites[BlobId::BackgroundSprite];
    // sprites[BlobId::ModdedBackgroundSpriteLowend] = sprites[BlobId::BackgroundSpriteLowend];

    sprites[BlobId::BackgroundSprite].fillWithDebugColor();

    int       scale      = 1;
    const int cellWidth  = jsonProfileHd["ItemCellSize"]["x"].getScalar().toInt() / scale;
    const int cellHeight = jsonProfileHd["ItemCellSize"]["y"].getScalar().toInt() / scale;

    int cellCountX = 0;
    int cellCountY = 0;

    int magicOffsetX = -124 / scale;
    int magicOffsetY = +74 / scale;

    int magicOffsetOverX = -198 / scale;
    int magicOffsetOverY = -42 / scale;

    for (auto& row : output.inventory.rows) {
        if (row.isChar2) {
            cellCountX = row.gridX.value();
            cellCountY = row.gridY.value();
            break;
        }
    }

    using Color = Bitmap::Color;

    //bmp.readFromSprite(blobInventorySprite);

    InvSlotList origSlots;

    int gridStartX = 0;
    int gridStartY = 0;

    for (const Mernel::PropertyTree& child : jsonLCClassic["children"].getList()) {
        InvSlot invslot;
        invslot.name = child["name"].getScalar().toString();
        if (invslot.name.starts_with("slot_")) {
            invslot.read(child);
            invslot.scale(1, scale);
            origSlots.push_back(invslot);
        }
        if (invslot.name == "grid") {
            invslot.read(child);
            invslot.scale(1, scale);
            invslot.wi = cellWidth * cellCountX;
            invslot.he = cellHeight * cellCountY;

            gridStartX    = invslot.x;
            gridStartY    = invslot.y;
            invslot.color = Bitmap::Color{ .g = 255, .a = 255 };
            origSlots.push_back(invslot);
        }
    }
    gridStartX += cellWidth;
    gridStartY += cellHeight;

    auto& bmpBack = sprites[BlobId::BackgroundSprite].bmp;
    // auto& bmpOver = bmpInventorySpriteLowend;
    for (auto& invslot : origSlots) {
        Logger() << "slot '" << invslot.name << "' = " << invslot.toString();
        invslot.paintTo(bmpBack, magicOffsetX, magicOffsetY);
        //invslot.paintTo(bmpOver, magicOffsetOverX, magicOffsetOverY);
    }

    for (auto& [blobid, sprite] : sprites) {
        //sprite.fillWithDebugColor();
        sprite.write(output);
    }

    /*
    bmpBack.fillRect(gridStartX + magicOffsetX,
                     gridStartY + magicOffsetY,
                     cellWidth,
                     cellHeight,
                     Color{ .r = 255, .g = 255, .a = 255 });
    bmpOver.fillRect(gridStartX + magicOffsetOverX,
                     gridStartY + magicOffsetOverY,
                     cellWidth,
                     cellHeight,
                     Color{ .r = 255, .b = 255, .a = 255 });

    bmpBack.writeToSprite(blobBackgroundSpriteLowend);
    bmpOver.writeToSprite(blobInventorySpriteLowend);*/

    /*
    auto& bmp = output.bitmaps["expanded"];
    bmp.resize(50, 50);

    auto draw = [&bmp](int x, int y, int w, int h, Color c) {
        for (int x1 = 0; x1 < w; x1++) {
            for (int y1 = 0; y1 < h; y1++) {
                bmp.setColor(x + x1, y + y1, c);
            }
        }
    };

    draw(10, 10, 10, 20, { .r = 255, .g = 0, .b = 0, .a = 255 });     // wide red
    draw(20, 20, 20, 10, { .r = 0, .g = 255, .b = 255, .a = 255 });   // tall cyan
    draw(40, 40, 10, 10, { .r = 128, .g = 128, .b = 128, .a = 128 }); // half-opaque gray
    */

    /*
    const std::string profileHDFilename = "global\\ui\\layouts\\_profilehd.json";
    //const profileHD = D2RMM.readJson(profileHDFilename);
    Rect profileHD_RightPanelRect_ExpandedInventory{
        .x = -1394 - (1382 - 1162),
        .y = -651,
        .w = 1382,
        .h = 1507,
    };
    Rect profileHD_PanelClickCatcherRect_ExpandedInventory = {
        .x = 0,
        .y = 0,
        .w = 1162,
        .h = 1507,
    };
    // offset the right hinge so that it doesn't overlap with content of the right panel
    Rect profileHD_RightHingeRect                   = { .x = 1076 + 20, .y = 630 };
    Rect profileHD_RightHingeRect_ExpandedInventory = {
        .x = 1076 + (1382 - 1162) + 20,
        .y = 630,
    };
    //D2RMM.writeJson(profileHDFilename, profileHD);

    const std::string profileLVFilename = "global\\ui\\layouts\\_profilelv.json";
    //const profileLV = D2RMM.readJson(profileLVFilename);
    Rect profileLV_RightPanelRect_ExpandedInventory = {
        .x = int(-1346 - (1382 - 1162) * 1.16),
        .y = -856,
        .w = 1382,
        .h = 1507,
        // scale: 1.16,
    };
    //D2RMM.writeJson(profileLVFilename, profileLV);

    const std::string playerInventoryOriginalLayoutHDFilename = "global\\ui\\layouts\\playerinventoryoriginallayouthd.json";
    //const playerInventoryOriginalLayoutHD = D2RMM.readJson(
    //  playerInventoryOriginalLayoutHDFilename,
    //);
    // playerInventoryOriginalLayoutHD.fields.rect =
    //   '$RightPanelRect_ExpandedInventory';
    // playerInventoryOriginalLayoutHD.children =
    //   playerInventoryOriginalLayoutHD.children.filter((child) => {

*/
}

}
