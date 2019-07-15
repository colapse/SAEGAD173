// 19T2-GAD173-Brief2-Taennler-1015901.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "TextInput.h"
#include "Button.h"
#include "GameObject.h"
#include "GameObjectType.h"
#include "TileType.h"
#include "Tile.h"
#include "Level.h"
#include "View.h"
#include <algorithm>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <sstream>

/*
TILE TYPES
0 = Air
c = coin
e = enemy
g = ground
s = Player Spawn Position
t = Player Terminal/Exit point

*/


#pragma region Global Function Declarations
enum class ViewName;
void WindowLifeCycle();
void InitMainMenuView();
void CreateLevelEditorTileButtons();
//void DrawGrid();
//void DrawButtons();
void LoadTileTextures(int tileWidth, int tileHeight);
void InitVars();
void DrawActiveView(ViewName activeView);
void HandleViewEvents(ViewName viewName, sf::Event e);
void LoadLevelsFromDirectory();
Level* LoadLevelFromFile(std::string file);
std::vector<std::string> SplitStringByDeli(std::string stringToSplit, char delimeter);
void InitLevelEditorView();
void LevelEditorLoadLevel(Level * pLevel);
void InitLevelEditorSideBarView();
#pragma endregion

#pragma region Global Variables

enum class ViewName
{
	MainMenu,
	LevelEditor,
	LevelEditorSideBar
};
std::map<ViewName, View*> views;

sf::RenderWindow renderWindow;
sf::Font generalFont;
float windowWidth = 1000; // Width of the window
float windowHeight = 600; // Height of the window

std::string exeDir = "Z:\\Documents\\gitlocal\\SAEGAD173\\19T2-GAD173-Brief2-V2-Taennler-1015901\\19T2-GAD173-Brief2-V2-Taennler-1015901\\x64\\Debug\\";//"C:\\Users\\lucat\\Documents\\gitlocal\\SAEGAD173\\19T2-GAD173-Brief2-Taennler-1015901\\19T2-GAD173-Brief2-Taennler-1015901\\x64\\Debug\\"; // Directory of the exe file (Not a nice solution)
std::string fontName = "Radiant.ttf"; // Font retrieved from https://www.dafont.com/
std::string levelFolder = "levels";
std::string levelExt = ".lvl";

std::string texturePath = "Z:\\Documents\\gitlocal\\SAEGAD173\\19T2-GAD173-Brief2-V2-Taennler-1015901\\19T2-GAD173-Brief2-V2-Taennler-1015901\\x64\\Debug\\textures\\";//"C:\\Users\\lucat\\Documents\\gitlocal\\SAEGAD173\\19T2-GAD173-Brief2-Taennler-1015901\\19T2-GAD173-Brief2-Taennler-1015901\\x64\\Debug\\textures\\"; //C:\\Users\\lucat\\Documents\\gitlocal\\SAEGAD173\\19T2-GAD173-Brief2-Taennler-1015901\\19T2-GAD173-Brief2-Taennler-1015901\\x64\\Debug\\

std::vector<ViewName> activeViews;

std::vector<std::string> levelsInFolder; //levels in level folder


Level * activeLevel;

bool enableZoom = true;
float currentZoom = 1;
float mouseWheelScrollX = 0;
sf::Vector2f mousePos;
sf::Vector2i mousePosInt;
float deltaTime = 0;

#pragma endregion

bool leftMouseButtonPressed = false;
bool middleMouseButtonPressed = false;
int main()
{
	TileType::tileTypes = {
	{'0',new TileType('0', "Air", texturePath + "BlockSky.png", true, true)},
	{'1',new TileType('1', "Ground", texturePath + "BlockPlatform.png", true, false)},
	{'2',new TileType('2', "Lava", texturePath + "LavaAnimated.gif", true, false)},
	{'c',new TileType('c', "Coin", texturePath + "CoinAnimated.gif", false, true)},
	{'e',new TileType('e', "Enemy Spawn", texturePath + "EnemyAlive.png", false, true)},
	{'s',new TileType('s', "Player Spawn", texturePath + "Door.png", false, false)},
	{'t',new TileType('t', "Player Exit", texturePath + "Player.png")}
	};

	LoadLevelsFromDirectory();
	LoadTileTextures(32, 32);
	InitVars();
	InitMainMenuView();
	InitLevelEditorView();
	activeViews.push_back(ViewName::MainMenu);

	WindowLifeCycle();
}


#pragma region Load/Unload
Level* LoadLevelFromFile(std::string file) {
	Level * level = nullptr;
	std::ifstream myfile(file);
	std::vector<std::vector<char>> charGrid;
	std::string line;
	if (myfile.is_open()) {
		
		int yOffset = -1;
		int xSize = 0;
		int ySize = 0;
		while (std::getline(myfile, line)) {
			if (line.length() == 0)
				continue;
			std::stringstream ss{ line };
			std::string entry;

			if (yOffset == -1) {
				int delimCounter = 0;
				while (std::getline(ss, entry, ':')) {
					if (delimCounter == 0)
						xSize = static_cast<int>(std::stoi(entry));
					else if (delimCounter == 1)
						ySize = static_cast<int>(std::stoi(entry));

					delimCounter++;
				}

				yOffset = 0;

				charGrid = std::vector<std::vector<char>>(ySize, std::vector<char>(xSize, TileType::defaultTileTypeChar));
			}
			else {
				if (yOffset >= ySize)
					break;
				//grid.push_back(std::vector<Brick*>());
				int xOffset = 0;
				std::vector<std::string> parts = SplitStringByDeli(line,',');
				for (std::string part : parts) {
					if (xOffset >= xSize)
						break;

					char tileType;
					// Read TileType
					if (part.length() > 0)
						tileType = part[0];
					else
						tileType = TileType::defaultTileTypeChar;

					if (!TileType::IsValidTileTypeChar(tileType)) {
						tileType = TileType::defaultTileTypeChar;
					}

					// Read settings (If there are any)
					std::map<std::string, std::string> tileSettings;
					unsigned first = (unsigned)part.find_first_of('[');
					unsigned last = (unsigned)part.find_last_of(']');
					if (first != std::string::npos && last != std::string::npos && first < part.size() && last < part.size()) {
						std::string values = part.substr(first, last - first);
						if (values.length() > 0) {
							std::vector<std::string> settings = SplitStringByDeli(values, ',');
							if (settings.size() > 0) {
								for (std::string setting : settings) {
									std::vector<std::string> settingParts = SplitStringByDeli(setting, '=');

									if (settingParts.size() >= 1) {
										tileSettings.insert({ settingParts[0] , ((settingParts.size() >= 2) ? settingParts[1] : "")});
									}
								}
							}
						}
					}

					charGrid[yOffset][xOffset] = tileType;
					//TODO Assign settings
					xOffset++;
				}

				yOffset++;
			}
		}

		if(yOffset > 1)
		level = new Level(charGrid, Level::defaultTileSize);
		level->existingFile = file;
	}
	myfile.close();

	return level;
}

