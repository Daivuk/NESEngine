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
    HORIZONTAL,
    VERTICAL
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
                for (auto &zone : zones)
                {
                    if (zone.dir == eDir::HORIZONTAL)
                    {
                        if (entity.x >= zone.x * 256 &&
                            entity.x < (zone.x + zone.size) * 256 &&
                            entity.y >= zone.y * 208 &&
                            entity.y < zone.y * 208 + 208)
                        {
                            zone.entities.push_back(entity);
                        }
                    }
                    else if (zone.dir == eDir::VERTICAL)
                    {
                        if (entity.x >= zone.x * 256 &&
                            entity.x < zone.x * 256 + 256 &&
                            entity.y >= zone.y * 208 &&
                            entity.y < (zone.y + zone.size) * 208)
                        {
                            zone.entities.push_back(entity);
                        }
                    }
                }
            }
        }
    }

    // Data is loaded, now convert it
    int tmp;
    tmp = 5;
}
