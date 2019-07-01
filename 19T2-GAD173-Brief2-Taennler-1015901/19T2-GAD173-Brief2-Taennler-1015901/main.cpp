// 19T2-GAD173-Brief2-Taennler-1015901.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

/*
TILE TYPES
0 = Air
c = coin
e = enemy
g = ground
s = Player Spawn Position
t = Player Terminal/Exit point

*/

void CreateTileButtons();
void DrawGrid();
void DrawButtons();
void LoadTileTextures(int tileWidth, int tileHeight);

std::string texturePath = "C:\\Users\\lucat\\Documents\\gitlocal\\SAEGAD173\\19T2-GAD173-Brief2-Taennler-1015901\\19T2-GAD173-Brief2-Taennler-1015901\\x64\\Debug\\textures\\";

class TileType {
public:
	const char tileId;
	const std::string name;
	const std::string spriteLoc;
	TileType(char pTileId, std::string pName, std::string tileSprite) : tileId(pTileId), name(pName), spriteLoc(tileSprite){
		
	}

	TileType() : tileId(' '), name(""), spriteLoc("") {

	}
	/*
	std::string GetName() {
		return name;
	}

	char GetTileId() {
		return tileId;
	}

	std::string GetSpriteLoc() {
		return spriteLoc;
	}*/
};

std::map<char,TileType*> tileTypes{
	{'0',new TileType('0', "Air", texturePath + "tileAir.png")},
	{'c',new TileType('c', "Coin", texturePath+"tileCoin.png")},
	{'e',new TileType('e', "Enemy Spawn", texturePath + "tileEnemySpawn.png")},
	{'g',new TileType('g', "Ground", texturePath + "tileGround.png")},
	{'s',new TileType('s', "Player Spawn", texturePath + "tilePlayerExit.png")},
	{'t',new TileType('t', "Player Exit", texturePath + "tilePlayerSpawn.png")},
	{'w',new TileType('w', "Water", texturePath + "tileWater.png")}
};

std::map<char, sf::Texture*> tileTypeTextures;

class Tile {
	TileType * tileType;
	sf::Sprite sprite = sf::Sprite();
public:
	//sf::RectangleShape rect;
	bool active;
	sf::Vector2i size;
	Tile(int width, int height, TileType * pTileType, bool pActive) : active(pActive), tileType(pTileType){
		//rect.setSize(sf::Vector2f(width, height));
		size = sf::Vector2i(width,height);
		SetTileType(tileType);
	}

	Tile() {
		active = true;
		SetTileType(tileTypes['0']);
	}

	void SetPosition(float x, float y) {
		sprite.setPosition(x, y);
		//sf::Vector2i intPos(x, y);
		//sprite.setTextureRect(sf::IntRect(intPos, size));
	}

	void SetPosition(sf::Vector2f pos) {
		sprite.setPosition(pos);
		//sf::Vector2i intPos(pos.x, pos.y);
		//sprite.setTextureRect(sf::IntRect(intPos, size));
	}

	TileType * GetTileType() {
		return tileType;
	}

	sf::Sprite GetSprite() {
		return sprite;
	}

	void SetTileType(TileType * tileType) {
		this->tileType = tileType;

		if (this->tileType == nullptr) {
			return;
		}
		/*
		sf::Texture * texture = new sf::Texture();
		if ( !texture->loadFromFile(this->tileType->spriteLoc, sf::IntRect(0, 0, size.x, size.y)))
		{
			std::cout << "Couldn't load tile sprite! Tile ID: " + std::to_string(tileType->tileId) + " . " + this->tileType->spriteLoc + " ." << std::endl;
			if (sprite.getTexture()) {
				delete sprite.getTexture();
			}
		}
		else {
			std::cout << "Loaded texture " + std::to_string(tileType->tileId) + " . " + this->tileType->spriteLoc + " ." << std::endl;
			if (sprite.getTexture()) {
				delete sprite.getTexture();
			}
			
			sprite.setTexture(*texture);
		}*/
		if (tileTypeTextures[tileType->tileId] != nullptr)
			sprite.setTexture(*tileTypeTextures[tileType->tileId]);
		else {
			sf::Texture t;
			t.create(size.x, size.y);
			sprite.setTexture(t);
		}
	}

};

class Level {

	std::vector<std::vector<char>> charGrid;
	std::vector<std::vector<Tile*>> tileGrid;


	void UpdateTile(unsigned int x, unsigned int y) {
		tileGrid[y][x]->SetTileType(tileTypes[charGrid[y][x]]);
		tileGrid[y][x]->SetPosition(x*tileSize.x, y*tileSize.y);
	}

	void InitializeTileGrid() {
		for (int y = 0; y < charGrid.size(); y++) {
			for (int x = 0; x < charGrid[y].size(); x++) {
				tileGrid[y][x] = new Tile(tileSize.x, tileSize.y, tileTypes['0'], true);
				UpdateTile(x, y);
			}
		}
	}
public:
	
	const sf::Vector2i gridSize;
	sf::Vector2f tileSize = sf::Vector2f(50,50);
	sf::Vector2i playerSpawnPosition = sf::Vector2i(0, 0);