bool SaveLevel(Level * level) {
	std::string filename = level->existingFile;
	bool createdNewFile = false;
	
	if (filename.length() == 0 || !std::filesystem::exists(filename)) {
		if (level->levelName.length() > 0) {
			filename = level->levelName;
			filename.erase(std::remove_if(filename.begin(), filename.end(), isspace), filename.end());
			filename = exeDir + levelFolder + "\\" + filename + levelExt;
		}
		else {
			filename = "level";
			while (std::filesystem::exists(filename)) {
				filename += std::to_string(std::rand()%9+1);
			}
			filename = exeDir + levelFolder + "\\" + filename + levelExt;
		}

		createdNewFile = true;
	}

	std::ofstream myfile(filename);
	if (myfile.is_open()) {
		for (int y = 0; y < level->gridSize.y; y++) {
			if (y == 0) {
				myfile << std::to_string(level->gridSize.y) + ":" + std::to_string(level->gridSize.x) + "\n";
			}
			for (int x = 0; x < level->gridSize.x; x++) {
				if (level->GetTileAtCoord(x,y) == nullptr || level->GetTileAtCoord(x, y)->GetTileType() == nullptr) {
					myfile << TileType::defaultTileTypeChar;
				}
				else {
					myfile << level->GetTileAtCoord(x, y)->GetTileType()->tileId;
				}
				if (x < level->gridSize.x - 1)
					myfile << ",";
			}
			myfile << "\n";
		}
		myfile.close();

		if (createdNewFile) {
			level->existingFile = filename;
			LoadLevelsFromDirectory();// TODO Need to update fastaccess levels in main view!
		}

		return true;
	}
	else {
		return false;
	}
}

std::vector<std::string> SplitStringByDeli(std::string stringToSplit, char delimeter) {
	std::vector<std::string> parts;

	std::stringstream ss{ stringToSplit };
	std::string entry;

	while (std::getline(ss, entry, delimeter)) {
		parts.push_back(entry);
	}

	return parts;
}

// original from https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
void LoadLevelsFromDirectory() {
	levelsInFolder.clear();
	for (const auto & entry : std::filesystem::directory_iterator(exeDir + levelFolder)) {
		if (entry.is_regular_file() && entry.path().extension() == levelExt) {
			levelsInFolder.push_back(entry.path().stem().u8string());
		}
	}
}

void LoadTileTextures(int tileWidth, int tileHeight) {
	//tileTypeTextures = std::map<char, sf::Texture>(tileTypes.size());
	for (auto itr = TileType::tileTypes.begin(); itr != TileType::tileTypes.end(); ++itr) {
		sf::Texture * texture = new sf::Texture();
		if (!texture->loadFromFile(itr->second->spriteLoc, sf::IntRect(0, 0, tileWidth, tileHeight)))
		{
		}
		else {
			TileType::tileTypeTextures.insert({ itr->first,texture });
		}
	}
}

void LevelEditorLoadLevel(Level * pLevel) {
	activeLevel = pLevel;

	if (pLevel == nullptr)
		return;

	InitLevelEditorSideBarView();

	for (int y = 0; y < pLevel->gridSize.y; y++) {
		for (int x = 0; x < pLevel->gridSize.x; x++) {
			views[ViewName::LevelEditor]->AddSprite(pLevel->GetTileAtCoord(x, y)->GetSprite());
		}
	}

	CreateLevelEditorTileButtons();

	//sf::RectangleShape * viewBg = new sf::RectangleShape(sf::Vector2f(views[ViewName::LevelEditor]->GetSFView()->getSize().x, views[ViewName::LevelEditor]->GetSFView()->getSize().y));
	sf::RectangleShape * viewBg = new sf::RectangleShape(sf::Vector2f(pLevel->gridSize.x * pLevel->tileSize, pLevel->gridSize.y * pLevel->tileSize));
	viewBg->setFillColor(sf::Color::Color(154, 217, 234, 255));
	views[ViewName::LevelEditor]->AddShape(viewBg, true);
}

void LevelEditorUnloadLevel() {
	delete views[ViewName::LevelEditor];
	views[ViewName::LevelEditor] = nullptr;
	delete activeLevel;
	activeLevel = nullptr;

	InitLevelEditorView();
}

#pragma endregion

#pragma region Initialize

TileType* activeBrush;
Button* activeBrushButtonPtr;
std::vector<Button*> brushButtons;

