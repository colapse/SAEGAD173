#pragma once
#ifndef SFML_GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#endif // !SFML/Graphics.hpp
#ifndef Tile
#include "Tile.h"
#endif // !Tile.h


class Level {

	std::vector<std::vector<char>> charGrid;
	std::vector<std::vector<Tile*>> tileGrid;


	void UpdateTile(unsigned int x, unsigned int y);

	void InitializeTileGrid();
public:
	static const float defaultTileSize;
	const sf::Vector2i gridSize;
	float tileSize;
	sf::Vector2i playerSpawnPosition;
	std::string levelName;
	std::string existingFile;

	Level(unsigned int gridSizeX, unsigned int gridSizeY, float pTileSize);
	Level(unsigned int gridSizeX, unsigned int gridSizeY, float pTileSize, std::string pLevelName);

	Level(std::vector<std::vector<char>> pGrid, float pTileSize);
	Level(std::vector<std::vector<char>> pGrid, float pTileSize, std::string pLevelName);


	~Level();
	Level();

	Tile* GetTileAtCoord(unsigned int x, unsigned int y);

	void SetTileTypeAtCoord(unsigned int x, unsigned int y, TileType * pTileType);
};

