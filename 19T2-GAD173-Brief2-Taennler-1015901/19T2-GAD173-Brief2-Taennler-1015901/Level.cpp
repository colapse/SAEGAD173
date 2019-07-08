#include "pch.h"
#include "Level.h"


Level::Level()
{
	tileSize = sf::Vector2f(50, 50);
	playerSpawnPosition = sf::Vector2i(0, 0);
}

Level::Level(unsigned int gridSizeX, unsigned int gridSizeY, float tileWidth, float tileHeight) : gridSize(sf::Vector2i(gridSizeX, gridSizeY)),
charGrid(gridSizeY, std::vector<char>(gridSizeX, '0')),
tileGrid(gridSizeY, std::vector<Tile*>(gridSizeX, nullptr))
{

	tileSize.x = tileWidth;
	tileSize.y = tileHeight;
	playerSpawnPosition = sf::Vector2i(0, 0);

	InitializeTileGrid();
}

Level::Level(std::vector<std::vector<char>> pGrid, float tileWidth, float tileHeight) : charGrid(pGrid),
tileGrid(std::size(pGrid), std::vector<Tile*>(std::size(pGrid[0]), nullptr)),
gridSize(sf::Vector2i(std::size(pGrid[0]), std::size(pGrid))) {

	tileSize.x = tileWidth;
	tileSize.y = tileHeight;
	playerSpawnPosition = sf::Vector2i(0, 0);

	InitializeTileGrid();
}


Level::~Level()
{
	if (tileGrid.size() > 0) {
		for (std::vector<Tile*> tArr : tileGrid) {
			for (Tile* t : tArr) {
				delete t;
			}
		}
	}
}

void Level::UpdateTile(unsigned int x, unsigned int y) {
	tileGrid[y][x]->SetTileType(TileType::tileTypes[charGrid[y][x]]);
	tileGrid[y][x]->SetPosition(x*tileSize.x, y*tileSize.y);
}

void Level::InitializeTileGrid() {
	for (int y = 0; y < charGrid.size(); y++) {
		for (int x = 0; x < charGrid[y].size(); x++) {
			tileGrid[y][x] = new Tile(tileSize.x, tileSize.y, TileType::tileTypes['0'], true);
			UpdateTile(x, y);
		}
	}
}

Tile* Level::GetTileAtCoord(unsigned int x, unsigned int y) {
	if (x >= gridSize.x || y >= gridSize.y)
		return nullptr;

	return tileGrid[y][x];
}

void Level::SetTileTypeAtCoord(unsigned int x, unsigned int y, TileType * pTileType) {
	if (x >= gridSize.x || y >= gridSize.y)
		return;
	UpdateTile(x, y);
	//tileGrid[y][x]->SetTileType(pTileType);
}
/*
char GetCharAtCoord(int x, int y) {
	if (x >= gridSize.x || y >= gridSize.y)
		return 0;

	return charGrid[y][x];
}*/
/*
void SetTileTypeAtCoord(int x, int y, char tileType) {
	if (x >= gridSize.x || y >= gridSize.y)
		return;

	charGrid[y][x] = tileType;
}*/