void InitLevelEditorSideBarView() {
	View * editorBarView = new View("Level Editor Side Bar", (float)renderWindow.getSize().x, (float)renderWindow.getSize().y);
	float yOffset = 10;
	sf::View * sfView = new sf::View(sf::FloatRect(0, 0, renderWindow.getSize().x*0.15f, (float)renderWindow.getSize().y)); //renderWindow.getPosition().x + renderWindow.getSize().x/2, renderWindow.getPosition().y + renderWindow.getSize().y / 2
	sfView->setViewport(sf::FloatRect(0, 0, 0.15f, 1));
	editorBarView->SetSfView(sfView);

	// Background
	sf::RectangleShape * viewBg = new sf::RectangleShape(sf::Vector2f(sfView->getSize().x, sfView->getSize().y));
	viewBg->setFillColor(sf::Color::Color(99, 99, 99, 255));
	editorBarView->AddShape(viewBg, true);


	// Txt level name
	sf::Text * txtLevelName = new sf::Text("Level Name:", generalFont, 18);
	sf::Vector2f txtNewLevelNamePos(sfView->getSize().x / 2 - txtLevelName->getGlobalBounds().width / 2, yOffset);
	txtLevelName->setPosition(txtNewLevelNamePos);
	yOffset += txtLevelName->getGlobalBounds().height + 10;
	editorBarView->AddText(txtLevelName);

	// Txt Field level name
	TextInput * tiLevelName = new TextInput();
	tiLevelName->SetFont(generalFont);
	tiLevelName->SetSize(sfView->getSize().x - 20, 20);
	tiLevelName->allowOnlyAlphaNumerical = true;
	tiLevelName->maxLength = 30;
	sf::Vector2f tiLevelNamePos(sfView->getSize().x / 2 - tiLevelName->GetBoundaries().width/2, yOffset);
	tiLevelName->SetPosition(tiLevelNamePos);
	tiLevelName->SetTextColor(sf::Color::Black);
	//// Create mouse enter lambda func which changes the Outline color
	auto FnMOver_TILevelName = [tiLevelName]() {
		if (!tiLevelName->HasFocus())
			tiLevelName->SetBorder(2, sf::Color::Black);
	};
	tiLevelName->AddOnMouseEnterFunc(FnMOver_TILevelName); // Assign func to input
	//// Create mouse exit lambda func which changes the Outline color
	auto FnMExit_TILevelName = [tiLevelName]() {
		if (!tiLevelName->HasFocus())
			tiLevelName->SetBorder(2, sf::Color::Color(99, 99, 99, 255));
	};
	tiLevelName->AddOnMouseExitFunc(FnMExit_TILevelName); // Assign func to input
	//// Create focus lambda func which changes the Outline color
	auto FnMFocus_TILevelName = [tiLevelName]() {
		tiLevelName->SetBorder(2, sf::Color::Cyan);
	};
	tiLevelName->AddOnFocusFunc(FnMFocus_TILevelName); // Assign func to input
	//// Create focus exit lambda func which changes the Outline color
	auto FnMFocusExit_TILevelName = [tiLevelName]() {
		tiLevelName->SetBorder(2, sf::Color::Color(99, 99, 99, 255));
	};
	tiLevelName->AddOnFocusExitFunc(FnMFocusExit_TILevelName); // Assign func to input

	//// Create focus exit lambda func which changes the Outline color
	auto FnMTIn_TILevelName = [tiLevelName]() {
		if (activeLevel != nullptr)
			activeLevel->levelName = tiLevelName->GetText();

	};
	tiLevelName->AddOnTextInputFunc(FnMTIn_TILevelName); // Assign func to input

	// Fill data if accessible
	if (activeLevel != nullptr) {
		tiLevelName->SetText(activeLevel->levelName);
	}

	editorBarView->AddTextInput(tiLevelName);
	yOffset += tiLevelName->GetBoundaries().height + 10;

	// Button Save Level
	Button * btnSaveLevel = new Button();
	sf::RectangleShape * rectBtnSaveLevel = new sf::RectangleShape(sf::Vector2f(sfView->getSize().x -20, 30));
	btnSaveLevel->SetShape(rectBtnSaveLevel);
	btnSaveLevel->SetFillColor(sf::Color::Green);
	btnSaveLevel->SetText("Save", sf::Color::Black);
	btnSaveLevel->SetFont(generalFont);
	btnSaveLevel->SetOutline(2, sf::Color::Color(0, 143, 43, 255));
	btnSaveLevel->SetPosition(10, yOffset);

	//// Create mouse enter lambda func which changes the BG col
	auto FnMOver_BTNSaveLevel = [btnSaveLevel]() {
		btnSaveLevel->SetFillColor(sf::Color::Color(0, 143, 43, 255));
	};
	btnSaveLevel->AddMouseEnterFunc(FnMOver_BTNSaveLevel); // Assign func to input
	//// Create mouse exit lambda func which changes the BG col
	auto FnMExit_BTNSaveLevel = [btnSaveLevel]() {
		btnSaveLevel->SetFillColor(sf::Color::Green);
	};
	btnSaveLevel->AddMouseExitFunc(FnMExit_BTNSaveLevel); // Assign func to input
	//// Create mouse click func
	auto FnMReleased_BTNSaveLevel = []() {
		if(activeLevel != nullptr && SaveLevel(activeLevel))
			std::cout << "Level saved! "+activeLevel->existingFile << std::endl;
		else
			std::cout << "Level couldn't be saved! " + activeLevel->existingFile << std::endl;
	};
	btnSaveLevel->AddButtonReleasedFunc(FnMReleased_BTNSaveLevel); // Assign func to input

	yOffset += btnSaveLevel->GetGlobalBounds().height + 5;
	editorBarView->AddButton(btnSaveLevel);

	// Button Reset Level
	Button * btnResetLevel = new Button();
	sf::RectangleShape * rectBtnResetLevel = new sf::RectangleShape(sf::Vector2f(sfView->getSize().x - 20, 30));
	btnResetLevel->SetShape(rectBtnResetLevel);
	btnResetLevel->SetFillColor(sf::Color::Yellow);
	btnResetLevel->SetText("Reset", sf::Color::Black);
	btnResetLevel->SetFont(generalFont);
	btnResetLevel->SetOutline(2, sf::Color::Color(199, 209, 4, 255));
	btnResetLevel->SetPosition(10, yOffset);

	//// Create mouse enter lambda func which changes the BG col
	auto FnMOver_BTNResetLevel = [btnResetLevel]() {
		btnResetLevel->SetFillColor(sf::Color::Color(199, 209, 4, 255));
	};
	btnResetLevel->AddMouseEnterFunc(FnMOver_BTNResetLevel); // Assign func to input
	//// Create mouse exit lambda func which changes the BG col
	auto FnMExit_BTNResetLevel = [btnResetLevel]() {
		btnResetLevel->SetFillColor(sf::Color::Yellow);
	};
	btnResetLevel->AddMouseExitFunc(FnMExit_BTNResetLevel); // Assign func to input
	//// Create mouse click func
	auto FnMReleased_BTNResetLevel = []() {
		std::cout << "================ TODO: Reset level grid! " + activeLevel->levelName << std::endl;
	};
	btnResetLevel->AddButtonReleasedFunc(FnMReleased_BTNResetLevel); // Assign func to input

	yOffset += btnResetLevel->GetGlobalBounds().height + 5;
	editorBarView->AddButton(btnResetLevel);

	// Button Cancel Level
	Button * btnCancelLevel = new Button();
	sf::RectangleShape * rectBtnCancelLevel = new sf::RectangleShape(sf::Vector2f(sfView->getSize().x - 20, 30));
	btnCancelLevel->SetShape(rectBtnCancelLevel);
	btnCancelLevel->SetFillColor(sf::Color::Red);
	btnCancelLevel->SetText("Cancel", sf::Color::Black);
	btnCancelLevel->SetFont(generalFont);
	btnCancelLevel->SetOutline(2, sf::Color::Color(166, 7, 7, 255));
	btnCancelLevel->SetPosition(10, yOffset);

	//// Create mouse enter lambda func which changes the BG col
	auto FnMOver_BTNCancelLevel = [btnCancelLevel]() {
		btnCancelLevel->SetFillColor(sf::Color::Color(166, 7, 7, 255));
	};
	btnCancelLevel->AddMouseEnterFunc(FnMOver_BTNCancelLevel); // Assign func to input
	//// Create mouse exit lambda func which changes the BG col
	auto FnMExit_BTNCancelLevel = [btnCancelLevel]() {
		btnCancelLevel->SetFillColor(sf::Color::Red);
	};
	btnCancelLevel->AddMouseExitFunc(FnMExit_BTNCancelLevel); // Assign func to input
	//// Create mouse click func
	auto FnMReleased_BTNCancelLevel = []() {
		LevelEditorUnloadLevel();
		activeViews.clear();
		activeViews.push_back(ViewName::MainMenu);
	};
	btnCancelLevel->AddButtonReleasedFunc(FnMReleased_BTNCancelLevel); // Assign func to input

	yOffset += btnCancelLevel->GetGlobalBounds().height + 15;
	editorBarView->AddButton(btnCancelLevel);

	

	// Txt Tile Brushes
	sf::Text * txtTileBrushes = new sf::Text("Tile Brushes:", generalFont, 18);
	sf::Vector2f txtTileBrushesPos(sfView->getSize().x / 2 - txtTileBrushes->getGlobalBounds().width / 2, yOffset);
	txtTileBrushes->setPosition(txtTileBrushesPos);
	yOffset += txtTileBrushes->getGlobalBounds().height + 10;
	editorBarView->AddText(txtTileBrushes);

	float xOffset = 10;
	float tileButtonSize = (sfView->getSize().x-30)/3;
	for (auto const& entry : TileType::tileTypes) {
		Button * ttb = new Button();
		TileType * tt = entry.second;

		sf::RectangleShape * rect = new sf::RectangleShape(sf::Vector2f(tileButtonSize, tileButtonSize));
		ttb->SetShape(rect);
		ttb->SetOutline(2, sf::Color::Color(99,99,99, 255));
		ttb->SetPosition(xOffset, yOffset);

		
		sf::Sprite * btnSprite = new sf::Sprite(*TileType::tileTypeTextures[entry.first], sf::IntRect(0, 0, activeLevel->tileSize, activeLevel->tileSize));
		btnSprite->setScale(tileButtonSize/activeLevel->tileSize, tileButtonSize/ activeLevel->tileSize);
		btnSprite->setPosition(xOffset, yOffset);
		ttb->SetSprite(btnSprite);

		//// Create mouse enter lambda func which changes the BG col
		auto FnMOver = [ttb]() {
			ttb->SetOutline(2, sf::Color::Green);
		};
		ttb->AddMouseEnterFunc(FnMOver); // Assign func to input
		//// Create mouse exit lambda func which changes the BG col
		auto FnMExit = [ttb, tt]() {
			if(activeBrush != tt)
				ttb->SetOutline(2, sf::Color::Color(99, 99, 99, 255));
		};
		ttb->AddMouseExitFunc(FnMExit); // Assign func to input
		//// Create mouse click func
		auto FnMReleased = [ttb, tt]() {
			activeBrush = tt;

			for (Button * b : brushButtons) {
				b->SetOutline(2, sf::Color::Color(99, 99, 99, 255));
			}

			ttb->SetOutline(2, sf::Color::Green);
		};
		ttb->AddButtonReleasedFunc(FnMReleased); // Assign func to input


		xOffset += tileButtonSize + 5;
		if (xOffset + tileButtonSize >= sfView->getSize().x) {
			xOffset = 10;
			yOffset += tileButtonSize + 5;
		}

		editorBarView->AddButton(ttb);
		brushButtons.push_back(ttb);
	}

	views.insert({ ViewName::LevelEditorSideBar, editorBarView });
}