	Level(unsigned int gridSizeX, unsigned int gridSizeY, float tileWidth, float tileHeight) : gridSize(sf::Vector2i(gridSizeX, gridSizeY)),
		charGrid(gridSizeY, std::vector<char>(gridSizeX,'0')),
		tileGrid(gridSizeY, std::vector<Tile*>(gridSizeX, nullptr))
	{

		tileSize.x = tileWidth;
		tileSize.y = tileHeight;

		InitializeTileGrid();
	}

	Level(std::vector<std::vector<char>> pGrid, float tileWidth, float tileHeight) : charGrid(pGrid),
		tileGrid(std::size(pGrid), std::vector<Tile*>(std::size(pGrid[0]),nullptr)),
		gridSize(sf::Vector2i(std::size(pGrid[0]), std::size(pGrid))) {
		
		tileSize.x = tileWidth;
		tileSize.y = tileHeight;

		InitializeTileGrid();
	}

	Tile* GetTileAtCoord(unsigned int x, unsigned int y) {
		if (x >= gridSize.x || y >= gridSize.y)
			return nullptr;
		
		return tileGrid[y][x];
	}

	void SetTileTypeAtCoord(unsigned int x, unsigned int y, TileType * pTileType) {
		if (x >= gridSize.x || y >= gridSize.y)
			return;
		UpdateTile(x,y);
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
};

class Button {
	bool isMouseOver = false;
	bool isMousePressed = false;
	bool isButtonDown = false;

	std::vector<std::function<void()>> btnDownFuncs;
	std::vector<std::function<void()>> btnPressedFuncs;
	std::vector<std::function<void()>> btnReleasedFuncs;
	std::vector<std::function<void()>> mouseEnterFuncs;
	std::vector<std::function<void()>> mouseExitFuncs;

	void SetButtonDown() {
		if (isButtonDown)
			return;

		for (std::function<void()> func : btnDownFuncs) {
			func();
		}
	}
public:

	sf::Shape * shape = nullptr;
	sf::Sprite sprite;

#pragma region Button Event Handling

	void SetButtonPressed() {
		if (!isButtonDown) {
			SetButtonDown();
			isButtonDown = true;
		}

		for (std::function<void()> func : btnPressedFuncs) {
			func();
		}
	}

	void SetButtonReleased() {
		if (!isButtonDown)
			return;

		isButtonDown = false;

		for (std::function<void()> func : btnReleasedFuncs) {
			func();
		}
	}

	void SetMouseEnter() {
		if (isMouseOver)
			return;

		isMouseOver = true;

		for (std::function<void()> func : mouseEnterFuncs) {
			func();
		}
	}

	void SetMouseExit() {
		if (!isMouseOver)
			return;

		isMouseOver = false;

		for (std::function<void()> func : mouseExitFuncs) {
			func();
		}
	}

	// Add Functions
	void AddButtonDownFunc(std::function<void()> func) {
		btnDownFuncs.push_back(func);
	}

	void AddButtonPressedFunc(std::function<void()> func) {
		btnPressedFuncs.push_back(func);
	}

	void AddButtonReleasedFunc(std::function<void()> func) {
		btnReleasedFuncs.push_back(func);
	}

	void AddMouseEnterFunc(std::function<void()> func) {
		mouseEnterFuncs.push_back(func);
	}

	void AddMouseExitFunc(std::function<void()> func) {
		mouseExitFuncs.push_back(func);
	}
	/*
	// Remove Functions
	void RemoveButtonDownFunc(std::function<void()> * func) {
		for (int i = 0; i < btnDownFuncs.size; i++) {
			if (&btnDownFuncs[i] == func) {
				btnDownFuncs.erase(btnDownFuncs.begin()+i);
				break;
			}
		}
	}

	void RemoveButtonPressedFunc(std::function<void()> * func) {
		for (int i = 0; i < btnPressedFuncs.size; i++) {
			if (&btnPressedFuncs[i] == func) {
				btnPressedFuncs.erase(btnPressedFuncs.begin() + i);
				break;
			}
		}
	}

	void RemoveButtonReleasedFunc(std::function<void()> * func) {
		for (int i = 0; i < btnReleasedFuncs.size; i++) {
			if (&btnReleasedFuncs[i] == func) {
				btnReleasedFuncs.erase(btnReleasedFuncs.begin() + i);
				break;
			}
		}
	}

	void RemoveMouseEnterFunc(std::function<void()> * func) {
		for (int i = 0; i < mouseEnterFuncs.size; i++) {
			if (&mouseEnterFuncs[i] == func) {
				mouseEnterFuncs.erase(mouseEnterFuncs.begin() + i);
				break;
			}
		}
	}

	void RemoveMouseExitFunc(std::function<void()> * func) {
		for (int i = 0; i < mouseExitFuncs.size; i++) {
			if (&mouseExitFuncs[i] == func) {
				mouseExitFuncs.erase(mouseExitFuncs.begin() + i);
				break;
			}
		}
	}*/
#pragma endregion

	
	void SetShape(sf::Shape * shape) {
		this->shape = shape;
	}

