#pragma once
#ifndef string
#include <string>
#endif // !string
#ifndef map
#include <map>
#endif // !map
#ifndef SFML_GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#endif // !SFML/Graphics.hpp
#ifndef TileType
#include "TileType.h"
#endif // !TileType.h


class TileType {
public:
	static std::map<char, TileType*> tileTypes;
	static std::map<char, sf::Texture*> tileTypeTextures;
	static const char defaultTileTypeChar = '0';

	static bool IsValidTileTypeChar(char c);

	const char tileId;
	const std::string name;
	const std::string spriteLoc;

	TileType(char pTileId, std::string pName, std::string tileSprite);
	TileType();
	~TileType();
};
