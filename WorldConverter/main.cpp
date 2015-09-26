#include <cassert>
#include <cinttypes>
#include <string>
#include <vector>

#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

struct sTile
{
    int id = 0;
    int flag = 0;
    int pal = 0;
};

enum class eDir : uint8_t
{
    HORIZONTAL = 0,
    VERTICAL = 0x4
};

enum eTheme : uint8_t
{
    THEME_PIPES = 0
};

enum eEntityType : uint8_t
{
    ENTITY_PLAYER_START = 0,
    ENTITY_MONSTER1 = 1,
    ENTITY_DOOR = 2
};

struct sEntity
{
    int x, y;
    int id = 0;
    int to = 0;
    eEntityType type;
};

struct sZone
{
    int x = 0, y = 0;
    eDir dir = eDir::HORIZONTAL;
    int size = 1;
    int id = 0;
    eTheme theme = THEME_PIPES;
    vector<sEntity> entities;
};

int w;
int h;
sTile *pTiles = nullptr;

vector<sZone> zones;

vector<string> splitString(const string& in_string, char in_delimiter)
{
    vector<string> elems;
    unsigned int start = 0;
    unsigned int end = 0;
    for (; end < in_string.length(); ++end)
    {
        if (in_string[end] == in_delimiter)
        {
            if (end - start)
            {
                elems.push_back(in_string.substr(start, end - start));
            }
            start = end + 1;
        }
    }
    if (start < in_string.length())
    {
        if (end - start)
        {
            elems.push_back(in_string.substr(start, end - start));
        }
    }
    return elems;
}

#define FLAG_COLLISION 0x01

int main()
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile("../world.tmx");
    assert(!doc.Error());
    auto pXMLMap = doc.FirstChildElement("map");
    assert(pXMLMap);

    w = pXMLMap->IntAttribute("width");
    h = pXMLMap->IntAttribute("height");
    pTiles = new sTile[w * h];

    for (auto pXMLLayer = pXMLMap->FirstChildElement("layer"); pXMLLayer; pXMLLayer = pXMLLayer->NextSiblingElement("layer"))
    {
        string name = pXMLLayer->Attribute("name");

        if (name == "Tiles")
        {
            auto pXMLData = pXMLLayer->FirstChildElement("data");
            int i = 0;
            for (auto pXMLTile = pXMLData->FirstChildElement("tile"); pXMLTile; pXMLTile = pXMLTile->NextSiblingElement("tile"))
            {
                auto gid = pXMLTile->IntAttribute("gid");
                pTiles[i].id = (gid - 1) % 240;
                pTiles[i].pal = (gid - 1) / 240;
                ++i;
            }
        }
        else if (name == "Collisions")
        {
            auto pXMLData = pXMLLayer->FirstChildElement("data");
            int i = 0;
            for (auto pXMLTile = pXMLData->FirstChildElement("tile"); pXMLTile; pXMLTile = pXMLTile->NextSiblingElement("tile"))
            {
                auto gid = pXMLTile->IntAttribute("gid");
                if (gid % 240 == 2)
                {
                    pTiles[i].flag |= FLAG_COLLISION;
                }
                ++i;
            }
        }
    }

    int startZoneId = 0;
    sEntity playerStart;

    for (auto pXMLLayer = pXMLMap->FirstChildElement("objectgroup"); pXMLLayer; pXMLLayer = pXMLLayer->NextSiblingElement("objectgroup"))
    {
        string name = pXMLLayer->Attribute("name");

        if (name == "Zones")
        {
            for (auto pXMLObject = pXMLLayer->FirstChildElement("object"); pXMLObject; pXMLObject = pXMLObject->NextSiblingElement("object"))
            {
                sZone zone;

                auto zoneX = pXMLObject->IntAttribute("x");
                auto zoneY = pXMLObject->IntAttribute("y");
                auto zoneW = pXMLObject->IntAttribute("width");
                auto zoneH = pXMLObject->IntAttribute("height");

                zone.x = zoneX / 256;
                zone.y = zoneY / 208;

                if (zoneH > zoneW)
                {
                    zone.dir = eDir::VERTICAL;
                    zone.size = zoneH / 208;
                }
                else
                {
                    zone.size = zoneW / 256;
                }

                auto pXMLProperties = pXMLObject->FirstChildElement("properties");
                if (pXMLProperties)
                {
                    for (auto pXMLProperty = pXMLProperties->FirstChildElement("property"); pXMLProperty; pXMLProperty = pXMLProperty->NextSiblingElement("property"))
                    {
                        string propName = pXMLProperty->Attribute("name");
                        if (propName == "id")
                        {
                            zone.id = pXMLProperty->IntAttribute("value");
                        }
                        else if (propName == "theme")
                        {
                            zone.theme = (eTheme)pXMLProperty->IntAttribute("value");
                        }
                    }
                }

                zones.push_back(zone);
            }
        }

        if (name == "Entities")
        {
            for (auto pXMLObject = pXMLLayer->FirstChildElement("object"); pXMLObject; pXMLObject = pXMLObject->NextSiblingElement("object"))
            {
                sEntity entity;
                string type = pXMLObject->Attribute("type");

                auto x = pXMLObject->IntAttribute("x");
                auto y = pXMLObject->IntAttribute("y");
                auto ww = pXMLObject->IntAttribute("width");
                auto hh = pXMLObject->IntAttribute("height");

                if (type == "PlayerStart")
                {
                    entity.x = (x + ww / 2);
                    entity.y = y + hh - 1;
                    entity.type = eEntityType::ENTITY_PLAYER_START;
                }
                else if (type == "Monster1")
                {
                    entity.x = (x + ww / 2);
                    entity.y = y + hh - 1;
                    entity.type = eEntityType::ENTITY_MONSTER1;
                }
                else if (type == "Door")
                {
                    entity.x = x;
                    entity.y = y;
                    entity.type = eEntityType::ENTITY_DOOR;

                    auto pXMLProperties = pXMLObject->FirstChildElement("properties");
                    if (pXMLProperties)
                    {
                        for (auto pXMLProperty = pXMLProperties->FirstChildElement("property"); pXMLProperty; pXMLProperty = pXMLProperty->NextSiblingElement("property"))
                        {
                            string propName = pXMLProperty->Attribute("name");
                            if (propName == "id")
                            {
                                entity.id = pXMLProperty->IntAttribute("value");
                            }
                            else if (propName == "to")
                            {
                                entity.to = (eTheme)pXMLProperty->IntAttribute("value");
                            }
                        }
                    }
                }

                // Find its zone
                int zoneId = 0;
                for (auto &zone : zones)
                {
                    zone.id = zoneId;
                    if ((zone.dir == eDir::HORIZONTAL &&
                        entity.x >= zone.x * 256 &&
                        entity.x < (zone.x + zone.size) * 256 &&
                        entity.y >= zone.y * 208 &&
                        entity.y < zone.y * 208 + 208) ||
                        (zone.dir == eDir::VERTICAL &&
                        entity.x >= zone.x * 256 &&
                        entity.x < zone.x * 256 + 256 &&
                        entity.y >= zone.y * 208 &&
                        entity.y < (zone.y + zone.size) * 208))
                    {
                        playerStart.x -= zone.x * 256;
                        playerStart.y -= zone.y * 208;
                        if (entity.type == eEntityType::ENTITY_PLAYER_START)
                        {
                            playerStart = entity;
                            startZoneId = zoneId;
                        }
                        else
                        {
                            zone.entities.push_back(entity);
                        }
                    }
                    ++zoneId;
                }
            }
        }
    }

