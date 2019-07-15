#pragma once
#ifndef SFML_GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#endif // !SFML/Graphics.hpp
#ifndef TileType
#include "TileType.h"
#endif // !TileType.h
#ifndef GameObject
#include "GameObject.h"
#endif // !GameObject.h
#ifndef GameObjectType
#include "GameObjectType.h"
#endif // !GameObjectType.h

class Tile {
	TileType * tileType;
	sf::Sprite * sprite = new sf::Sprite();
	sf::Sprite * gameObjectSprite = new sf::Sprite();

	GameObjectType * gameObjectType = nullptr;
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
	GameObjectType * GetGameObjectType();

	sf::Sprite * GetSprite();
	sf::Sprite * GetGameObjectSprite();

	void SetTileType(TileType * tileType);

	void SetGameObjectType(GameObjectType * gameObjectType);

};