void InitLevelEditorView() {
	View * editorView = new View("Level Editor", (float)renderWindow.getSize().x, (float)renderWindow.getSize().y);
	float yOffset = 10;
	sf::View * sfView = new sf::View(sf::FloatRect(0,0, renderWindow.getSize().x*0.85f, (float)renderWindow.getSize().y)); //renderWindow.getPosition().x + renderWindow.getSize().x/2, renderWindow.getPosition().y + renderWindow.getSize().y / 2
	sfView->setViewport(sf::FloatRect(0.15f, 0, 0.85f, 1));
	editorView->SetSfView(sfView);

	auto it = views.find(ViewName::LevelEditor);
	if (it != views.end()) {
		views[ViewName::LevelEditor] = editorView;
	}
	else {
		views.insert({ ViewName::LevelEditor, editorView });
	}
	
}

void InitMainMenuView() {
	View * mainMenuView = new View("Main Menu", (float)renderWindow.getSize().x, (float)renderWindow.getSize().y);
	float yOffset = 10;

	// Title
	sf::Text * txtTitle = new sf::Text("Level Editor", generalFont, 30);
	sf::Vector2f titlePos(mainMenuView->GetViewRect()->getGlobalBounds().left + mainMenuView->GetViewRect()->getGlobalBounds().width/2 - txtTitle->getGlobalBounds().width/2, yOffset);
	txtTitle->setPosition(titlePos);
	yOffset += txtTitle->getGlobalBounds().height + 30;
	mainMenuView->AddText(txtTitle);

	// Txt Create New Level
	sf::Text * txtCreateNewLevel = new sf::Text("Create New Level", generalFont, 24);
	sf::Vector2f txtCreateNewLevelPos(mainMenuView->GetViewRect()->getGlobalBounds().left + mainMenuView->GetViewRect()->getGlobalBounds().width / 2 - txtCreateNewLevel->getGlobalBounds().width / 2, yOffset);
	txtCreateNewLevel->setPosition(txtCreateNewLevelPos);
	yOffset += txtCreateNewLevel->getGlobalBounds().height + 10;
	mainMenuView->AddText(txtCreateNewLevel);

	// Txt level name
	sf::Text * txtNewLevelName = new sf::Text("Level Name:", generalFont, 18);
	sf::Vector2f txtNewLevelNamePos(mainMenuView->GetViewRect()->getGlobalBounds().left + mainMenuView->GetViewRect()->getGlobalBounds().width / 2 - txtNewLevelName->getGlobalBounds().width - 10, yOffset);
	txtNewLevelName->setPosition(txtNewLevelNamePos);
	mainMenuView->AddText(txtNewLevelName);

	// Txt Field new level name
	TextInput * tiNewLevelName = new TextInput();
	tiNewLevelName->SetFont(generalFont);
	tiNewLevelName->SetSize(200 , 20);
	tiNewLevelName->allowOnlyAlphaNumerical = true;
	tiNewLevelName->maxLength = 30;
	sf::Vector2f tiNewLevelNamePos(mainMenuView->GetViewRect()->getGlobalBounds().left + mainMenuView->GetViewRect()->getGlobalBounds().width / 2 + 10, yOffset);
	tiNewLevelName->SetPosition(tiNewLevelNamePos);
	tiNewLevelName->SetTextColor(sf::Color::Black);
	//// Create mouse enter lambda func which changes the Outline color
	auto FnMOver_TINewLevelName = [tiNewLevelName]() {
		if(!tiNewLevelName->HasFocus())
			tiNewLevelName->SetBorder(2,sf::Color::Black);
	};
	tiNewLevelName->AddOnMouseEnterFunc(FnMOver_TINewLevelName); // Assign func to input
	//// Create mouse exit lambda func which changes the Outline color
	auto FnMExit_TINewLevelName = [tiNewLevelName]() {
		if (!tiNewLevelName->HasFocus())
			tiNewLevelName->SetBorder(2, sf::Color::Color(99,99,99,255));
	};
	tiNewLevelName->AddOnMouseExitFunc(FnMExit_TINewLevelName); // Assign func to input
	//// Create focus lambda func which changes the Outline color
	auto FnMFocus_TINewLevelName = [tiNewLevelName]() {
		tiNewLevelName->SetBorder(2, sf::Color::Cyan);
	};
	tiNewLevelName->AddOnFocusFunc(FnMFocus_TINewLevelName); // Assign func to input
	//// Create focus exit lambda func which changes the Outline color
	auto FnMFocusExit_TINewLevelName = [tiNewLevelName]() {
		tiNewLevelName->SetBorder(2, sf::Color::Color(99,99,99,255));
	};
	tiNewLevelName->AddOnFocusExitFunc(FnMFocusExit_TINewLevelName); // Assign func to input
	
	mainMenuView->AddTextInput(tiNewLevelName);
	yOffset += txtNewLevelName->getGlobalBounds().height + 10;

	// Txt level width
	sf::Text * txtNewLevelWidth = new sf::Text("Width:", generalFont, 18);
	sf::Vector2f txtNewLevelWidthPos(mainMenuView->GetViewRect()->getGlobalBounds().left + mainMenuView->GetViewRect()->getGlobalBounds().width / 2 - txtNewLevelName->getGlobalBounds().width - 10, yOffset);
	txtNewLevelWidth->setPosition(txtNewLevelWidthPos);
	mainMenuView->AddText(txtNewLevelWidth);

	// Txt Field new level width
	TextInput * tiNewLevelWidth = new TextInput();
	tiNewLevelWidth->SetFont(generalFont);
	tiNewLevelWidth->SetSize(40, 20);
	tiNewLevelWidth->allowOnlyNumerical = true;
	tiNewLevelWidth->maxLength = 5;
	sf::Vector2f tiNewLevelWidthPos(mainMenuView->GetViewRect()->getGlobalBounds().left + mainMenuView->GetViewRect()->getGlobalBounds().width / 2 + 10, yOffset);
	tiNewLevelWidth->SetPosition(tiNewLevelWidthPos);
	tiNewLevelWidth->SetTextColor(sf::Color::Black);
	//// Create mouse enter lambda func which changes the Outline color
	auto FnMOver_TINewLevelWidth = [tiNewLevelWidth]() {
		if (!tiNewLevelWidth->HasFocus())
			tiNewLevelWidth->SetBorder(2, sf::Color::Black);
	};
	tiNewLevelWidth->AddOnMouseEnterFunc(FnMOver_TINewLevelWidth); // Assign func to input
	//// Create mouse exit lambda func which changes the Outline color
	auto FnMExit_TINewLevelWidth = [tiNewLevelWidth]() {
		if (!tiNewLevelWidth->HasFocus())
			tiNewLevelWidth->SetBorder(2, sf::Color::Color(99, 99, 99, 255));
	};
	tiNewLevelWidth->AddOnMouseExitFunc(FnMExit_TINewLevelWidth); // Assign func to input
	//// Create focus lambda func which changes the Outline color
	auto FnMFocus_TINewLevelWidth = [tiNewLevelWidth]() {
		tiNewLevelWidth->SetBorder(2, sf::Color::Cyan);
	};
	tiNewLevelWidth->AddOnFocusFunc(FnMFocus_TINewLevelWidth); // Assign func to input
	//// Create focus exit lambda func which changes the Outline color
	auto FnMFocusExit_TINewLevelWidth = [tiNewLevelWidth]() {
		tiNewLevelWidth->SetBorder(2, sf::Color::Color(99, 99, 99, 255));
	};
	tiNewLevelWidth->AddOnFocusExitFunc(FnMFocusExit_TINewLevelWidth); // Assign func to input

	mainMenuView->AddTextInput(tiNewLevelWidth);
	yOffset += txtNewLevelWidth->getGlobalBounds().height + 10;

	// Txt level height
	sf::Text * txtNewLevelHeight = new sf::Text("Height:", generalFont, 18);
	sf::Vector2f txtNewLevelHeightPos(mainMenuView->GetViewRect()->getGlobalBounds().left + mainMenuView->GetViewRect()->getGlobalBounds().width / 2 - txtNewLevelName->getGlobalBounds().width - 10, yOffset);
	txtNewLevelHeight->setPosition(txtNewLevelHeightPos);
	mainMenuView->AddText(txtNewLevelHeight);

	// Txt Field new level height
	TextInput * tiNewLevelHeight = new TextInput();
	tiNewLevelHeight->SetFont(generalFont);
	tiNewLevelHeight->SetSize(40, 20);
	tiNewLevelHeight->allowOnlyNumerical = true;
	tiNewLevelHeight->maxLength = 5;
	sf::Vector2f tiNewLevelHeightPos(mainMenuView->GetViewRect()->getGlobalBounds().left + mainMenuView->GetViewRect()->getGlobalBounds().width / 2 + 10, yOffset);
	tiNewLevelHeight->SetPosition(tiNewLevelHeightPos);
	tiNewLevelHeight->SetTextColor(sf::Color::Black);
	//// Create mouse enter lambda func which changes the Outline color
	auto FnMOver_TINewLevelHeight = [tiNewLevelHeight]() {
		if (!tiNewLevelHeight->HasFocus())
			tiNewLevelHeight->SetBorder(2, sf::Color::Black);
	};
	tiNewLevelHeight->AddOnMouseEnterFunc(FnMOver_TINewLevelHeight); // Assign func to input
	//// Create mouse exit lambda func which changes the Outline color
	auto FnMExit_TINewLevelHeight = [tiNewLevelHeight]() {
		if (!tiNewLevelHeight->HasFocus())
			tiNewLevelHeight->SetBorder(2, sf::Color::Color(99, 99, 99, 255));
	};
	tiNewLevelHeight->AddOnMouseExitFunc(FnMExit_TINewLevelHeight); // Assign func to input
	//// Create focus lambda func which changes the Outline color
	auto FnMFocus_TINewLevelHeight = [tiNewLevelHeight]() {
		tiNewLevelHeight->SetBorder(2, sf::Color::Cyan);
	};
	tiNewLevelHeight->AddOnFocusFunc(FnMFocus_TINewLevelHeight); // Assign func to input
	//// Create focus exit lambda func which changes the Outline color
	auto FnMFocusExit_TINewLevelHeight = [tiNewLevelHeight]() {
		tiNewLevelHeight->SetBorder(2, sf::Color::Color(99, 99, 99, 255));
	};
	tiNewLevelHeight->AddOnFocusExitFunc(FnMFocusExit_TINewLevelHeight); // Assign func to input

	mainMenuView->AddTextInput(tiNewLevelHeight);
	yOffset += txtNewLevelHeight->getGlobalBounds().height + 20;

	// Button Create New Level
	Button * btnCreateNewLevel = new Button();
	sf::RectangleShape * rectBtnCreateNewLevel = new sf::RectangleShape(sf::Vector2f(200, 40));
	btnCreateNewLevel->SetShape(rectBtnCreateNewLevel);
	btnCreateNewLevel->SetFillColor(sf::Color::Green);
	btnCreateNewLevel->SetText("Create Level", sf::Color::Black);
	btnCreateNewLevel->SetFont(generalFont);
	btnCreateNewLevel->SetOutline(2,sf::Color::Color(0,143,43,255));
	btnCreateNewLevel->SetPosition(mainMenuView->GetViewRect()->getGlobalBounds().left + mainMenuView->GetViewRect()->getGlobalBounds().width / 2 - btnCreateNewLevel->GetGlobalBounds().width/2,yOffset);
	
	//// Create mouse enter lambda func which changes the BG col
	auto FnMOver_BTNCreateNewLevel = [btnCreateNewLevel]() {
		btnCreateNewLevel->SetFillColor(sf::Color::Color(0, 143, 43, 255));
	};
	btnCreateNewLevel->AddMouseEnterFunc(FnMOver_BTNCreateNewLevel); // Assign func to input
	//// Create mouse exit lambda func which changes the BG col
	auto FnMExit_BTNCreateNewLevel = [btnCreateNewLevel]() {
		btnCreateNewLevel->SetFillColor(sf::Color::Green);
	};
	btnCreateNewLevel->AddMouseExitFunc(FnMExit_BTNCreateNewLevel); // Assign func to input
	//// Create mouse click func
	auto FnMReleased_BTNCreateNewLevel = [tiNewLevelName,tiNewLevelWidth,tiNewLevelHeight]() {
		Level * newLevel = new Level(std::stoi(tiNewLevelWidth->GetText()), std::stoi(tiNewLevelHeight->GetText()),Level::defaultTileSize,tiNewLevelName->GetText());
		
		LevelEditorLoadLevel(newLevel);
		activeViews.clear();
		activeViews.push_back(ViewName::LevelEditorSideBar);
		activeViews.push_back(ViewName::LevelEditor);
		//activeView = ViewName::LevelEditor;
		// TODO Change View
	};
	btnCreateNewLevel->AddButtonReleasedFunc(FnMReleased_BTNCreateNewLevel); // Assign func to input

	yOffset += btnCreateNewLevel->GetGlobalBounds().height + 40;
	mainMenuView->AddButton(btnCreateNewLevel);


	// Txt Open Existing Level
	sf::Text * txtOpenExistingLevel = new sf::Text("Open Existing Level", generalFont, 24);
	sf::Vector2f txtOpenExistingLevelPos(mainMenuView->GetViewRect()->getGlobalBounds().left + mainMenuView->GetViewRect()->getGlobalBounds().width / 2 - txtCreateNewLevel->getGlobalBounds().width / 2, yOffset);
	txtOpenExistingLevel->setPosition(txtOpenExistingLevelPos);
	yOffset += txtOpenExistingLevel->getGlobalBounds().height + 10;
	mainMenuView->AddText(txtOpenExistingLevel);

	// Txt Path
	sf::Text * txtPath = new sf::Text("Path: ", generalFont, 18);
	sf::Vector2f txtPathPos(mainMenuView->GetViewRect()->getGlobalBounds().left + mainMenuView->GetViewRect()->getGlobalBounds().width / 2 - txtCreateNewLevel->getGlobalBounds().width / 2, yOffset);
	txtPath->setPosition(txtPathPos);
	mainMenuView->AddText(txtPath);

	// Txt Field Path
	TextInput * tiPath = new TextInput();
	tiPath->SetFont(generalFont);
	tiPath->SetSize(200, 20);
	sf::Vector2f tiPathPos(mainMenuView->GetViewRect()->getGlobalBounds().left + mainMenuView->GetViewRect()->getGlobalBounds().width / 2 + 10, yOffset);
	tiPath->SetPosition(tiPathPos);
	tiPath->SetTextColor(sf::Color::Black);
	//// Create mouse enter lambda func which changes the Outline color
	auto FnMOver_TIPath = [tiPath]() {
		if (!tiPath->HasFocus())
			tiPath->SetBorder(2, sf::Color::Black);
	};
	tiPath->AddOnMouseEnterFunc(FnMOver_TIPath); // Assign func to input
	//// Create mouse exit lambda func which changes the Outline color
	auto FnMExit_TIPath = [tiPath]() {
		if (!tiPath->HasFocus())
			tiPath->SetBorder(2, sf::Color::Color(99, 99, 99, 255));
	};
	tiPath->AddOnMouseExitFunc(FnMExit_TIPath); // Assign func to input
	//// Create focus lambda func which changes the Outline color
	auto FnMFocus_TIPath = [tiPath]() {
		tiPath->SetBorder(2, sf::Color::Cyan);
	};
	tiPath->AddOnFocusFunc(FnMFocus_TIPath); // Assign func to input
	//// Create focus exit lambda func which changes the Outline color
	auto FnMFocusExit_TIPath = [tiPath]() {
		tiPath->SetBorder(2, sf::Color::Color(99, 99, 99, 255));
	};
	tiPath->AddOnFocusExitFunc(FnMFocusExit_TIPath); // Assign func to input

	mainMenuView->AddTextInput(tiPath);

	// Button Open Level
	Button * btnOpenLevelByPath = new Button();
	sf::RectangleShape * rectBtnOpenLevelByPath = new sf::RectangleShape(sf::Vector2f(50, 20));
	btnOpenLevelByPath->SetShape(rectBtnOpenLevelByPath);
	btnOpenLevelByPath->SetFillColor(sf::Color::Green);
	btnOpenLevelByPath->SetText("Open", sf::Color::Black);
	btnOpenLevelByPath->SetFont(generalFont);
	btnOpenLevelByPath->SetOutline(2, sf::Color::Color(0, 143, 43, 255));
	btnOpenLevelByPath->SetPosition(tiPath->GetBoundaries().left + tiPath->GetBoundaries().width + 10, yOffset);
	//// Create mouse enter lambda func which changes the BG col
	auto FnMOver_BTNOpenLevelByPath = [btnOpenLevelByPath]() {
		btnOpenLevelByPath->SetFillColor(sf::Color::Color(0, 143, 43, 255));
	};
	btnOpenLevelByPath->AddMouseEnterFunc(FnMOver_BTNOpenLevelByPath); // Assign func to input
	//// Create mouse exit lambda func which changes the BG col
	auto FnMExit_BTNOpenLevelByPath = [btnOpenLevelByPath]() {
		btnOpenLevelByPath->SetFillColor(sf::Color::Green);
	};
	btnOpenLevelByPath->AddMouseExitFunc(FnMExit_BTNOpenLevelByPath); // Assign func to input
	//// Create button release lambda func which tries to load the level
	auto FnMRelease_BTNLevel = [tiPath]() {
		std::string filename = tiPath->GetText();
		Level * level = LoadLevelFromFile(filename);

		if (level != nullptr) {
			LevelEditorLoadLevel(level);
			activeViews.clear();
			activeViews.push_back(ViewName::LevelEditorSideBar);
			activeViews.push_back(ViewName::LevelEditor);
		}
	};
	btnOpenLevelByPath->AddButtonReleasedFunc(FnMRelease_BTNLevel); // Assign func to input

	mainMenuView->AddButton(btnOpenLevelByPath);

	yOffset += btnOpenLevelByPath->GetGlobalBounds().height + 20;

	// Txt Level Folder
	sf::Text * txtFastAccess = new sf::Text("Fast Access (/levels): ", generalFont, 18);
	sf::Vector2f txtFastAccessPos(mainMenuView->GetViewRect()->getGlobalBounds().left + mainMenuView->GetViewRect()->getGlobalBounds().width / 2 - txtFastAccess->getGlobalBounds().width / 2, yOffset);
	txtFastAccess->setPosition(txtFastAccessPos);
	mainMenuView->AddText(txtFastAccess);
	yOffset += txtFastAccess->getGlobalBounds().height + 20;


	for (int i = 0; i < levelsInFolder.size(); i++) {
		float xPos = mainMenuView->GetViewRect()->getGlobalBounds().left + mainMenuView->GetViewRect()->getGlobalBounds().width/2 - 125 + (i % 5)*60;
		// Button Open Level
		Button * btnLevel = new Button();
		sf::RectangleShape * rectBtnLevel = new sf::RectangleShape(sf::Vector2f(50, 50));
		btnLevel->SetShape(rectBtnLevel);
		btnLevel->SetFillColor(sf::Color::Green);
		btnLevel->SetText(levelsInFolder[i], sf::Color::Black);
		btnLevel->SetFont(generalFont);
		btnLevel->SetOutline(2, sf::Color::Color(0, 143, 43, 255));
		btnLevel->SetPosition(xPos, yOffset);
		//// Create mouse enter lambda func which changes the BG col
		auto FnMOver_BTNLevel = [btnLevel]() {
			btnLevel->SetFillColor(sf::Color::Color(0, 143, 43, 255));
		};
		btnLevel->AddMouseEnterFunc(FnMOver_BTNLevel); // Assign func to input
		//// Create mouse exit lambda func which changes the BG col
		auto FnMExit_BTNLevel = [btnLevel]() {
			btnLevel->SetFillColor(sf::Color::Green);
		};
		btnLevel->AddMouseExitFunc(FnMExit_BTNLevel); // Assign func to input
		//// Create button release lambda func which changes the BG col
		std::string filename = levelsInFolder[i];
		auto FnMRelease_BTNLevel = [filename]() {
			// TODO Load Level
			std::cout << "[TODO] Load Level "+ filename +"!" << std::endl;
			Level * level = LoadLevelFromFile(exeDir+levelFolder+"\\"+filename+levelExt);

			if (level != nullptr) {
				LevelEditorLoadLevel(level);
				activeViews.clear();
				activeViews.push_back(ViewName::LevelEditorSideBar);
				activeViews.push_back(ViewName::LevelEditor);
			}
			else {
				std::cout << "Couldn't load level " + filename << std::endl;
			}
		};
		btnLevel->AddButtonReleasedFunc(FnMRelease_BTNLevel); // Assign func to input

		mainMenuView->AddButton(btnLevel);
		if((i+1) % 5 == 0)
			yOffset += btnLevel->GetGlobalBounds().height + 10;
	}

	views.insert({ ViewName::MainMenu, mainMenuView });
}

