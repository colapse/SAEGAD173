// 19T2-GAD173-Brief2-Taennler-1015901.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "TextInput.h"
#include "Button.h"
#include "TileType.h"
#include "Tile.h"
#include "Level.h"
#include "View.h"
#include <algorithm>
#include <iterator>
#include <filesystem>

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
void WindowLifeCycle();
void InitMainMenuView();
void CreateTileButtons();
void DrawGrid();
void DrawButtons();
void LoadTileTextures(int tileWidth, int tileHeight);
void InitVars();
void DrawActiveView();
void HanleViewEvents(sf::Event e);
void LoadLevelsFromDirectory();
#pragma endregion

#pragma region Global Variables
enum class ViewName
{
	MainMenu,
	LevelEditor
};

std::map<ViewName, View*> views;

sf::RenderWindow renderWindow;
sf::Font generalFont;
float windowWidth = 1000; // Width of the window
float windowHeight = 600; // Height of the window

std::string exeDir = "C:\\Users\\lucat\\Documents\\gitlocal\\SAEGAD173\\19T2-GAD173-Brief2-Taennler-1015901\\19T2-GAD173-Brief2-Taennler-1015901\\x64\\Debug\\"; // Directory of the exe file (Not a nice solution)
std::string fontName = "Radiant.ttf"; // Font retrieved from https://www.dafont.com/
std::string levelFolder = "levels";
std::string levelExt = ".lvl";

ViewName activeView = ViewName::MainMenu;

std::vector<std::string> levelsInFolder; //levels in level folder


#pragma endregion


std::string texturePath = "C:\\Users\\lucat\\Documents\\gitlocal\\SAEGAD173\\19T2-GAD173-Brief2-Taennler-1015901\\19T2-GAD173-Brief2-Taennler-1015901\\x64\\Debug\\textures\\";//C:\\Users\\lucat\\Documents\\gitlocal\\SAEGAD173\\19T2-GAD173-Brief2-Taennler-1015901\\19T2-GAD173-Brief2-Taennler-1015901\\x64\\Debug\\

std::vector<Button*> buttons;


std::vector<std::vector<Button*>> buttonGrid;
Level * activeLevel;
//std::vector<std::vector<Tile>> currentGrid;


sf::Vector2f mousePos;
float deltaTime = 0;

int main()
{
	TileType::tileTypes = {
	{'0',new TileType('0', "Air", texturePath + "tileAir.png")},
	{'c',new TileType('c', "Coin", texturePath + "tileCoin.png")},
	{'e',new TileType('e', "Enemy Spawn", texturePath + "tileEnemySpawn.png")},
	{'g',new TileType('g', "Ground", texturePath + "tileGround.png")},
	{'s',new TileType('s', "Player Spawn", texturePath + "tilePlayerExit.png")},
	{'t',new TileType('t', "Player Exit", texturePath + "tilePlayerSpawn.png")},
	{'w',new TileType('w', "Water", texturePath + "tileWater.png")}
	};

	LoadLevelsFromDirectory();
	InitVars();
	InitMainMenuView();
	LoadLevelsFromDirectory();
	WindowLifeCycle();
}

// original from https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
void LoadLevelsFromDirectory() {
	levelsInFolder.clear();
	for (const auto & entry : std::filesystem::directory_iterator(exeDir+levelFolder)) {
		if (entry.is_regular_file() && entry.path().extension() == levelExt) {
			levelsInFolder.push_back(entry.path().stem().u8string());
		}
	}
}

#pragma region Initialize

void InitMainMenuView() {
	View * mainMenuView = new View("Main Menu", renderWindow.getSize().x, renderWindow.getSize().y);
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

	mainMenuView->AddButton(btnOpenLevelByPath);

	yOffset += btnOpenLevelByPath->GetGlobalBounds().height + 20;

	// Txt Level Folder
	sf::Text * txtFastAccess = new sf::Text("Fast Access (TODO): ", generalFont, 18);
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
		//// Create mouse exit lambda func which changes the BG col
		std::string filename = levelsInFolder[i];
		auto FnMRelease_BTNLevel = [filename]() {
			// TODO Load Level
			std::cout << "[TODO] Load Level "+ filename +"!" << std::endl;
		};
		btnLevel->AddButtonReleasedFunc(FnMRelease_BTNLevel); // Assign func to input

		mainMenuView->AddButton(btnLevel);
		if((i+1) % 5 == 0)
			yOffset += btnLevel->GetGlobalBounds().height + 10;
	}

	views.insert({ ViewName::MainMenu, mainMenuView });
}

void InitVars() {
	renderWindow.create(sf::VideoMode(windowWidth, windowHeight), "Level Editor - GAD173 - Brief 2"); // Initialize the window

	// Try to load the main font
	if (!generalFont.loadFromFile(exeDir + fontName))
	{
		std::cout << "Couldn't load font!" << std::endl;
	}
}

