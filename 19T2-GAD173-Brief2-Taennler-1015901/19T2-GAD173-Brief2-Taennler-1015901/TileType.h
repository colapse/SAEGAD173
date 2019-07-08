#pragma once
#ifndef string
#include <string>
#endif // !string
#ifndef map
#include <map>
#endif // !map
#ifndef SFML/Graphics.hpp
#include <SFML/Graphics.hpp>
#endif // !SFML/Graphics.hpp
#ifndef TileType.h
#include "TileType.h"
#endif // !TileType.h


class TileType {
public:
	static std::map<char, TileType*> tileTypes;
	static std::map<char, sf::Texture*> tileTypeTextures;

	const char tileId;
	const std::string name;
	const std::string spriteLoc;

	TileType(char pTileId, std::string pName, std::string tileSprite);
	TileType();
	~TileType();
};
