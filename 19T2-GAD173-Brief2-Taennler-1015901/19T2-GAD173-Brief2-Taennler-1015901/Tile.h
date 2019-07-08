#pragma once
#ifndef SFML/Graphics.hpp
#include <SFML/Graphics.hpp>
#endif // !SFML/Graphics.hpp
#ifndef TileType.h
#include "TileType.h"
#endif // !TileType.h

class Tile {
	TileType * tileType;
	sf::Sprite sprite = sf::Sprite();
public:
	//sf::RectangleShape rect;
	bool active;
	sf::Vector2i size;
	Tile(int width, int height, TileType * pTileType, bool pActive);

	Tile();

	~Tile();

	void SetPosition(float x, float y);

	void SetPosition(sf::Vector2f pos);

	TileType * GetTileType();

	sf::Sprite GetSprite();

	void SetTileType(TileType * tileType);

};

