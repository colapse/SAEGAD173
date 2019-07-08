#pragma once
#ifndef SFML/Graphics.hpp
#include <SFML/Graphics.hpp>
#endif // !SFML/Graphics.hpp
#ifndef Tile.h
#include "Tile.h";
#endif // !Tile.h


class Level {

	std::vector<std::vector<char>> charGrid;
	std::vector<std::vector<Tile*>> tileGrid;


	void UpdateTile(unsigned int x, unsigned int y);

	void InitializeTileGrid();
public:

	const sf::Vector2i gridSize;
	sf::Vector2f tileSize;
	sf::Vector2i playerSpawnPosition;

	Level(unsigned int gridSizeX, unsigned int gridSizeY, float tileWidth, float tileHeight);

	Level(std::vector<std::vector<char>> pGrid, float tileWidth, float tileHeight);


	~Level();
	Level();

	Tile* GetTileAtCoord(unsigned int x, unsigned int y);

	void SetTileTypeAtCoord(unsigned int x, unsigned int y, TileType * pTileType);
};