void InitVars() {
	renderWindow.create(sf::VideoMode((unsigned)windowWidth, (unsigned)windowHeight), "Level Editor - GAD173 - Brief 2"); // Initialize the window

	// Try to load the main font
	if (!generalFont.loadFromFile(exeDir + fontName))
	{
		std::cout << "Couldn't load font!" << std::endl;
	}
}

#pragma endregion


#pragma region General Logic

sf::Vector2i lastMousePos;
void WindowLifeCycle() {


	sf::Clock Clock;
	int offset = 0;

	sf::RectangleShape windowBg(sf::Vector2f(windowWidth, windowHeight));
	windowBg.setFillColor(sf::Color::Color(33, 33, 33, 255));

	while (renderWindow.isOpen())
	{

		mouseWheelScrollX = 0;

		deltaTime = Clock.restart().asSeconds();
		sf::Event event;
		while (renderWindow.pollEvent(event))
		{
			for (ViewName viewName : activeViews) {
				HandleViewEvents(viewName, event);
			}

			switch (event.type) {
			case sf::Event::Closed: {
				renderWindow.close();
				break;
			}
			case sf::Event::MouseMoved: {
				mousePosInt = sf::Mouse::getPosition(renderWindow);
				mousePos.x = (float)mousePosInt.x;
				mousePos.y = (float)mousePosInt.y;
				break;
			}
			case sf::Event::MouseWheelScrolled: {
				mouseWheelScrollX = event.mouseWheelScroll.delta;
				break;
			}
			case sf::Event::MouseButtonPressed: {
				if(event.mouseButton.button == sf::Mouse::Button::Left)
					leftMouseButtonPressed = true;
				if (event.mouseButton.button == sf::Mouse::Button::Middle)
					middleMouseButtonPressed = true;
				break;
			}
			case sf::Event::MouseButtonReleased: {
				if (event.mouseButton.button == sf::Mouse::Button::Left)
					leftMouseButtonPressed = false;
				if (event.mouseButton.button == sf::Mouse::Button::Middle)
					middleMouseButtonPressed = false;
				break;
			}
			}

		}


		// TODO Testing view movement
		if (std::find(activeViews.begin(), activeViews.end(), ViewName::LevelEditor) != activeViews.end() && views[ViewName::LevelEditor] != nullptr && activeLevel != nullptr) {
			// ======== SCROLLING
			sf::View * sfView = views[ViewName::LevelEditor]->GetSFView();
			sf::Vector2f movement(0, 0);
			float zoomMulti = 1;// (currentZoom < 1) ? std::abs(currentZoom) : currentZoom;
			//std::cout << currentZoom << std::endl;

			if (middleMouseButtonPressed) {
				if (lastMousePos.x == 0 && lastMousePos.y == 0) {
					lastMousePos = mousePosInt;
					continue;
				}
				sf::Vector2i direction(lastMousePos.x - mousePos.x, lastMousePos.y - mousePos.y);
				
				if (direction.x > 10)
					direction.x = 10;
				if (direction.x < -10)
					direction.x = -10;
				if (direction.y > 10)
					direction.y = 10;
				if (direction.y < -10)
					direction.y = -10;
					
				if (direction.x > 3 || direction.x < -3) {
					movement.x = direction.x * 150 * deltaTime;
				}
				if (direction.y > 3 || direction.y < -3) {
					movement.y = direction.y * 150 * deltaTime;
				}

				lastMousePos = mousePosInt;
			}
			else {
				lastMousePos = sf::Vector2i(0, 0);
				// Move grid in view with arrow keys
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
					(sfView->getCenter().x*zoomMulti - sfView->getSize().x / 2 - 150 * deltaTime) > 0)/*renderWindow.getSize().x / 2 * sfView->getViewport().width*/
					movement.x = -150 * deltaTime;
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
					(sfView->getCenter().x*zoomMulti + sfView->getSize().x / 2 + 150 * deltaTime) < activeLevel->tileSize*activeLevel->gridSize.x)/*renderWindow.getSize().x / 2 * sfView->getViewport().width */
					movement.x = 150 * deltaTime;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
					(sfView->getCenter().y*zoomMulti - sfView->getSize().y / 2 - 150 * deltaTime) > 0)/*renderWindow.getSize().y / 2 * sfView->getViewport().height*/
					movement.y = -150 * deltaTime;
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
					(sfView->getCenter().y*zoomMulti + sfView->getSize().y / 2 + 150 * deltaTime) < activeLevel->tileSize*activeLevel->gridSize.y)/*renderWindow.getSize().y / 2 * sfView->getViewport().height*/
					movement.y = 150 * deltaTime;

			}
			sfView->move(movement);
			
			//std::cout << sfView->getCenter().x - renderWindow.getSize().x / 2 * sfView->getViewport().width << std::endl;


			if (enableZoom && mouseWheelScrollX != 0 && currentZoom + mouseWheelScrollX * deltaTime < 2 && currentZoom + mouseWheelScrollX * deltaTime > 0.8) {
				//mouseWheelScrollX -= mouseWheelScrollX * deltaTime;
				//currentZoom += mouseWheelScrollX * deltaTime;
				currentZoom += mouseWheelScrollX * deltaTime * 5;
				sfView->zoom(1 + mouseWheelScrollX * deltaTime * 5);
			}
		}

		renderWindow.clear();
		renderWindow.setView(renderWindow.getDefaultView());
		renderWindow.draw(windowBg);


		//DrawGrid();
		//DrawButtons();

		for (ViewName viewName : activeViews) {
			DrawActiveView(viewName);
		}


		renderWindow.display();
	}
}