	void SetPosition(float x, float y) {
		shape->setPosition(x,y);
	}

	void ScaleSize(float scaleX, float scaleY) {
		shape->setScale(scaleX, scaleY);
	}

	sf::FloatRect GetGlobalBounds() {
		return shape->getGlobalBounds();
	}
};

std::vector<Button*> buttons;


std::vector<std::vector<Button*>> buttonGrid;
Level * activeLevel;
//std::vector<std::vector<Tile>> currentGrid;

void WindowLifeCycle();
sf::RenderWindow renderWindow;

int main()
{
	WindowLifeCycle();
}

sf::Vector2f mousePos;
float deltaTime = 0;
void WindowLifeCycle() {
	

	sf::Clock Clock;
	int offset = 0;


	LoadTileTextures(50,50);
	activeLevel = new Level(5, 5, 50, 50);
	CreateTileButtons();
	
	int windowWidth = activeLevel->gridSize.x * activeLevel->tileSize.x;
	int windowHeight = activeLevel->gridSize.y * activeLevel->tileSize.y;
	sf::RectangleShape windowBg(sf::Vector2f(windowWidth, windowHeight));
	windowBg.setFillColor(sf::Color::Color(153,217,234,255));
	renderWindow.create(sf::VideoMode(windowWidth, windowHeight), "Level Editor - GAD173 - Brief 2");
	while (renderWindow.isOpen())
	{
		deltaTime = Clock.restart().asSeconds();
		sf::Event event;
		while (renderWindow.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed: {
				renderWindow.close();
				break;
			}
			case sf::Event::MouseMoved: {
				sf::Vector2i mousePosInt = sf::Mouse::getPosition(renderWindow);
				mousePos.x = (float)mousePosInt.x;
				mousePos.y = (float)mousePosInt.y;

				for (Button* b : buttons) {
					if (b->GetGlobalBounds().contains(mousePos)) {
						b->SetMouseEnter();
					}
					else {
						b->SetMouseExit();
					}
				}
				break;
			}
			case sf::Event::MouseButtonPressed: {
				for (Button* b : buttons) {
					if (b->GetGlobalBounds().contains(mousePos)) {
						b->SetButtonPressed();
					}
				}
				break;
			}
			case sf::Event::MouseButtonReleased: {
				for (Button* b : buttons) {
					b->SetButtonReleased();
				}
				break;
			}
				
			}

		}

		renderWindow.clear();
		renderWindow.draw(windowBg);
		DrawGrid();

		renderWindow.display();
	}
}

void LoadTileTextures(int tileWidth, int tileHeight) {
	//tileTypeTextures = std::map<char, sf::Texture>(tileTypes.size());
	for (auto itr = tileTypes.begin(); itr != tileTypes.end(); ++itr) {
		sf::Texture * texture = new sf::Texture();
		if (!texture->loadFromFile(itr->second->spriteLoc, sf::IntRect(0, 0, tileWidth, tileHeight)))
		{
		}
		else {
			tileTypeTextures.insert({ itr->first,texture });
		}
	}
}

void CreateTileButtons() {
	buttonGrid.resize(activeLevel->gridSize.y, std::vector<Button*>(activeLevel->gridSize.x,nullptr));
	for (int y = 0; y < activeLevel->gridSize.y; y++) {
		for (int x = 0; x < activeLevel->gridSize.x; x++) {
			Button* b = new Button();

			sf::RectangleShape * rect = new sf::RectangleShape(sf::Vector2f(activeLevel->tileSize));
			rect->setFillColor(sf::Color::Blue);
			b->SetShape(rect);
			b->SetPosition(x * activeLevel->tileSize.x, y * activeLevel->tileSize.y);
			auto clickFunction = [x,y]() {

				if (activeLevel->GetTileAtCoord(x, y)->GetTileType() == nullptr)
					return;

				std::map<char, TileType*>::iterator it = tileTypes.find(activeLevel->GetTileAtCoord(x, y)->GetTileType()->tileId);
				it++;
				if (it != tileTypes.end()) {
					activeLevel->GetTileAtCoord(x, y)->SetTileType(it->second);
				}
				else {
					activeLevel->GetTileAtCoord(x, y)->SetTileType(tileTypes['0']);
				}
			};
			b->AddButtonDownFunc(clickFunction);
			

			buttonGrid[y][x] = b;
			buttons.push_back(b);
		}
	}
}

void DrawGrid() {
	for (int y = 0; y < activeLevel->gridSize.y; y++) {
		for (int x = 0; x < activeLevel->gridSize.x; x++) {
			Tile* tile = activeLevel->GetTileAtCoord(x, y);

			if (tile->active) {
				if (&tile->GetSprite()) {
					//std::cout << tile->GetSprite().getTexture()->getSize().x << std::endl;
					renderWindow.draw(tile->GetSprite());
				}
					
				
			}
				
		}
	}
}

void DrawButtons() {
	for (Button* b : buttons) {
		if(b->shape != nullptr)
			renderWindow.draw(*b->shape);
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
