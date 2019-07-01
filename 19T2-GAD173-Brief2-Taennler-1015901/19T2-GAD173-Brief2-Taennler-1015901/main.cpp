// 19T2-GAD173-Brief2-Taennler-1015901.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>


class Tile {
public:
	sf::RectangleShape rect;
	char tileType;
	bool active;
	Tile(char pBrickType, bool pActive) : active(pActive), brickType(pBrickType){
	
	}

};

class Level {
	std::vector<std::vector<char>> grid;


public:

	const sf::Vector2i gridSize;
	sf::Vector2f tileSize = sf::Vector2f(50,50);

	Level(int gridSizeX, int gridSizeY, float tileWidth, float tileHeight) : gridSize(sf::Vector2i(gridSizeX, gridSizeY)){
		tileSize.x = tileWidth;
		tileSize.y = tileHeight;

		grid.resize(gridSizeY);
		for (int i = 0; i < gridSizeY; i++) {
			grid[i].resize(gridSizeX);
		}
	}

	Level(std::vector<std::vector<char>> pGrid, float tileWidth, float tileHeight) : grid(pGrid) {
		tileSize.x = tileWidth;
		tileSize.y = tileHeight;
	}

	char GetValueAtCoord(int x, int y) {
		if (x >= gridSize.x || y >= gridSize.y)
			return 0;

		return grid[y][x];
	}




};

Level * activeLevel;
std::vector<std::vector<Tile>> currentGrid;

void WindowLifeCycle();
sf::RenderWindow renderWindow;

int main()
{
	WindowLifeCycle();
}

float deltaTime = 0;
void WindowLifeCycle() {
	renderWindow.create(sf::VideoMode(800, 540), "Level Editor - GAD173 - Brief 2");
	

	sf::Clock Clock;
	int offset = 0;

	activeLevel = new Level(5, 5, 50, 50);

	while (renderWindow.isOpen())
	{
		deltaTime = Clock.restart().asSeconds();
		sf::Event event;
		while (renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				renderWindow.close();
		}


		renderWindow.clear();

		DrawGrid();

		renderWindow.display();
	}
}

void InitializeGrid() {
	// Initialize currentGrid vec
	currentGrid.resize(activeLevel->gridSize.y);
	for (int i = 0; i < activeLevel->gridSize.y; i++) {
		currentGrid[i].resize(activeLevel->gridSize.x);
	}

	// Initialize Tiles
	for (int y = 0; y < activeLevel->gridSize.y; y++) {
		for (int x = 0; x < activeLevel->gridSize.x; x++) {
			currentGrid[y][x] = Tile()
		}
	}
}

void DrawGrid() {
	for (int y = 0; y < activeLevel->gridSize.y; y++) {
		for (int x = 0; x < activeLevel->gridSize.x; x++) {
			renderWindow.draw(activeLevel->GetValueAtCoord(y,x));
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