void DrawActiveView(ViewName activeView) {
	if (views[activeView] == nullptr)
		return;

	sf::View sfView;
	if (views[activeView]->GetSFView() == nullptr)
		sfView = renderWindow.getDefaultView();
	else
		sfView = *views[activeView]->GetSFView();

	renderWindow.setView(sfView);

	for (sf::Drawable * drawable : views[activeView]->GetDrawables()) {
		if (drawable != nullptr) {
			renderWindow.draw(*drawable);
		}


	}

}

#pragma endregion

#pragma region View Logic

void HandleViewEvents(ViewName viewName, sf::Event e) {
	if (views[viewName] == nullptr)
		return;

	sf::View* sfView;
	if (views[viewName]->GetSFView() == nullptr)
		sfView = new sf::View(renderWindow.getDefaultView());
	else
		sfView = views[viewName]->GetSFView();

	renderWindow.setView(*sfView);

	// Buttons
	for (Button * b : views[viewName]->GetButtons()) {
		switch (e.type) {
		case sf::Event::MouseMoved:
			if (b->isActive && b->GetGlobalBounds().contains(renderWindow.mapPixelToCoords(mousePosInt))) {
				b->SetMouseEnter();
			}
			else if(b->isActive) {
				b->SetMouseExit();
			}
			break;
		case sf::Event::MouseButtonPressed:
			if (e.mouseButton.button == sf::Mouse::Button::Left && b->isActive && b->GetGlobalBounds().contains(renderWindow.mapPixelToCoords(mousePosInt))) {
				b->SetButtonPressed();
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (e.mouseButton.button == sf::Mouse::Button::Left && b->isActive && b->GetGlobalBounds().contains(renderWindow.mapPixelToCoords(mousePosInt))) {
				b->SetButtonReleased();
			}
			break;
		}
	}

	// TextInput
	for (TextInput * ele : views[viewName]->GetTextInputs()) {
		switch (e.type) {
		case sf::Event::MouseMoved:
			if (ele->GetBoundaries().contains(renderWindow.mapPixelToCoords(mousePosInt))) {
				ele->_OnMouseEnter();
			}
			else{
				ele->_OnMouseExit();
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (ele->GetBoundaries().contains(renderWindow.mapPixelToCoords(mousePosInt))) {
				ele->_OnFocus();
			}
			else {
				ele->_OnFocusExit();
			}
			break;
		case sf::Event::TextEntered:
			if(e.text.unicode < 128 && ele->HasFocus())
				
				ele->_OnTextInput(static_cast<char>(e.text.unicode));
			break;
		}
	}
}

void CreateLevelEditorTileButtons() {
	if (activeLevel == nullptr)
		return;
	//buttonGrid.resize(activeLevel->gridSize.y, std::vector<Button*>(activeLevel->gridSize.x,nullptr));
	for (int y = 0; y < activeLevel->gridSize.y; y++) {
		for (int x = 0; x < activeLevel->gridSize.x; x++) {
			Button* b = new Button();

			float outlineColorThickness = 2;
			sf::RectangleShape * rect = new sf::RectangleShape(sf::Vector2f(activeLevel->tileSize- 2*outlineColorThickness, activeLevel->tileSize - 2*outlineColorThickness));
			rect->setFillColor(sf::Color::Color(0,0,0,0));
			rect->setOutlineThickness(outlineColorThickness);
			rect->setOutlineColor(sf::Color::Black);
			b->SetShape(rect);
			b->SetPosition(x * activeLevel->tileSize+ outlineColorThickness, y * activeLevel->tileSize+ outlineColorThickness);
			auto clickFunction = [x,y]() {
				if (activeLevel->GetTileAtCoord(x, y)->GetTileType() == nullptr || activeBrush == nullptr || activeLevel->GetTileAtCoord(x, y)->GetTileType() == activeBrush) {
					return;
				}
				activeLevel->GetTileAtCoord(x, y)->SetTileType(activeBrush);
				/*change tile type when clicking on it mouseButtonPressed
				std::map<char, TileType*>::iterator it = TileType::tileTypes.find(activeLevel->GetTileAtCoord(x, y)->GetTileType()->tileId);
				it++;
				if (it != TileType::tileTypes.end()) {
					activeLevel->GetTileAtCoord(x, y)->SetTileType(it->second);
				}
				else {
					activeLevel->GetTileAtCoord(x, y)->SetTileType(TileType::tileTypes[TileType::defaultTileTypeChar]);
				}*/
			};
			b->AddButtonReleasedFunc(clickFunction);
			b->AddButtonDownFunc(clickFunction);

			auto mouseEnterFunction = [x, y]() {
				if (activeLevel->GetTileAtCoord(x, y)->GetTileType() == nullptr || activeBrush == nullptr || activeLevel->GetTileAtCoord(x, y)->GetTileType() == activeBrush || !leftMouseButtonPressed) {
					return;
				}
				activeLevel->GetTileAtCoord(x, y)->SetTileType(activeBrush);
			};
			b->AddMouseEnterFunc(mouseEnterFunction);
			

			//buttonGrid[y][x] = b;
			//buttons.push_back(b);
			views[ViewName::LevelEditor]->AddButton(b);
		}
	}
}

#pragma endregion