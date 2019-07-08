#include "pch.h"
#include "TileType.h"

std::map<char, TileType*> TileType::tileTypes;
std::map<char, sf::Texture*> TileType::tileTypeTextures;

TileType::TileType(char pTileId, std::string pName, std::string tileSprite) : tileId(pTileId), name(pName), spriteLoc(tileSprite) {
	
}

TileType::TileType() : tileId(' '), name(""), spriteLoc("") {

}


TileType::~TileType()
{
}