#define WRITEB(__value__) {uint8_t val = (uint8_t)__value__; fwrite(&val, 1, 1, pFic);}
#define WRITEW(__value__) {uint16_t val = (uint16_t)__value__; fwrite(&val, 1, 2, pFic);}

    // Data is loaded, now convert it
    FILE *pFic = nullptr;
    fopen_s(&pFic, "../assets/world.bin", "wb");
    WRITEB(startZoneId);
    WRITEW(playerStart.x);
    WRITEW(playerStart.y);
    int offset = 5 + (int)zones.size() * 2;
    for (auto &zone : zones)
    {
        WRITEW(offset);
        if (zone.dir == eDir::HORIZONTAL)
        {
            offset += (zone.size * ((13 + 4) * 16));
        }
        else
        {
            offset += (zone.size * (13 * (16 + 4)));
        }
    }
    for (auto &zone : zones)
    {
        WRITEB(zone.dir);
        WRITEB(zone.size);
        WRITEB(zone.x);
        WRITEB(zone.y);

        if (zone.dir == eDir::VERTICAL)
        {
            for (int j = zone.y * 13; j < zone.y * 13 + zone.size * 13; ++j)
            {
                for (int i = zone.x * 16; i < zone.x * 16 + 16; ++i)
                {
                    WRITEB(pTiles[j * w + i].id);
                }
                int i = zone.x * 16;
                uint8_t pal = pTiles[j * w + i + 0].pal << 6;
                pal |= pTiles[j * w + i + 1].pal << 4;
                pal |= pTiles[j * w + i + 2].pal << 2;
                pal |= pTiles[j * w + i + 3].pal;
                WRITEB(pal);
                pal = pTiles[j * w + i + 4].pal << 6;
                pal |= pTiles[j * w + i + 5].pal << 4;
                pal |= pTiles[j * w + i + 6].pal << 2;
                pal |= pTiles[j * w + i + 7].pal;
                WRITEB(pal);
                pal = pTiles[j * w + i + 8].pal << 6;
                pal |= pTiles[j * w + i + 9].pal << 4;
                pal |= pTiles[j * w + i + 10].pal << 2;
                pal |= pTiles[j * w + i + 11].pal;
                WRITEB(pal);
                pal = pTiles[j * w + i + 12].pal << 6;
                pal |= pTiles[j * w + i + 13].pal << 4;
                pal |= pTiles[j * w + i + 14].pal << 2;
                pal |= pTiles[j * w + i + 15].pal;
                WRITEB(pal);
            }
        }
        else if (zone.dir == eDir::HORIZONTAL)
        {
            for (int i = zone.x * 16; i < zone.x * 16 + zone.size * 16; ++i)
            {
                for (int j = zone.y * 13; j < zone.y * 13 + 13; ++j)
                {
                    WRITEB(pTiles[j * w + i].id);
                }

                if ((i % 2) == 0)
                {
                    int j = zone.y * 13;
                    uint8_t pal = pTiles[(j + 0) * w + (i + 0)].pal << 0;
                    pal |= pTiles[(j + 0) * w + (i + 1)].pal << 2;
                    pal |= pTiles[(j + 1) * w + (i + 0)].pal << 4;
                    pal |= pTiles[(j + 1) * w + (i + 1)].pal << 6;
                    WRITEB(pal);
                    pal = pTiles[(j + 2) * w + (i + 0)].pal << 0;
                    pal |= pTiles[(j + 2) * w + (i + 1)].pal << 2;
                    pal |= pTiles[(j + 3) * w + (i + 0)].pal << 4;
                    pal |= pTiles[(j + 3) * w + (i + 1)].pal << 6;
                    WRITEB(pal);
                    pal = pTiles[(j + 4) * w + (i + 0)].pal << 0;
                    pal |= pTiles[(j + 4) * w + (i + 1)].pal << 2;
                    pal |= pTiles[(j + 5) * w + (i + 0)].pal << 4;
                    pal |= pTiles[(j + 5) * w + (i + 1)].pal << 6;
                    WRITEB(pal);
                }
                else
                {
                    int j = zone.y * 13 + 6;
                    uint8_t pal = pTiles[(j + 0) * w + (i - 1)].pal << 0;
                    pal |= pTiles[(j + 0) * w + (i + 0)].pal << 2;
                    pal |= pTiles[(j + 1) * w + (i - 1)].pal << 4;
                    pal |= pTiles[(j + 1) * w + (i + 0)].pal << 6;
                    WRITEB(pal);
                    pal = pTiles[(j + 2) * w + (i - 1)].pal << 0;
                    pal |= pTiles[(j + 2) * w + (i + 0)].pal << 2;
                    pal |= pTiles[(j + 3) * w + (i - 1)].pal << 4;
                    pal |= pTiles[(j + 3) * w + (i + 0)].pal << 6;
                    WRITEB(pal);
                    pal = pTiles[(j + 4) * w + (i - 1)].pal << 0;
                    pal |= pTiles[(j + 4) * w + (i + 0)].pal << 2;
                    pal |= pTiles[(j + 5) * w + (i - 1)].pal << 4;
                    pal |= pTiles[(j + 5) * w + (i + 0)].pal << 6;
                    WRITEB(pal);
                }
            }
        }
    }
    fclose(pFic);

    // Now do the nametable, we pretty much just have to reorder it
    uint8_t nam[960];
    uint8_t namOut[960];
    fopen_s(&pFic, "../tiles.nam", "rb");
    fread(nam, 1, 960, pFic);
    fclose(pFic);
    for (int j = 0; j < 15; ++j)
    {
        for (int i = 0; i < 16; ++i)
        {
            namOut[(j * 16 + i) + 240 * 0] = nam[(j * 2 + 0) * 2 * 16 + i * 2 + 0];
            namOut[(j * 16 + i) + 240 * 1] = nam[(j * 2 + 0) * 2 * 16 + i * 2 + 1];
            namOut[(j * 16 + i) + 240 * 2] = nam[(j * 2 + 1) * 2 * 16 + i * 2 + 0];
            namOut[(j * 16 + i) + 240 * 3] = nam[(j * 2 + 1) * 2 * 16 + i * 2 + 1];
        }
    }
    fopen_s(&pFic, "../assets/tiles.bin", "wb");
    fwrite(namOut, 1, 960, pFic);
    fclose(pFic);
}
