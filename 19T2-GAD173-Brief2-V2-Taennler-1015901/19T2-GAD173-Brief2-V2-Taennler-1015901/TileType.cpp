#include "pch.h"
#include "TileType.h"


std::map<char, TileType*> TileType::tileTypes;
std::map<char, sf::Texture*> TileType::tileTypeTextures;

bool TileType::IsValidTileTypeChar(char c) {
	std::map<char, TileType*>::iterator it = tileTypes.find(c);
	if (it != tileTypes.end())
	{
		return true;
	}
	return false;
}

TileType::TileType(char pTileId, std::string pName, std::string tileSprite) : tileId(pTileId), name(pName), spriteLoc(tileSprite) {
	
}

TileType::TileType(char pTileId, std::string pName, std::string tileSprite, bool pAllowGameObjects) : tileId(pTileId), name(pName), spriteLoc(tileSprite), allowGameObjects(pAllowGameObjects) {
	
}

TileType::TileType() : tileId(' '), name(""), spriteLoc("") {

}


TileType::~TileType()
{
}