#pragma endregion


void WindowLifeCycle() {
	

	sf::Clock Clock;
	int offset = 0;


	LoadTileTextures(50,50);
	activeLevel = new Level(5, 5, 50, 50);
	CreateTileButtons();
	
	//int windowWidth = activeLevel->gridSize.x * activeLevel->tileSize.x;
	//int windowHeight = activeLevel->gridSize.y * activeLevel->tileSize.y;
	sf::RectangleShape windowBg(sf::Vector2f(windowWidth, windowHeight));
	windowBg.setFillColor(sf::Color::Color(33,33,33,255));
	//renderWindow.create(sf::VideoMode(windowWidth, windowHeight), "Level Editor - GAD173 - Brief 2");
	while (renderWindow.isOpen())
	{
		deltaTime = Clock.restart().asSeconds();
		sf::Event event;
		while (renderWindow.pollEvent(event))
		{
			HanleViewEvents(event);
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


		//DrawGrid();
		//DrawButtons();

		DrawActiveView();

		renderWindow.display();
	}
}

void DrawActiveView() {
	if (views[activeView] == nullptr)
		return;

	// Draw Buttons
	for (Button * ele : views[activeView]->GetButtons()) {
		if (ele->isActive) {
			renderWindow.draw(*ele->GetShapeObject());
			renderWindow.draw(*ele->GetTextObject());
		}
	}

	// Draw Text Inputs
	for (TextInput * ele : views[activeView]->GetTextInputs()) {
		renderWindow.draw(*ele->GetRectComponent());
		renderWindow.draw(*ele->GetTextComponent());
	}

	// Draw Shapes
	for (sf::Shape * ele : views[activeView]->GetShapes()) {
		renderWindow.draw(*ele);
	}

	// Draw Texts
	for (sf::Text * ele : views[activeView]->GetTexts()) {
		renderWindow.draw(*ele);
	}

}

void HanleViewEvents(sf::Event e) {
	if (views[activeView] == nullptr)
		return;

	// Buttons
	for (Button * b : views[activeView]->GetButtons()) {
		switch (e.type) {
		case sf::Event::MouseMoved:
			if (b->isActive && b->GetGlobalBounds().contains(mousePos)) {
				b->SetMouseEnter();
			}
			else if(b->isActive) {
				b->SetMouseExit();
			}
			break;
		case sf::Event::MouseButtonPressed:
			if (b->isActive && b->GetGlobalBounds().contains(mousePos)) {
				b->SetButtonPressed();
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (b->isActive) {
				b->SetButtonReleased();
			}
			break;
		}
	}

	// TextInput
	for (TextInput * ele : views[activeView]->GetTextInputs()) {
		switch (e.type) {
		case sf::Event::MouseMoved:
			if (ele->GetBoundaries().contains(mousePos)) {
				ele->_OnMouseEnter();
			}
			else{
				ele->_OnMouseExit();
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (ele->GetBoundaries().contains(mousePos)) {
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

void CreateTileButtons() {
	buttonGrid.resize(activeLevel->gridSize.y, std::vector<Button*>(activeLevel->gridSize.x,nullptr));
	for (int y = 0; y < activeLevel->gridSize.y; y++) {
		for (int x = 0; x < activeLevel->gridSize.x; x++) {
			Button* b = new Button();

			int outlineColorThickness = 2;
			sf::RectangleShape * rect = new sf::RectangleShape(sf::Vector2f(activeLevel->tileSize.x- 2*outlineColorThickness, activeLevel->tileSize.y - 2*outlineColorThickness));
			rect->setFillColor(sf::Color::Color(0,0,0,0));
			rect->setOutlineThickness(outlineColorThickness);
			rect->setOutlineColor(sf::Color::Black);
			b->SetShape(rect);
			b->SetPosition(x * activeLevel->tileSize.x+ outlineColorThickness, y * activeLevel->tileSize.y+ outlineColorThickness);
			auto clickFunction = [x,y]() {

				if (activeLevel->GetTileAtCoord(x, y)->GetTileType() == nullptr)
					return;

				std::map<char, TileType*>::iterator it = TileType::tileTypes.find(activeLevel->GetTileAtCoord(x, y)->GetTileType()->tileId);
				it++;
				if (it != TileType::tileTypes.end()) {
					activeLevel->GetTileAtCoord(x, y)->SetTileType(it->second);
				}
				else {
					activeLevel->GetTileAtCoord(x, y)->SetTileType(TileType::tileTypes['0']);
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
		if(b->GetShapeObject() != nullptr)
			renderWindow.draw(*b->GetShapeObject());
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
