/**
 * @file 19T2-GAD173-Brief1-Taennler-1015901.cpp
 * @brief 19T2 GAD173 Brief 1 Breakout
 * @author Luca Taennler
 * Contact: 1015901@student.sae.edu.au
*/

#include "pch.h"
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <Windows.h>
#include <string>
#include <functional>

#pragma region Classes
/**
 * Implementation of Button class (BETA)
 * 
 * Enables easy creation of a button with text and input events (mouse & click). (Sprite in button not supported yet)
*/
class Button {
	// Private Variables: Input States
	bool isMouseOver = false;
	bool isMousePressed = false;
	bool isButtonDown = false;

	// Private Variables: Button Components
	sf::Font font; // Font used for the text component
	sf::Text * _txtText; // Text component of the button
	sf::Shape * _shape = nullptr; // Pointer to a shape. (=> Buttons "boundaries")
	sf::Sprite _sprite; // (Optional) Used for an image button - NOT IMPLEMENTED YET

	// Private Variables: Input Event Functions. They store functions (Such as lambda functions) which will be executed on specific input events
	std::vector<std::function<void()>> btnDownFuncs;
	std::vector<std::function<void()>> btnPressedFuncs;
	std::vector<std::function<void()>> btnReleasedFuncs;
	std::vector<std::function<void()>> mouseEnterFuncs;
	std::vector<std::function<void()>> mouseExitFuncs;

	// Private Variables: Settings
	float minTextMargin = 10; // The minimum space between the texts boundaries and the buttons border

	/** OnButtonDown: Executes functions when button is pressed down. (Happens only once until button is released again) */
	void OnButtonDown() {
		// If the button is currently held down, exit
		if (isButtonDown)
			return;

		// Loop through btnDownFuncs list
		for (std::function<void()> func : btnDownFuncs) {
			func();
		}
	}

	/** SetAutoCharacterSize: Finds the perfect character size for the text component to fit within the buttons bounds */
	void SetAutoCharacterSize() {
		// Exit if either the _shape or _txtText component is null. (Avoid access violations)
		if (_shape == nullptr || _txtText == nullptr)
			return;

		// Calculate the horizontal ratio between the buttons & texts(+ minium margin) width. (Basically checking if the text is overflowing the button horizontally or if its too small.)
		float horRatio = _shape->getLocalBounds().width / (_txtText->getGlobalBounds().width + minTextMargin * 2);
		// Calculate a new character size depending on the horizontal ratio
		int newSize = _txtText->getCharacterSize() * horRatio;

		// Calculate the vertical ratio between the buttons & texts(+ minium margin) height. (Basically checking if the text is overflowing the button vertically or if its too small.)
		float vertRatio = _shape->getLocalBounds().height / (2*newSize + minTextMargin * 2);
		// Check if the text is overflowing vertically with the new character size
		if (vertRatio < 1) {
			// Modify the new character size to fit in vertically
			newSize *= vertRatio;
		}

		// Set the new character size
		_txtText->setCharacterSize(newSize);
	}

	/** SetTextPosition: Centers the text within the button. */
	void SetTextPosition() {
		if (_shape != nullptr || _txtText != nullptr) { // Avoid access violations
			// Calculate & set the center position depending on the texts & buttons boundaries
			_txtText->setPosition((_shape->getGlobalBounds().left + _shape->getGlobalBounds().width / 2 - (_txtText->getGlobalBounds().width / 2)), (_shape->getGlobalBounds().top + _shape->getGlobalBounds().height / 2 - (_txtText->getGlobalBounds().height / 2)));
		}
	}

public:
	// Public Variables: States
	bool isActive = true; // Defines if the button is activated or not

#pragma region Button Event Handling
	/** SetButtonPressed: Sets the buttons press state to true & executes button pressed functions. (Continously until button isn't pressed down anymore) */
	void SetButtonPressed() {
		// Check if the button is already set as pressed down
		if (!isButtonDown) {
			OnButtonDown(); // OnButtonDown() will execute one-time functions when the button is pressed down
			isButtonDown = true; // Set the buttons state to pressed down
		}

		// Iterate through all btnPressedFuncs (Functions that will be executed over'n'over again while the button is pressed down.)
		for (std::function<void()> func : btnPressedFuncs) {
			func();
		}
	}

	/** SetButtonReleased: Sets the buttons press state to false & executes button released functions (Once). */
	void SetButtonReleased() {
		// Exit if the buttons pressed state is already set to false
		if (!isButtonDown)
			return;

		isButtonDown = false; // Set the buttons state to pressed down to false

		// Iterate through all btnReleasedFuncs (Functions that will be executed once when the button is released.)
		for (std::function<void()> func : btnReleasedFuncs) {
			func();
		}
	}

	/** SetMouseEnter: Sets the mouse over state to true & executes mouse enter functions (Once). */
	void SetMouseEnter() {
		// Exit if the mouse over state is already set to true
		if (isMouseOver)
			return;

		isMouseOver = true; // Set mouse over state to true

		// Iterate through all mouseEnterFuncs (Functions that will be executed once when the mouse enters the buttons boundaries.)
		for (std::function<void()> func : mouseEnterFuncs) {
			func();
		}
	}

	/** SetMouseExit: Sets the mouse over state to false & executes mouse exit functions (Once). */
	void SetMouseExit() {
		// Exit if the mouse over state is already set to false
		if (!isMouseOver)
			return;

		isMouseOver = false; // Set mouse over state to false

		// Iterate through all mouseExitFuncs (Functions that will be executed once when the mouse exits the buttons boundaries.)
		for (std::function<void()> func : mouseExitFuncs) {
			func();
		}
	}

	/** AddButtonDownFunc: Add a new function which will be called once when the button is pressed down.
	 * @param func The function (F.ex. a lambda function) to be added
	*/
	void AddButtonDownFunc(std::function<void()> func) {
		btnDownFuncs.push_back(func);
	}

	/** AddButtonPressedFunc: Add a new function which will be called over'n'over again while the button is pressed down.
	 * @param func The function (F.ex. a lambda function) to be added
	*/
	void AddButtonPressedFunc(std::function<void()> func) {
		btnPressedFuncs.push_back(func);
	}

	/** AddButtonReleasedFunc: Add a new function which will be called once when the button is released.
	 * @param func The function (F.ex. a lambda function) to be added
	*/
	void AddButtonReleasedFunc(std::function<void()> func) {
		btnReleasedFuncs.push_back(func);
	}

	/** AddMouseEnterFunc: Add a new function which will be called once when the mouse enters the buttons boundaries.
	 * @param func The function (F.ex. a lambda function) to be added
	*/
	void AddMouseEnterFunc(std::function<void()> func) {
		mouseEnterFuncs.push_back(func);
	}

	/** AddButtonDownFunc: Add a new function which will be called once when the mouse exits the buttons boundaries.
	 * @param func The function (F.ex. a lambda function) to be added
	*/
	void AddMouseExitFunc(std::function<void()> func) {
		mouseExitFuncs.push_back(func);
	}

	/* === Following functions will enable to remove button event functions - NOT FULLY IMPLEMENTED YET.
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

	/** Default Constructor for Button class */
	Button() {
		_txtText = new sf::Text(); // Initialize text component
	}

	/** SetShape: Assign a pointer to a shape (This can be a RectangularShape, CircleShape or any other child class of sf::Shape
	 * @param shape The pointer to the shape
	*/
	void SetShape(sf::Shape * shape) {
		this->_shape = shape; // Assign the shape pointer
		SetTextPosition(); // This function call will realign the text to the new boundaries of the button
	}

	/** SetPosition: Sets the position of the button
	 * @param x The new x position
	 * @param y The new y position
	*/
	void SetPosition(float x, float y) {
		// Exit if the shape isn't set (Avoid access violations)
		if (_shape == nullptr)
			return;
		_shape->setPosition(x, y); // Set the shapes global position
		SetTextPosition(); // This function call will moves the text component along with the new shape position
	}

	/** ScaleSize: Scale the buttons shape
	 * @param scaleX Horizontal scale factor
	 * @param scaleY Vertical scale factor
	*/
	void ScaleSize(float scaleX, float scaleY) {
		// Exit if the shape isn't set (Avoid access violations)
		if (_shape == nullptr)
			return;
		_shape->setScale(scaleX, scaleY); // Sets the shapes new scale
		SetAutoCharacterSize(); // Resize the text to fit within the new shapes size
	}

	/** SetFillColor: Sets the fill/background color of the buttons shape
	 * @param color The color for the new background
	*/
	void SetFillColor(sf::Color color) {
		// Exit if the shape isn't set (Avoid access violations)
		if (_shape == nullptr)
			return;
		_shape->setFillColor(color); // Assigns the given color to the shape
	}

	/** SetOutline: Set up an outline for the buttons shape
	 * @param thickness The desired thickness of the buttons outline
	 * @param color The desired color of the buttons background
	*/
	void SetOutline(float thickness, sf::Color color) {
		// Exit if the shape isn't set (Avoid access violations)
		if (_shape == nullptr)
			return;
		_shape->setOutlineColor(color); // Assign the outline color to the shape
		_shape->setOutlineThickness(thickness); // Assign the outline thickness to the shape
	}

	/** SetFont: Set the font of the text component
	 * @param font The desired font to be used for the text component
	*/
	void SetFont(sf::Font font) {
		// Exit if the text isn't set (Avoid access violations)
		if (_txtText == nullptr)
			return;

		this->font = sf::Font::Font(font); // Store the font in the class instance
		_txtText->setFont(this->font); // Assign the new font to the text component
		SetAutoCharacterSize(); // Automatically resize the character size to fit within the buttons boundaries with the new font
		SetTextPosition(); // Reset the position of the text component to the center of the buttons boundaries
	}

	/** SetText: Set a text to be displayed in the button & its color
	 * @param text Desired string to be displayed in the button
	 * @param color Desired color of the text
	*/
	void SetText(std::string text, sf::Color color) {
		// Check if the text component is a null pointer
		if (_txtText == nullptr) {
			_txtText = new sf::Text(); // Create a new sf::Text instance and assign it
			_txtText->setFont(this->font); // Set the font of the new text component
		}
			

		_txtText->setString(text); // Sets the given string in the text component
		_txtText->setFillColor(color); // Sets the given color of the text
		SetAutoCharacterSize(); // Automatically resize the character size to fit within the buttons boundaries
		SetTextPosition(); // Reset the position of the text component to the center of the buttons boundaries
	}

	/** SetText: Set a text to be displayed in the button (With default color black)
	 * @param text Desired string to be displayed in the button
	*/
	void SetText(std::string text) {
		SetText(text, sf::Color::Black); // Sets the text of the text component and set default color black
	}

	/** GetTextObject: Returns the pointer to the text component
	 * @return sf::Text The pointer to the Text component
	*/
	sf::Text * GetTextObject() {
		return _txtText;
	}

	/** GetShapeObject: Returns the pointer to the shape component
	 * @return sf::Shape The pointer to the shape
	*/
	sf::Shape * GetShapeObject() {
		return _shape;
	}

	/** GetGlobalBounds: Returns the global bounds of the shape component
	 * @return sf::FloatRect the bounds of the shape
	*/
	sf::FloatRect GetGlobalBounds() {
		// Exit if the shape isn't set (Avoid access violations)
		if (_shape == nullptr)
			return sf::FloatRect();
		return _shape->getGlobalBounds();
	}
};

/**
 * Implementation of Brick class (BETA)
 *
 * Represents a destroyable brick in the game.
*/
class Brick {
	// Private Variables
	int colorChangeStep = 7; // The gravity of the color change for blocks with multiple lifes. Bigger value = bigger change.
	char brickType; // The type of the brick (Currently there are only 2 definitions '1' & '2')
	int healthPoints; // The amount of hits it needs to destroy the brick

	/** SetHealth: Sets the current health of the brick.
	 * @param health The new health of the brick.
	*/
	void SetHealth(int health) {
		this->healthPoints = health; // Assign new health

		// Exit if the health is equals/below 0
		if (health <= 0)
			return;

		// Calculate the new color of the brick dependig on the set basecolor, health and colorChangeStep
		int r = baseColor.r - (health * (baseColor.r / colorChangeStep));
		int g = baseColor.g - (health * (baseColor.g / colorChangeStep));
		int b = baseColor.b - (health * (baseColor.b / colorChangeStep));

		// Set the bricks new background color if the rect is set
		if(rect != nullptr)
			rect->setFillColor(sf::Color::Color(r, g, b, 255));
	}


public:
	// Public Variables
	sf::RectangleShape * rect; // The shape of the brick
	sf::Color baseColor = sf::Color::White; // The base color of the brick (Represents the background color with 1 health)

	/** SetBrickType: Sets the type of the brick
	 * @param brickType The desired brick type (char).
	*/
	void SetBrickType(char brickType) {
		this->brickType = brickType; // Store the new bricktype

		// Set the baseColor & health depending on the brick type (Yes, this is not a good implementation - it would be better to store that information in a configuration/list with its own BrickType class)
		switch (brickType) {
		case 1:
			baseColor = sf::Color::Color(237, 173, 55, 255);
			SetHealth(1);
			break;
		case 2:
			baseColor = sf::Color::Color(237, 55, 55, 255); 
			SetHealth(5);
			break;
		}

	}

	/** DecreaseHealth: Decrease the blocks health by one. */
	void DecreaseHealth() {
		SetHealth(healthPoints - 1); // This will reduce the health by 1 and set the new background color
	}

	/** Brick Constructor
	 * @param width The desired width of the brick
	 * @param height The desired height of the brick
	 * @param brickType The type of the brick
	 * @param pos The global position of the brick
	 * @return New instance of the brick
	*/
	Brick(float width, float height, char brickType, sf::Vector2f pos) {
		rect = new sf::RectangleShape(sf::Vector2f(width, height)); // Creates a new instance of RectangleShape and assigns its pointer
		rect->setPosition(pos); // Sets the position of the rectangle
		SetBrickType(brickType); // Set the brick type and initialises health & basecolor
	}

	/** Brick Deconstructor: Called when the instance is destroyed. */
	~Brick() {
		delete rect; // Delete the rectangle from memory
	}

	/** IsAlive
	* @return bool Returns if the brick is alive or not depending on its health.
	*/
	bool IsAlive() {
		return healthPoints > 0;
	}
};

#pragma region UNFINISHED WORK; DELETE
/** :>>> INFORMATION ABOUT THIS REGION <<<:
 * I wanted to clean up the code & split everything up into classes to make it more neat.
 * I stopped working on it since it would have taken up too much time (Time which I currently don't have).
 * However, I left it in here anyways.
*/

/*
class BrickType {
public:
	char brickType;
	sf::Color baseColor;
	int health;
};

// TOdo
class PlayArea {
public:
	sf::RectangleShape playArea;
};

// TODo
class Paddle {
public:
	sf::RectangleShape paddle;
};

//TOdo
class Ball {
	sf::Vector2f* currentBallMovement;
};

class Level {
	PlayArea playArea;
	Paddle paddle;
	std::vector<Ball> activeBalls;
	std::vector<std::vector<char>> grid;
	std::vector<std::vector<Brick*>> activeBricks;

	// Grid / Bricks Vars
	int gridSizeX = 0;
	int gridSizeY = 0;
	float brickWidth = 0;
	float brickHeight = 0;
	float brickSpacing = 10;


	Brick * CreateAndGetBrick(unsigned int x, unsigned int y) {
		float vertPos = playArea.playArea.getPosition().y + (brickSpacing / 2) + (y) * (brickHeight + brickSpacing);
		float horPos = playArea.playArea.getPosition().x + (brickSpacing / 2) + (x) * (brickWidth + brickSpacing);
		Brick * brick = new Brick(brickWidth, brickHeight, grid[y][x], sf::Vector2f(horPos, vertPos));

		return brick;
	}

	void CreateBlockGrid() {
		if (gridSizeX == 0 || gridSizeY == 0)
			return;

		// Determine grid width & height depending on the size of the playArea & amount of bricks (+1 Brick width/height empty space left/right/top/bottom)
		brickWidth = playArea.playArea.getLocalBounds().width / gridSizeX - brickSpacing;
		brickHeight = (playArea.playArea.getLocalBounds().height - brickPaddleMinSpace - (3 * paddle.paddle.getLocalBounds().height)) / gridSizeY - brickSpacing;

		for (int y = 0; y < gridSizeY; y++) {
			for (int x = 0; x < gridSizeX; x++) {
				activeBricks[y][x] = CreateAndGetBrick(x, y);
			}
		}
	}
public:
	// Ball Settings
	const int initialBallCount = 1;
	float maxBallSpeed = 150;
	float defaultBallSpeed = 100;
	// Paddle Settings
	float paddleSpeed = 200;
	float brickPaddleMinSpace = 150;

	Level(unsigned int gridSizeX, unsigned int gridSizeY, unsigned int initialBallCount) : initialBallCount(initialBallCount){
		this->gridSizeX = gridSizeX;
		this->gridSizeY = gridSizeY;
		grid.resize(gridSizeY, std::vector<char>(gridSizeX,'0'));
		CreateBlockGrid();
	}

	Level(std::vector<std::vector<char>> pGrid, int initialBallCount) : grid(pGrid), initialBallCount(initialBallCount) {
		gridSizeY = std::size(pGrid);
		if(gridSizeY > 0)
			gridSizeX = std::size(pGrid[0]);
		CreateBlockGrid();
	}

	Level(std::vector<std::vector<char>> pGrid) : grid(pGrid), initialBallCount(1) {
		gridSizeY = std::size(pGrid);
		if (gridSizeY > 0)
			gridSizeX = std::size(pGrid[0]);
		CreateBlockGrid();
	}

	Brick * GetBrickAtCoord(unsigned int x, unsigned int y) {
		if (x >= gridSizeX || y >= gridSizeY)
			return nullptr;

		return activeBricks[y][x];
	}

	void SetCharValueAtCoord(char brickType, unsigned int x, unsigned int y) {
		if (x >= gridSizeX || y >= gridSizeY)
			return;
		grid[y][x] = brickType;
		if (activeBricks[x][y] != nullptr)
			delete activeBricks[x][y];
		activeBricks[x][y] = CreateAndGetBrick(x,y);
	}

	int GetInitialBallCount() {
		return initialBallCount;
	}


	void BuildLevel() {
		CreateBlockGrid();
	}

};
*/

#pragma endregion

#pragma endregion

#pragma region Function Declarations
// @Region: Declaring all functions so the order of the function definitions don't matter further down
void WindowLifeCycle();
void MovePaddle();
bool IsInBounds(sf::FloatRect objectBounds, sf::FloatRect areaBounds);
void UpdateText();
void InitGrid();
void SpawnBricks();
void SpawnBrick(int verticalPos, int horizontalPos);
void MoveBall(int ballNo);
void RemoveBall(int ballNo);
void AddPoints(int amount);
void ReduceLife();
void RemoveBrick(Brick *brick);
void InitVars();
void RenderGame();
void HandleMovements();
void RenderMainMenu();
void SaveHighscores();
bool LoadHighscores();
void RenderHighscoreList();
void UI_UpdateHighscoreEntries();
void SortHighscoreList();
void InitGame();
double GetHorizontalAngleBetween(sf::Vector2f objectOne, sf::Vector2f objectTwo);
sf::Vector2i GetActiveBrickArrayLocation(Brick *brick);
sf::Vector2f IsObjectWithinAreaVector(sf::FloatRect objectBounds, sf::FloatRect areaBounds);
sf::Vector2f GetCollisionSideVector(sf::FloatRect objectOne, sf::FloatRect collider);
sf::CircleShape* SpawnBall();

#pragma endregion

#pragma region Global Variables
//// General Components/Settings
sf::RenderWindow renderWindow;
sf::Font generalFont;
float windowWidth = 800; // Width of the window
float windowHeight = 540; // Height of the window

std::string exeDir = "C:\\Users\\lucat\\Documents\\gitlocal\\SAEGAD173\\19T2-GAD173-Brief1-Taennler-1015901\\x64\\Debug\\"; // Directory of the exe file (Not a nice solution)
std::string fontName = "Radiant.ttf"; // Font retrieved from https://www.dafont.com/

std::vector<int> highscores; // Stores the highscores
sf::Vector2f mousePos; // Stores the current position of the mouse pointer (Used for buttons)

bool gamePause = true; // State: Defines if the game is currently active
bool showHighscoreList = false; // State: Defines if the Highscore List View is shown or not
bool showMainMenu = true; // State: Defines if the Main Menu View is shown or not
float deltaTime = 0; // Stores the delta time (Used for smoothing & stability of framerate)

//// Game Components/Settings

sf::RectangleShape playArea; // Boundaries for the paddle, bricks and balls
sf::RectangleShape paddle; // Paddle component

sf::Text txtLifes; // Text component displaying the lifes in the current game/level
sf::Text txtPoints; // Text component displaying the points in the current game/level
sf::RectangleShape boxPressKeyToSpawnBall; // "Panel" shown when there is no active ball in the current game/level
sf::Text txtPressKeyToSpawnBall; // Text component with instructions to spawn ball shown when there is no active ball in the current game/level

int lifeCount = 3; // Remaining lifes in the current game/level
int pointCount = 0; // Received points in the current game/level
int brickPointValue = 1; // Default points received for hitting a brick
int pointMultiplier = 0; // Multiplier for points - Increased with every brick hit / Resettet on paddle hit

float ballSize = 5; // Size of the ball in pixels
sf::Vector2f ballSpawnPoint; // The default location where new balls will be spawned
std::vector<sf::CircleShape*> balls; // Vector-list storing pointers to the active balls in the current game/level
std::vector<sf::Vector2f*> curBallMoveVectors; // Vector-list storing pointers to the current movement vectors of the active balls (They are connected by their index)

float brickWidth = 0; // Width of a brick (Dynamically calculated)
float brickHeight = 0; // Height of a brick (Dynamically calculated)
float brickSpacing = 5; // The spacing between the bricks (Affects brick size)
float maxBallSpeed = 150; // The maximum speed of the ball (pixel/second)
float defaultBallSpeed = 100; // The default speed of the ball (pixels/second)
float paddleSpeed = 200; // The speed of the paddle (pixels/second)
float brickPaddleMinSpace = 150; // The minimum distance in pixels between the lowest brick and the paddle (Affects brick size)

float wallColCheckCooldown = 0.1; // Seconds
float brickColCheckCooldown = 0.01; // Seconds
float paddleColCheckCooldown = 0.1; // Seconds
float curWallColCheckCooldown = 0; // Cooldown timer for brick collision checks
float curBrickColCheckCooldown = 0; // Cooldown timer for brick collision checks
float curPaddleColCheckCooldown = 0; // Cooldown timer for paddle collision checks

//// Main Menu View Components
sf::Text txtLevelSelection;
std::vector<Button*> menuButtons; // Stores buttons contained in the Main Menu View
sf::RectangleShape menuBackground(sf::Vector2f(windowWidth, windowHeight)); // Background panel of the MainMenu View

//// Highscore View Components
sf::Text txtHighscore; // Title (Text Component) of Highscore View
std::vector<Button*> highscoreButtons; // Stores buttons contained in the Highscore View
std::vector<sf::Text*> txtHighscoreEntries; // Stores text components of the highscore entries contained in the Highscore View

//// Grid / Levels
const int gridHorizontalBricks = 11; // # of maximum bricks in horizontal axis (Currently, all levels need to work with this gridsize)
const int gridVerticalBricks = 11; // # of maximum bricks in vertical axis (Currently, all levels need to work with this gridsize)
// Configuration for Level 1
const std::vector<std::vector<char>> level1Grid = {
		{0,0,0,0,0,1,0,0,0,0,0},
		{0,0,0,0,1,1,1,0,0,0,0},
		{0,0,0,1,1,1,1,1,0,0,0},
		{0,0,1,1,1,1,1,1,1,0,0},
		{0,1,1,1,1,1,1,1,1,1,0},
		{2,1,1,1,1,1,1,1,1,1,2},
		{0,2,1,1,1,1,1,1,1,2,0},
		{0,0,2,1,1,1,1,1,2,0,0},
		{0,0,0,2,1,1,1,2,0,0,0},
		{0,0,0,0,2,1,2,0,0,0,0},
		{0,0,0,0,0,2,0,0,0,0,0}
};

// Configuration for Level 2
const std::vector<std::vector<char>> level2Grid = {
		{0,0,0,0,0,0,0,0,0,0,0},
		{1,0,1,0,1,1,1,0,1,0,1},
		{2,1,2,1,2,1,2,1,2,1,2},
		{0,2,1,2,1,2,1,2,1,2,0},
		{0,1,1,1,1,1,1,1,1,1,0},
		{2,1,2,1,1,1,1,1,2,1,2},
		{0,2,1,2,1,1,1,2,1,2,0},
		{0,0,2,1,2,1,2,1,2,0,0},
		{0,0,0,2,1,2,1,2,0,0,0},
		{0,2,0,0,0,0,0,0,0,2,0},
		{0,0,0,0,0,0,0,0,0,0,0}
};

// Configuration for Level 3
const std::vector<std::vector<char>> level3Grid = {
		{1,1,1,1,1,1,1,1,1,1,1},
		{1,0,1,0,1,1,1,0,1,0,1},
		{2,2,2,2,2,2,2,2,2,2,2},
		{0,2,1,1,1,1,1,1,1,2,0},
		{0,0,2,1,1,1,1,1,2,0,0},
		{0,0,0,2,1,1,1,2,0,0,0},
		{0,0,0,0,2,1,2,0,0,0,0},
		{0,0,0,0,0,2,0,0,0,0,0},
		{1,0,0,1,1,1,1,1,1,1,1},
		{2,0,0,1,2,1,2,1,2,1,2},
		{0,0,0,2,2,2,2,2,0,0,0}
};

std::vector<std::vector<char>> activeGrid; // Grid/level for the currently active game
Brick * activeBricks[gridVerticalBricks][gridHorizontalBricks]; // Stores pointers to the bricks of the currently active game/level
#pragma endregion

#pragma region General Logic
/**
 * WindowLifeCycle: Initializes everything & contains the "gameloop", running until the application is exited
*/
void WindowLifeCycle() {
	LoadHighscores(); // Loads highscores from a textfile
	InitVars(); // Initializes all components/values (Viewers discretion advised - Very ugly in the inside)
	sf::Clock Clock; // Initialize Clock component - will be used to calculate deltaTime

	// Application Loop - iterates while the window is open.
	while (renderWindow.isOpen())
	{
		deltaTime = Clock.restart().asSeconds(); // Calculate deltaTime. Used for smoothing.

		// Handle all events
		sf::Event event;
		while (renderWindow.pollEvent(event))
		{
			switch (event.type) {
				// Event received when the window is requested to be closed
			case sf::Event::Closed: {
				SaveHighscores(); // Saves highscore list to textfile
				renderWindow.close(); // Close window (Breaks the "Application loop")
				break;
			}

									// Event received when the mouse was moved
			case sf::Event::MouseMoved: {
				sf::Vector2i mousePosInt = sf::Mouse::getPosition(renderWindow); // Get the current coordinates of the mouse pointer
				mousePos.x = (float)mousePosInt.x; // Store mouse x position in global variable (&cast to float)
				mousePos.y = (float)mousePosInt.y; // Store mouse y position in global variable (&cast to float)

				// Check if the highscore list view is active (To trigger button events)
				if (showHighscoreList) {
					// Iterates through all buttons of the highscore list view
					for (Button* b : highscoreButtons) {
						// Checks if the button is active & if the current mouse pointer position is within the bounds of the button
						if (b->isActive && b->GetGlobalBounds().contains(mousePos)) {
							b->SetMouseEnter(); // Notifies the button that the mouse entered his bounds
						}
						else if (b->isActive) {
							b->SetMouseExit(); // Notifies the button that the mouse exited his bounds
						}
					}
				}

				// Check if the main menu view is active (To trigger button events)
				if (showMainMenu) {
					// Iterates through all buttons of the main menu view
					for (Button* b : menuButtons) {
						// Checks if the button is active & if the current mouse pointer position is within the bounds of the button
						if (b->isActive && b->GetGlobalBounds().contains(mousePos)) {
							b->SetMouseEnter(); // Notifies the button that the mouse entered his bounds
						}
						else if (b->isActive) {
							b->SetMouseExit(); // Notifies the button that the mouse exited his bounds
						}
					}
				}
				break;
			}

										// Event received when the mouse button was pressed
			case sf::Event::MouseButtonPressed: {
				// Check if the highscore list view is active (To trigger button events)
				if (showHighscoreList) {
					// Iterates through all buttons of the highscore list view
					for (Button* b : highscoreButtons) {
						// Checks if the button is active & if the current mouse pointer position is within the bounds of the button
						if (b->isActive && b->GetGlobalBounds().contains(mousePos)) {
							b->SetButtonPressed(); // Notifies the button that he is pressed down
						}
					}
				}
				// Check if the main menu view is active (To trigger button events)
				if (showMainMenu) {
					// Iterates through all buttons of the main menu view
					for (Button* b : menuButtons) {
						// Checks if the button is active & if the current mouse pointer position is within the bounds of the button
						if (b->isActive && b->GetGlobalBounds().contains(mousePos)) {
							b->SetButtonPressed(); // Notifies the button that he is pressed down
						}
					}
				}
				break;
			}

												// Event received when the mouse button was released
			case sf::Event::MouseButtonReleased: {
				// Check if the highscore list view is active (To trigger button events)
				if (showHighscoreList) {
					// Iterates through all buttons of the highscore list view
					for (Button* b : highscoreButtons) {
						// Checks if the button is active
						if (b->isActive)
							b->SetButtonReleased(); // Notifies the button that the mouse button was released
					}
				}
				// Check if the main menu view is active (To trigger button events)
				if (showMainMenu) {
					// Iterates through all buttons of the main menu view
					for (Button* b : menuButtons) {
						// Checks if the button is active
						if (b->isActive)
							b->SetButtonReleased(); // Notifies the button that the mouse button was released
					}
				}
				break;
			}

			}
		}

		// Delete everything that was drawn in the window
		renderWindow.clear();

		// Check if the game/level is active
		if (!gamePause) {
			HandleMovements(); // This methods handles all movements of the paddle and balls
			RenderGame(); // Renders all neccessary elements of the active game
		}

		// Check if the Main Menu View is active
		if (showMainMenu) {
			RenderMainMenu(); // Renders all components of the main menu view
		}

		// Check if the Highscore List View is active
		if (showHighscoreList) {
			RenderHighscoreList(); // Renders all components of the highscore list view
		}

		// Display the drawed contents
		renderWindow.display();
	}
}


/** SortHighscoreList: Sorts the highscore list from highest to lowest */
void SortHighscoreList() {
	std::sort(highscores.begin(), highscores.end()); // Sort the array lowest to highest value
	std::reverse(highscores.begin(), highscores.end()); // Invert the array to have the highest value first
}

/** SaveHighscores: Save the highscore list to a text file.
 * Each highscore is saved on a line, separated by line breaks.
*/
void SaveHighscores() {
	// Open an Output File Stream
	std::ofstream highscoreFile;
	// Open the text file (Creates a new one if it doesn't exist
	highscoreFile.open("breakout_highscores.txt");

	for (int i = 0; i < highscores.size(); i++) {
		// Check if the highscore is greater than 0 - we don't want to save zero values
		if (highscores[i] > 0) {
			// Write the highscore into the file
			highscoreFile << std::to_string(highscores[i]);
			// Check if this isn't the last entry in the highscore array
			if (i != highscores.size() - 1)
				highscoreFile << "\n"; // Add a line break
		}
	}

	// Close the File Stream
	highscoreFile.close();
}

/** LoadHighscores: Load highscores from a textfile. */
bool LoadHighscores() {
	std::string line;
	// Open an input file stream for the specified text file
	std::ifstream highscoreFile("breakout_highscores.txt");
	// Ccheck if the file opening was successful
	if (highscoreFile.is_open())
	{
		// Iterating through all lines ("\n")
		while (std::getline(highscoreFile, line))
		{
			// Convert the current line to an int and push it into the highscore list
			highscores.push_back(std::stoi(line));
		}
		// Close the Input File Stream
		highscoreFile.close();
		// Sort the highscore list highest to lowest
		SortHighscoreList();
		// Import Highscores successful, return true
		return true;
	}
	// Return false; Couldn't open file
	return false;
}

/**
 * InitVars: Initialised all components / variables.
 * Needs to be called when the application is started.
*/
void InitVars() {
	renderWindow.create(sf::VideoMode(windowWidth, windowHeight), "Breakout - GAD173 - Brief 1"); // Initialize the window

	// Try to load the main font
	if (!generalFont.loadFromFile(exeDir + fontName))
	{
		std::cout << "Couldn't load font!" << std::endl;
	}

#pragma region Highscore List View Inits
	// Init Back to Menu Button
	Button* btnHSBack = new Button(); // Using new to make sure it wont get deleted at the end of this function & to have a unique pointer
	sf::RectangleShape * rectBtnHSBack = new sf::RectangleShape(sf::Vector2f(windowWidth - 40, 80));
	rectBtnHSBack->setFillColor(sf::Color::Color(219, 219, 219, 255));
	rectBtnHSBack->setOutlineThickness(10);
	rectBtnHSBack->setOutlineColor(sf::Color::Black);
	btnHSBack->SetShape(rectBtnHSBack);
	btnHSBack->SetFont(generalFont);
	btnHSBack->SetText("BACK TO MENU");
	btnHSBack->SetPosition(((windowWidth / 2) - btnHSBack->GetGlobalBounds().width / 2), 10);

	// Create a button click lambda function which will take the player to the main menu view
	auto FnClick_HSBack = []() {
		showHighscoreList = false;
		showMainMenu = true;
		gamePause = true;
	};
	btnHSBack->AddButtonReleasedFunc(FnClick_HSBack); // Assign function to the button

	// Create button mouse over lambda func which changes the BG color of the button
	auto FnMOver_HSBack = [btnHSBack]() {
		btnHSBack->SetFillColor(sf::Color::Color(150, 150, 150, 255));
	};
	btnHSBack->AddMouseEnterFunc(FnMOver_HSBack); // Assign fun to btn

	// Create button mouse exit lambda func which resets the BG color of the button
	auto FnMExit_HSBack = [btnHSBack]() {
		btnHSBack->SetFillColor(sf::Color::Color(219, 219, 219, 255));
	};
	btnHSBack->AddMouseExitFunc(FnMExit_HSBack); // Assign fun to btn

	// Add the button to the highscore buttons list (Reference required for button event updates & rendering)
	highscoreButtons.push_back(btnHSBack);

	// Init Highscore title text component
	txtHighscore.setFont(generalFont);
	txtHighscore.setCharacterSize(24);
	txtHighscore.setFillColor(sf::Color::Color(219, 219, 219, 255));
	txtHighscore.setString("Highscores");
	txtHighscore.setPosition(((windowWidth / 2) - txtHighscore.getGlobalBounds().width / 2), btnHSBack->GetGlobalBounds().top + btnHSBack->GetGlobalBounds().height + 10);

	// Create/Update the highscore entry text components in the highscore list view
	UI_UpdateHighscoreEntries();
#pragma endregion

#pragma region Main Menu View Inits
	// Set the background color of the main menu
	menuBackground.setFillColor(sf::Color::Color(33, 33, 33, 255));

	// Init Level Selection (Title) Text Component
	txtLevelSelection.setFont(generalFont);
	txtLevelSelection.setCharacterSize(24);
	txtLevelSelection.setPosition(((windowWidth / 2) - txtLevelSelection.getGlobalBounds().width / 2), 10);
	txtLevelSelection.setFillColor(sf::Color::Color(219, 219, 219, 255));
	txtLevelSelection.setString("Select Level");

	// Create Level1 Button (Level Selection)
	Button* btnLevel1 = new Button(); // Using new to make sure it wont get deleted at the end of this function & to have a unique pointer
	sf::RectangleShape * rectBtnLevel1 = new sf::RectangleShape(sf::Vector2f(100, 100));
	rectBtnLevel1->setFillColor(sf::Color::Color(220, 220, 220, 255));
	rectBtnLevel1->setOutlineThickness(10);
	rectBtnLevel1->setOutlineColor(sf::Color::Color(33, 33, 33, 255));
	btnLevel1->SetShape(rectBtnLevel1);
	btnLevel1->SetPosition((windowWidth / 2 - 190), 60);//20
	btnLevel1->SetFont(generalFont);
	btnLevel1->SetText("1");

	// Add the button to the main menu buttons list (Reference required for button event updates & rendering)
	menuButtons.push_back(btnLevel1);

	// Create Level2 Button (Level Selection)
	Button* btnLevel2 = new Button(); // Using new to make sure it wont get deleted at the end of this function & to have a unique pointer
	sf::RectangleShape * rectBtnLevel2 = new sf::RectangleShape(sf::Vector2f(100, 100));
	rectBtnLevel2->setFillColor(sf::Color::Color(220, 220, 220, 255));
	rectBtnLevel2->setOutlineThickness(10);
	rectBtnLevel2->setOutlineColor(sf::Color::Color(33, 33, 33, 255));
	btnLevel2->SetShape(rectBtnLevel2);
	btnLevel2->SetPosition((windowWidth / 2 - 60), 60);//140
	btnLevel2->SetFont(generalFont);
	btnLevel2->SetText("2");

	// Add the button to the main menu buttons list (Reference required for button event updates & rendering)
	menuButtons.push_back(btnLevel2);

	// Create Level3 Button (Level Selection)
	Button* btnLevel3 = new Button(); // Using new to make sure it wont get deleted at the end of this function & to have a unique pointer
	sf::RectangleShape * rectBtnLevel3 = new sf::RectangleShape(sf::Vector2f(100, 100));
	rectBtnLevel3->setFillColor(sf::Color::Color(220, 220, 220, 255));
	rectBtnLevel3->setOutlineThickness(10);
	rectBtnLevel3->setOutlineColor(sf::Color::Color(33, 33, 33, 255));
	btnLevel3->SetShape(rectBtnLevel3);
	btnLevel3->SetPosition((windowWidth / 2 + 70), 60);//260
	btnLevel3->SetFont(generalFont);
	btnLevel3->SetText("3");

	// Add the button to the main menu buttons list (Reference required for button event updates & rendering)
	menuButtons.push_back(btnLevel3);

	// Level1 Button: Create Button Event Functions
	// --Click function
	auto FnClick_Level1 = [btnLevel1, btnLevel2, btnLevel3]() { // Pass button pointers
		btnLevel1->SetOutline(10, sf::Color::Green); // Set the outline of the level 1 button to green to mark current level selection
		btnLevel2->SetOutline(10, sf::Color::Color(33, 33, 33, 255)); // Reset outline of the other buttons
		btnLevel3->SetOutline(10, sf::Color::Color(33, 33, 33, 255)); // Reset outline of the other buttons
		activeGrid = level1Grid; // Sets the level selection to level 1
	};
	btnLevel1->AddButtonReleasedFunc(FnClick_Level1);
	// --Mouse Over function
	auto FnMOver_Level1 = [btnLevel1]() {
		btnLevel1->SetFillColor(sf::Color::Green); // Set button BG to green on mouse over
	};
	btnLevel1->AddMouseEnterFunc(FnMOver_Level1);
	// --Mouse Exit function
	auto FnMExit_Level1 = [btnLevel1]() {
		btnLevel1->SetFillColor(sf::Color::Color(220, 220, 220, 255)); // Reset button BG on mouse exit
	};
	btnLevel1->AddMouseExitFunc(FnMExit_Level1);

	// Level2 Button: Create Button Event Functions
	// --Click function
	auto FnClick_Level2 = [btnLevel1, btnLevel2, btnLevel3]() {// Pass button pointers
		btnLevel1->SetOutline(10, sf::Color::Color(33, 33, 33, 255)); // Reset outline of the other buttons
		btnLevel2->SetOutline(10, sf::Color::Green);// Set the outline of the level 2  button to green to mark current level selection
		btnLevel3->SetOutline(10, sf::Color::Color(33, 33, 33, 255)); // Reset outline of the other buttons
		activeGrid = level2Grid; // Sets the level selection to level 1
	};
	btnLevel2->AddButtonReleasedFunc(FnClick_Level2);
	// --Mouse Over function
	auto FnMOver_Level2 = [btnLevel2]() {
		btnLevel2->SetFillColor(sf::Color::Green); // Set button BG to green on mouse over
	};
	btnLevel2->AddMouseEnterFunc(FnMOver_Level2);
	// --Mouse Exit function
	auto FnMExit_Level2 = [btnLevel2]() {
		btnLevel2->SetFillColor(sf::Color::Color(220, 220, 220, 255)); // Reset button BG on mouse exit
	};
	btnLevel2->AddMouseExitFunc(FnMExit_Level2);

	// Level2 Button: Create Button Event Functions
	// --Click function
	auto FnClick_Level3 = [btnLevel1, btnLevel2, btnLevel3]() {// Pass button pointers
		btnLevel1->SetOutline(10, sf::Color::Color(33, 33, 33, 255)); // Reset outline of the other buttons
		btnLevel2->SetOutline(10, sf::Color::Color(33, 33, 33, 255)); // Reset outline of the other buttons
		btnLevel3->SetOutline(10, sf::Color::Green);// Set the outline of the level 3  button to green to mark current level selection
		activeGrid = level3Grid; // Sets the level selection to level 1
	};
	btnLevel3->AddButtonReleasedFunc(FnClick_Level3);
	// --Mouse Over function
	auto FnMOver_Level3 = [btnLevel3]() {
		btnLevel3->SetFillColor(sf::Color::Green); // Set button BG to green on mouse over
	};
	btnLevel3->AddMouseEnterFunc(FnMOver_Level3);
	// --Mouse Exit function
	auto FnMExit_Level3 = [btnLevel3]() {
		btnLevel3->SetFillColor(sf::Color::Color(220, 220, 220, 255)); // Reset button BG on mouse exit
	};
	btnLevel3->AddMouseExitFunc(FnMExit_Level3);


	// Create Start Game Button
	Button* btnStartGame = new Button(); // Using new to make sure it wont get deleted at the end of this function & to have a unique pointer
	sf::RectangleShape * rectBtnStartGame = new sf::RectangleShape(sf::Vector2f(windowWidth - 40, 80));
	rectBtnStartGame->setFillColor(sf::Color::Color(255, 255, 255, 255));
	rectBtnStartGame->setOutlineThickness(10);
	rectBtnStartGame->setOutlineColor(sf::Color::Green);
	btnStartGame->SetShape(rectBtnStartGame);
	btnStartGame->SetPosition(20, 190);
	btnStartGame->SetFont(generalFont);
	btnStartGame->SetText("START");
	// --Click function
	auto FnClick_StartGame = []() {
		showMainMenu = false; // deactivates main menu
		InitGame(); // Initialize game/level
		gamePause = false; // activates the game view

	};
	btnStartGame->AddButtonReleasedFunc(FnClick_StartGame);
	// --Mouse Over function
	auto FnMOver_StartGame = [btnStartGame]() {
		btnStartGame->SetFillColor(sf::Color::Green); // Set background of button green on mouse over
	};
	btnStartGame->AddMouseEnterFunc(FnMOver_StartGame);
	// --Mouse Exit function
	auto FnMExit_StartGame = [btnStartGame]() {
		btnStartGame->SetFillColor(sf::Color::Color(255, 255, 255, 255)); // Reset background on mouse exit
	};
	btnStartGame->AddMouseExitFunc(FnMExit_StartGame);

	menuButtons.push_back(btnStartGame);

	// Create Highscore Button
	Button* btnToHighscores = new Button(); // Using new to make sure it wont get deleted at the end of this function & to have a unique pointer
	sf::RectangleShape * rectBtnToHighscores = new sf::RectangleShape(sf::Vector2f(windowWidth - 40, 80));
	rectBtnToHighscores->setFillColor(sf::Color::Color(219, 219, 219, 255));
	rectBtnToHighscores->setOutlineThickness(10);
	rectBtnToHighscores->setOutlineColor(sf::Color::Black);
	btnToHighscores->SetShape(rectBtnToHighscores);
	btnToHighscores->SetPosition(20, 300);
	btnToHighscores->SetFont(generalFont);
	btnToHighscores->SetText("HIGHSCORES");
	// --Click function
	auto FnClick_ToHighscores = []() {
		showMainMenu = false; // Deactivate Main Menu View
		gamePause = true; // Make sure game view is disabled
		showHighscoreList = true; // Acticate Highscore List View
	};
	btnToHighscores->AddButtonReleasedFunc(FnClick_ToHighscores);
	// --Mouse Over function
	auto FnMOver_ToHighscores = [btnToHighscores]() {
		btnToHighscores->SetFillColor(sf::Color::Color(150, 150, 150, 255)); // Set background of button green on mouse over
	};
	btnToHighscores->AddMouseEnterFunc(FnMOver_ToHighscores);
	// --Mouse Exit function
	auto FnMExit_ToHighscores = [btnToHighscores]() {
		btnToHighscores->SetFillColor(sf::Color::Color(219, 219, 219, 255)); // Reset background on mouse exit
	};
	btnToHighscores->AddMouseExitFunc(FnMExit_ToHighscores);

	menuButtons.push_back(btnToHighscores);

	// Create Quit Game Button
	Button* btnQuitGame = new Button(); // Using new to make sure it wont get deleted at the end of this function & to have a unique pointer
	sf::RectangleShape * rectBtnQuiteGame = new sf::RectangleShape(sf::Vector2f(windowWidth - 40, 80));
	rectBtnQuiteGame->setFillColor(sf::Color::Red);
	rectBtnQuiteGame->setOutlineThickness(10);
	rectBtnQuiteGame->setOutlineColor(sf::Color::Color(255, 66, 66, 255));
	btnQuitGame->SetShape(rectBtnQuiteGame);
	btnQuitGame->SetPosition(20, 410);
	btnQuitGame->SetFont(generalFont);
	btnQuitGame->SetText("QUIT");
	// --Click function
	auto FnClick_QuitGame = []() {
		renderWindow.close(); // Close window -> Breaks application loop 
		exit(0); // Exit Application
	};
	btnQuitGame->AddButtonReleasedFunc(FnClick_QuitGame);
	// --Mouse Over function
	auto FnMOver_QuitGame = [btnQuitGame]() {
		btnQuitGame->SetFillColor(sf::Color::Color(255, 66, 66, 255)); // Set background of button green on mouse over
	};
	btnQuitGame->AddMouseEnterFunc(FnMOver_QuitGame);
	// --Mouse Exit function
	auto FnMExit_QuitGame = [btnQuitGame]() {
		btnQuitGame->SetFillColor(sf::Color::Red); // Reset background on mouse exit
	};
	btnQuitGame->AddMouseExitFunc(FnMExit_QuitGame);

	menuButtons.push_back(btnQuitGame);
#pragma endregion

#pragma region Game View Inits
	///////// GAME

	// Init Text String (Required to determine an adequate position [not perfect pos tho since the string length will change with diff. numbers])
	UpdateText();

	// Init Lifes Text
	txtLifes.setFont(generalFont);
	txtLifes.setCharacterSize(24);
	txtLifes.setOrigin(txtLifes.getLocalBounds().width / 2, 0);
	txtLifes.setPosition(200, 8);
	txtLifes.setFillColor(sf::Color::Color(219, 219, 219, 255));

	// Init Points Text
	txtPoints.setFont(generalFont);
	txtPoints.setCharacterSize(24);
	txtPoints.setOrigin(txtLifes.getLocalBounds().width / 2, 0);
	txtPoints.setPosition(600, 8);
	txtPoints.setFillColor(sf::Color::Color(219, 219, 219, 255));

	// Init play area
	playArea = sf::RectangleShape(sf::Vector2f(780, 480));
	playArea.setPosition(10, 50);
	playArea.setFillColor(sf::Color::Color(219, 219, 219, 255));

	// Init paddle
	paddle = sf::RectangleShape(sf::Vector2f(50, 10));
	paddle.setPosition(playArea.getGlobalBounds().left + playArea.getLocalBounds().width / 2 - paddle.getLocalBounds().width / 2, playArea.getPosition().y + playArea.getLocalBounds().height - 2 * paddle.getLocalBounds().height);
	paddle.setFillColor(sf::Color::Black);

	// Init Ball Spawn Point
	ballSpawnPoint = sf::Vector2f(playArea.getGlobalBounds().left + (playArea.getLocalBounds().width / 2), paddle.getGlobalBounds().top - 100);

	// Init boxPressKeyToSpawnBall & co
	boxPressKeyToSpawnBall = sf::RectangleShape(sf::Vector2f(playArea.getGlobalBounds().width / 1.7, 50));
	boxPressKeyToSpawnBall.setPosition(sf::Vector2f((playArea.getGlobalBounds().left + playArea.getGlobalBounds().width) / 2 - playArea.getGlobalBounds().width / 1.7 / 2, (playArea.getGlobalBounds().top + playArea.getGlobalBounds().height) / 2 - 25));
	boxPressKeyToSpawnBall.setFillColor(sf::Color::Color(33, 33, 33, 200));

	txtPressKeyToSpawnBall.setString("Press [Space] to spawn ball.");
	txtPressKeyToSpawnBall.setFillColor(sf::Color::White);
	txtPressKeyToSpawnBall.setFont(generalFont);
	txtPressKeyToSpawnBall.setCharacterSize(20);
	txtPressKeyToSpawnBall.setPosition(sf::Vector2f((playArea.getGlobalBounds().left + playArea.getGlobalBounds().width) / 2 - txtPressKeyToSpawnBall.getGlobalBounds().width / 2, (playArea.getGlobalBounds().top + playArea.getGlobalBounds().height) / 2 - 10));
#pragma endregion

}

#pragma endregion

#pragma region Main Menu View Logic

/**
 * RenderMainMenu: Draws all components of the main menu view
*/
void RenderMainMenu() {
	renderWindow.draw(menuBackground); // Draws the background of the main menu view
	renderWindow.draw(txtLevelSelection); // Draws the text component title for the level selection

	// Iterates through all main menu view buttons
	for (Button* b : menuButtons) {
		// Checks if the shape* isn't a nullpointer and if the button is activated
		if (b->GetShapeObject() != nullptr && b->isActive) {
			renderWindow.draw(*b->GetShapeObject()); // Draws the shape component of the button
		}
		// Checks if the text* isn't a nullpointer and if the button is activated
		if (b->GetTextObject() != nullptr && b->isActive) {
			renderWindow.draw(*b->GetTextObject()); // Draws the text component of the button
		}

	}
}



#pragma endregion

#pragma region Highscore List View Logic

/**
 * RenderHighscoreList: Draws all components of the highscore list view
*/
void RenderHighscoreList() {
	renderWindow.draw(menuBackground);
	renderWindow.draw(txtHighscore);

	// Loop through all highscore entry text components & draw them
	for (sf::Text* highscoreEntry : txtHighscoreEntries) {
		// Check if the pointer to the text isn't a nullptr (To avoid access violations)
		if (highscoreEntry != nullptr) {
			renderWindow.draw(*highscoreEntry); // Draw the text component
		}
	}

	// Iterates through all highscore view buttons
	for (Button* b : highscoreButtons) {
		// Checks if the shape* isn't a nullpointer and if the button is activated
		if (b->GetShapeObject() != nullptr && b->isActive) {
			renderWindow.draw(*b->GetShapeObject()); // Draws the shape component of the button
		}
		// Checks if the text* isn't a nullpointer and if the button is activated
		if (b->GetTextObject() != nullptr && b->isActive) {
			renderWindow.draw(*b->GetTextObject()); // Draws the text component of the button
		}

	}
}

/**
 * UI_UpdateHighscoreEntries: Updates the text components / highscore entries in the highscore list view
 * This function needs to be called when a highscore is added, removed or after highscores have been imported from an external source.
*/
void UI_UpdateHighscoreEntries() {
	// Defines the vertical global position of the next element; Initially set from the titles position
	float HS_yOffset = txtHighscore.getGlobalBounds().top + txtHighscore.getLocalBounds().height + 5;

	// Delete Old Entries
	for (sf::Text* entry : txtHighscoreEntries) {
		delete entry;
	}
	txtHighscoreEntries.clear(); // Empties list

	// Iterate through highscores & create an entry for each element
	for (int score : highscores) {
		sf::Text * highscoreEntry = new sf::Text(std::to_string(score), generalFont, 24); // Create new sf::text
		highscoreEntry->setPosition(((windowWidth / 2) - highscoreEntry->getGlobalBounds().width / 2), HS_yOffset); // Set the position of the entry
		HS_yOffset += highscoreEntry->getLocalBounds().height + 5; // Increase the offset for the next entry to use
		txtHighscoreEntries.push_back(highscoreEntry); // Add the text component to the txtHighscoreEntries list (Neccessary since we need a ref. to delete them in the next update)
	}

}

#pragma endregion

#pragma region Game Logic

/**
 * RenderGame: Draws all components of the active game
*/
void RenderGame() {
	renderWindow.draw(txtLifes); // Draw text component which displays the remaining lifes
	renderWindow.draw(txtPoints); // Draw text component which displays the gained points
	renderWindow.draw(playArea);
	renderWindow.draw(paddle);

	// Render Active Bricks
	if (std::size(activeBricks) > 0) {
		for (int i = 0; i < std::size(activeBricks); i++) {
			for (int y = 0; y < std::size(activeBricks[i]); y++) {
				if (activeBricks[i][y] != nullptr) {
					renderWindow.draw(*activeBricks[i][y]->rect); // Render the brick
				}
			}
		}
	}

	// Render Active Balls
	if (balls.size() > 0) {
		for (sf::CircleShape *cs : balls) {
			if (cs != nullptr)
				renderWindow.draw(*cs); // Render the ball
		}
	}
	else {// No active balls in game. Show a box & a text giving instructions on how to spawn a new ball/
		renderWindow.draw(boxPressKeyToSpawnBall);
		renderWindow.draw(txtPressKeyToSpawnBall);

		// Checks if the Space-bar is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			SpawnBall(); // This function will spawn a new ball at the default ball spawn location
		}

	}
}

/**
 * InitGame: Initializes a new game/level, resets points/lifecount & initialized the brick grid
*/
void InitGame() {
	// Default to level 1 if no active level is set
	if (activeGrid.size() == 0) {
		activeGrid = level1Grid;
	}
	pointCount = 0; // Reset point counter
	lifeCount = 3; // Reset remaining lifes
	UpdateText(); // This function updates the text components of the points/lifes


	// This function will initialize the grid from the template (create all bricks etc.)
	InitGrid();
}

/**
 * HandleMovements: Handles all movement in the active game (Paddle & Balls)
*/
void HandleMovements() {
	MovePaddle(); // This function will handle all movements, user inputs, collision checks etc for the paddle

	// Handle active balls
	for (int i = 0; i < std::size(balls); i++) {
		if (balls[i] != nullptr) {
			MoveBall(i); // This function will handle all movements, collisions etc for the ball
		}
	}
}

/** InitGrid: Dynamicly calculates the bricks width/heights & spawns Bricks. */
void InitGrid() {
	// Get the longth of the longest horizontal grid row
	int maxBricksHor = 0;
	for (int i = 0; i < activeGrid.size(); i++)
	{
		int arrSize = activeGrid[i].size();
		if (arrSize > maxBricksHor) {
			maxBricksHor = arrSize;
		}
	}

	// Determine grid width & height depending on the size of the playArea, brick spacing, brickPaddleMinSpace & amount of bricks
	brickWidth = playArea.getLocalBounds().width / (maxBricksHor)-brickSpacing;
	brickHeight = (playArea.getLocalBounds().height - brickPaddleMinSpace - (3 * paddle.getLocalBounds().height)) / (activeGrid.size()) - brickSpacing;

	SpawnBricks(); // This function will iterate through the active grid template & spawn the bricks.
}

/** SpawnBricks: Iterates through the active grid template and spawns bricks */
void SpawnBricks() {
	// Iterate through multidim. vector (1. level: vertical brick rows, 2. level: horizontal brick columns)
	for (int verti = 0; verti < activeGrid.size(); verti++)
	{
		for (int hori = 0; hori < activeGrid[verti].size(); hori++) {
			if (activeGrid[verti][hori] > 0)
				SpawnBrick(verti, hori); // This call will spawn a brick at the given vertical/horizontal spot
		}
	}
}

/** SpawnBrick: Spawns & initialises a brick at the given grid position.
 * @param verticalPos Vertical grid position of the brick
 * @param verticalPos Horizontal grid position of the brick
*/
void SpawnBrick(int verticalPos, int horizontalPos) {
	// Calculate the actual global vertical position in the window
	float vertPos = playArea.getPosition().y + (brickSpacing / 2) + (verticalPos) * (brickHeight + brickSpacing);
	// Calculate the actual global vertical position in the window
	float horPos = playArea.getPosition().x + (brickSpacing / 2) + (horizontalPos) * (brickWidth + brickSpacing);

	// Create the brick. Using new to ensure the brick "overlives" this function & we get a uniqe pointer to this brick.
	Brick * brick = new Brick(brickWidth, brickHeight, activeGrid[verticalPos][horizontalPos], sf::Vector2f(horPos, vertPos));

	// Store the newly created brick in the activeBricks multidim. array
	activeBricks[verticalPos][horizontalPos] = brick;
}


/** SpawnBall: Spawns & initialises a new ball
 * @return Pointer to the newly created ball
*/
sf::CircleShape* SpawnBall() {
	// Create the new ball (A CircleShape). Using new to ensure the brick "overlives" this function & we get a uniqe pointer to this brick.
	sf::CircleShape* ball = new sf::CircleShape(ballSize);
	ball->setPosition(ballSpawnPoint);
	ball->setFillColor(sf::Color::Black);

	// Store ball pointer in balls vector(list)
	balls.push_back(ball);
	// Create a new entry for the current ball movement vector
	curBallMoveVectors.push_back(new sf::Vector2f(0, 1));

	return ball;
}

/** MoveBall: Handles ball movement & collision checks
 * KNOWN "ISSUE": The brick/paddle cooldowns mechanic isn't implemented to support multiple balls (yet)! They currently share 1 cooldown each.
 * @param ballNo Index of the ball in the balls vector(list)
*/
void MoveBall(int ballNo) {
	// Count down wall collision cooldown
	curWallColCheckCooldown -= deltaTime;
	// Count down brick collision cooldown
	curBrickColCheckCooldown -= deltaTime;
	// Count down paddle collision cooldown
	curPaddleColCheckCooldown -= deltaTime;

	// Exit if the ball at given index doesn't exist (To avoid access violations)
	if (balls[ballNo] == nullptr)
		return;

	// Create a new ball movement vector if it doesn't exist. (Shouldn't happen - but makes it safer & avoids access violations)
	if (curBallMoveVectors[ballNo] == nullptr) {
		curBallMoveVectors[ballNo] = new sf::Vector2f(0, 1);
	}

#pragma region Handle Border Collisions

	if (curWallColCheckCooldown <= 0) {
		// Check for border collisions
		sf::Vector2f playerAreaCheck = IsObjectWithinAreaVector(balls[ballNo]->getGlobalBounds(), playArea.getGlobalBounds()); // Function checks wheither the balls is within the play area

		// Set Cooldown of there's a collision
		if (playerAreaCheck.x != 0 || playerAreaCheck.y != 0)
			curWallColCheckCooldown = wallColCheckCooldown;

		if (playerAreaCheck.x != 0)// Ball hit left/right wall
			curBallMoveVectors[ballNo]->x *= -1; // Invert x direction

		if (playerAreaCheck.y < 0) // Ball hit top wall
			curBallMoveVectors[ballNo]->y *= -1; // Invert y direction

		if (playerAreaCheck.y > 0) {// Ball hit bottom wall (=> Ball death)
			pointMultiplier = 0; // Reset point multiplier
			ReduceLife(); // Reduces the lifes & handles game over procedure
			RemoveBall(ballNo); // Removes the ball from the game
			return; // Exit function
		}
	}
	
#pragma endregion

#pragma region Handle Brick Collisions
	// Check if there is a cooldown on the brick collision
	if (curBrickColCheckCooldown <= 0) {
		// Iterate through all active bricks
		for (int i = 0; i < std::size(activeBricks); i++) {
			for (Brick *brick : activeBricks[i]) {
				// If there isn't a brick at the current itr position, continue with next itr;
				if (brick == nullptr) {
					continue;
				}

				// Get collision info with brick (Does it collide at all? Collision from which side?
				sf::Vector2f brickCollisionCheck = GetCollisionSideVector(balls[ballNo]->getGlobalBounds(), brick->rect->getGlobalBounds());

				// Check if there were any collisions between the ball and the current brick
				if (brickCollisionCheck.x != 0 || brickCollisionCheck.y != 0) {
					// Set collision check cooldown
					curBrickColCheckCooldown = brickColCheckCooldown;

					// Increase point multiplier
					pointMultiplier++;

					if (brickCollisionCheck.x != 0) {// Collision on left or right
						curBallMoveVectors[ballNo]->x *= -1;
					}
					else if (brickCollisionCheck.y != 0) {// Collision on top or bottom
						curBallMoveVectors[ballNo]->y *= -1;
					}

					// Decreases the health of the brick (& changes its color if it has multiple lifes)
					brick->DecreaseHealth();

					// Increases the gained points, multiplied with the pointMultiplier
					AddPoints(brickPointValue);

					// Remove the brick from the game if it has no lifes left
					if (!brick->IsAlive())
						RemoveBrick(brick);

					// Exit function
					return;
				}
			}
		}
	}
#pragma endregion

#pragma region Handle Paddle Collisions

	// Check if there is a cooldown on the paddle collision check
	if (curPaddleColCheckCooldown <= 0) {
		// Check Paddle Collisions (Are there any collisions? Collision from which side?)
		sf::Vector2f paddleCollisionCheck = GetCollisionSideVector(balls[ballNo]->getGlobalBounds(), paddle.getGlobalBounds());

		// Check if there are any collisions with the paddle
		if (paddleCollisionCheck.x != 0 || paddleCollisionCheck.y != 0) {
			curPaddleColCheckCooldown = paddleColCheckCooldown; // Set the paddle collision check cooldown

			// Check if the collision was on the left/right side; [TODO: Needs optimizations]
			if (paddleCollisionCheck.x != 0) {
				pointMultiplier = 0; // Reset pointMultiplier

				// Increase the horizontal ball speed if the paddle is moving
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
					// Increase the horizontal ball speed even more if the paddle is moving fast
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
						curBallMoveVectors[ballNo]->x *= 1.4;
					else
						curBallMoveVectors[ballNo]->x *= 1.1;
				}

				// Invert the horizontal ball direction
				curBallMoveVectors[ballNo]->x *= -1;
			}

			// Check if the collision was on the top/right side [TODO: Needs optimizations]
			if (paddleCollisionCheck.y != 0) {
				pointMultiplier = 0; // Reset pointMultiplier

				// Increase the horizontal ball speed if the paddle is moving
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {// Paddle moves left
					// Increase the horizontal ball speed even more if the paddle is moving fast
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
						curBallMoveVectors[ballNo]->x -= 0.9;
					else
						curBallMoveVectors[ballNo]->x -= 0.5;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {// Paddle moves right
					// Increase the horizontal ball speed even more if the paddle is moving fast
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
						curBallMoveVectors[ballNo]->x += 0.9;
					else
						curBallMoveVectors[ballNo]->x += 0.5;
				}


				//-- Change ball speed depending on the collision point on the paddle
				// Calculate the center point of the balls boundaries
				sf::Vector2f ballCenter(balls[ballNo]->getGlobalBounds().left + balls[ballNo]->getGlobalBounds().width / 2, balls[ballNo]->getGlobalBounds().top + balls[ballNo]->getGlobalBounds().height / 2);
				// Calculate the center point of the paddles boundaries
				sf::Vector2f paddleCenter(paddle.getGlobalBounds().left + paddle.getGlobalBounds().width / 2, paddle.getGlobalBounds().top + paddle.getGlobalBounds().height / 2);

				// Calculate the angle between the ball and the paddle (0 Degress = paddle middlepoint x on the left; Clockwise; 0-360 Degrees)
				float angleBallPaddle = GetHorizontalAngleBetween(ballCenter, paddleCenter);


				float angleSpeedAddition = 1;
				if (angleBallPaddle < 90) {// Ball hit the paddle on the left side of the paddle surface
					angleSpeedAddition = (1 - std::sin(angleBallPaddle)) * -1; // Speed addition to the left
				}
				else if (angleBallPaddle > 90) {// Ball hit the paddle on the right side of the paddle surface
					angleSpeedAddition = (1 - std::sin(angleBallPaddle)); // Speed addition to the right
				}

				// Check if the current horizontal speed is 0
				if (curBallMoveVectors[ballNo]->x != 0)
					curBallMoveVectors[ballNo]->x += angleSpeedAddition; // Add the angleSpeedAddition to the current horizontal speed
				else
					curBallMoveVectors[ballNo]->x = angleSpeedAddition; // Set the current horizontal ball speed to the angleSpeedAddition

				// Invert vertical ball movement
				curBallMoveVectors[ballNo]->y *= -1;
			}
		}
	}
#pragma endregion


	// Clamp movement speed to maxBallSpeed
	if (curBallMoveVectors[ballNo]->x* defaultBallSpeed > maxBallSpeed)
		curBallMoveVectors[ballNo]->x = 2;
	if (curBallMoveVectors[ballNo]->x* defaultBallSpeed < -maxBallSpeed)
		curBallMoveVectors[ballNo]->x = -2;
	if (curBallMoveVectors[ballNo]->y* defaultBallSpeed > maxBallSpeed)
		curBallMoveVectors[ballNo]->y = 2;
	if (curBallMoveVectors[ballNo]->y* defaultBallSpeed < -maxBallSpeed)
		curBallMoveVectors[ballNo]->y = -2;

	//Move Ball
	balls[ballNo]->move(*curBallMoveVectors[ballNo] * defaultBallSpeed* deltaTime);

}

/** MovePaddle: Handles the movement of the paddle */
void MovePaddle() {
	float multiplicator = 1; // Multiplicator value for the paddle speed

	// Increases the paddle speed multiplier if shift is held down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		multiplicator = 2;

	// Check if the Left/Right arrow is pressed  on the keyboard
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { // Move paddle left
		// Calculate the new position of the paddle
		sf::FloatRect newPaddleBounds = paddle.getGlobalBounds();
		newPaddleBounds.left -= paddleSpeed * multiplicator*deltaTime;

		// Check if the new paddle position is still within the playerArea bounds
		if (IsInBounds(newPaddleBounds, playArea.getGlobalBounds()))
			paddle.move(-paddleSpeed * multiplicator*deltaTime, 0); // Move the paddle
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { // Move paddle right
		// Calculate the new position of the paddle
		sf::FloatRect newPaddleBounds = paddle.getGlobalBounds();
		newPaddleBounds.left += paddleSpeed * multiplicator*deltaTime;

		// Check if the new paddle position is still within the playerArea bounds
		if (IsInBounds(newPaddleBounds, playArea.getGlobalBounds()))
			paddle.move(paddleSpeed * multiplicator*deltaTime, 0); // Move the paddle
	}
}

/** RemoveBrick: Removes the given brick from the play field and deletes it.
 * @param brick Pointer to the brick you want to delete.
*/
void RemoveBrick(Brick *brick) {
	// Exit if the given pointer to the brick is a nullpointer
	if (brick == nullptr)
		return;

	// Find the location of rectangle in activeBlock array (Multidimensional array)
	sf::Vector2i arrayLoc = GetActiveBrickArrayLocation(brick);

	// If the brick wasn't found in the array, delete it and exit function
	if (arrayLoc.x < 0 || arrayLoc.y < 0) {
		delete brick;
		return;
	}

	// Delete brick from memory
	delete activeBricks[arrayLoc.x][arrayLoc.y];

	// Set array entry to null
	activeBricks[arrayLoc.x][arrayLoc.y] = nullptr;
}

/** GetActiveBrickArrayLocation: Looks up the location of a given brick pointer in the activeBricks array
 * @param brick Pointer to the brick you're looking the location for
 * @return sf::Vector2i x=vertical postition, y=horizontal position, -1=not found
*/
sf::Vector2i GetActiveBrickArrayLocation(Brick *brick) {
	// Initialize the location vector with -1
	sf::Vector2i location(-1, -1);

	// Exit if given brick pointer is a nullpointer
	if (brick == nullptr)
		return location;

	// Iterate through the active brick array & look for the given brick pointer
	for (int verti = 0; verti < std::size(activeBricks); verti++) {
		for (int hori = 0; hori < std::size(activeBricks[verti]); hori++) {
			if (activeBricks[verti][hori] == brick) { // Brick found, return it
				location.x = verti;
				location.y = hori;
				return location;
			}
		}
	}

	return location;
}

/** RemoveBall: Removes a ball & its movement vector
 * @param ballNo Index of the ball
*/
void RemoveBall(int ballNo) {
	delete balls[ballNo]; // Delete ball from memory
	delete curBallMoveVectors[ballNo]; // Delete movement vector from memory

	balls.erase(balls.begin() + ballNo); // Remove nullptr entry from balls list
	curBallMoveVectors.erase(curBallMoveVectors.begin() + ballNo); // Remove nullptr entry from ball movement vector list

}

/** ReduceLife: Reduces the players life, updates UI text and handles game over mechanic */
void ReduceLife() {
	lifeCount--; // Reduce life by 1
	UpdateText(); // Update the UI text display of lifes

	// Checks if the player still has lifes left. If not = GAME OVER
	if (lifeCount <= 0) {
		// Check if the player made more than 0 points
		if (pointCount > 0)
			highscores.push_back(pointCount); // Add the points to the highscore list

		// Sort the highscore list highest to lowest
		SortHighscoreList();

		// Update the UI Highscorelist
		UI_UpdateHighscoreEntries();

		// Deactivate game view
		gamePause = true;

		// Activate Highscore List View
		showHighscoreList = true;

		// Saves the highscores to a text file
		SaveHighscores();
	}
}

/** AddPoints: Increases the players points by the given amount (Multiplied by the pointMultiplier */
void AddPoints(int amount) {
	pointCount += amount * (pointMultiplier + 1); // Increase points by the given amount & multiply it by the multiplier (+1 in case the multiplier is zero)

	// Updates the UI Points display
	UpdateText();
}

/** UpdateText: Updates the UI texts for current points & remaining lifes */
void UpdateText() {
	txtPoints.setString("Lifes: " + std::to_string(lifeCount));
	txtLifes.setString("Points: " + std::to_string(pointCount));
}


/** IsInBounds: Checks whether an object is within another one
 * @return bool True if objectBounds is fully within areaBounds
*/
bool IsInBounds(sf::FloatRect objectBounds, sf::FloatRect areaBounds) {
	// Check Left Side
	if (objectBounds.left <= areaBounds.left)
		return false;
	// Check Right Side
	if (objectBounds.left + objectBounds.width >= areaBounds.left + areaBounds.width)
		return false;
	// Check Top Side
	if (objectBounds.top <= areaBounds.top)
		return false;
	// Check Bottom Side
	if (objectBounds.top + objectBounds.height >= areaBounds.top + areaBounds.height)
		return false;

	return true;
}

/** IsObjectWithinAreaVector: Checks whether an object is within another one & returns a vector indicating on which side it is out of bounds
 * @return sf::Vector2f x=-1: left, x=1: right, y=-1: top, y=1: bottom, x/y=0: in bounds
*/
sf::Vector2f IsObjectWithinAreaVector(sf::FloatRect objectBounds, sf::FloatRect areaBounds) {
	sf::Vector2f outOfBoundsVec(0, 0);
	// Check Left Side
	if (objectBounds.left <= areaBounds.left)
		outOfBoundsVec.x = -1;
	// Check Right Side
	if (objectBounds.left + objectBounds.width >= areaBounds.left + areaBounds.width)
		outOfBoundsVec.x = 1;
	// Check Top Side
	if (objectBounds.top <= areaBounds.top)
		outOfBoundsVec.y = -1;
	// Check Bottom Side
	if (objectBounds.top + objectBounds.height >= areaBounds.top + areaBounds.height)
		outOfBoundsVec.y = 1;

	return outOfBoundsVec;
}

/** GetCollisionSideVector: Checks whether two objects are colliding and on which side the collision occured. The collision sides refer to the given objectOne.
 * @param objectOne The main objects bounds
 * @param collider The colliders bounds
 * @return x=-1: left, x=1: right, y=-1: top, y=1: bottom, x/y=0: no collision
*/
sf::Vector2f GetCollisionSideVector(sf::FloatRect objectOne, sf::FloatRect collider) {

	sf::Vector2f collisionSides(0, 0); // Init collision sides vector with 0

	// Check if there is a collision at all with the collider
	if (objectOne.intersects(collider)) {

		// Calculate the center of objectOne
		sf::Vector2f o1Center(objectOne.left + objectOne.width / 2, objectOne.top + objectOne.height / 2);
		// Calculate the center of the collider
		sf::Vector2f colCenter(collider.left + collider.width / 2, collider.top + collider.height / 2);

		// Calculate PI
		const double PI = atan(1) * 4;
		// Calculate delta X/Y of objectOnes' & the colliders' centers
		double deltaX = colCenter.x - o1Center.x;
		double deltaY = colCenter.y - o1Center.y;
		// Calculate the angle between the two objects position. (0 Degree = X position of colliders center on the left; Clockwise; 0-360 Degree;
		double angle = (std::atan2(deltaY, deltaX) * (180 / PI));

		if (angle < 0) {
			angle += 360; // Make sure the angles value is from 0-360
		}

		// Calculate the angles between the colliders corners and its center point
		double cornerLeftTopAngle = std::atan2(colCenter.y - collider.top, colCenter.x - collider.left) * 180 / PI;
		double cornerRightTopAngle = (180 - cornerLeftTopAngle);
		double cornerLeftBottomAngle = 360 - cornerLeftTopAngle;
		double cornerRightBottomAngle = 180 + cornerLeftTopAngle;

		// Left collision
		if (angle < cornerLeftTopAngle || angle > cornerLeftBottomAngle) {
			collisionSides.x = -1;
		}
		// Bottom collision
		if (angle > cornerLeftTopAngle && angle < cornerRightTopAngle) {
			collisionSides.y = 1;
		}
		// Right collision
		if (angle > cornerRightTopAngle && angle < cornerRightBottomAngle) {
			collisionSides.x = 1;
		}
		// Top collision
		if (angle > cornerRightBottomAngle && angle < cornerLeftBottomAngle) {
			collisionSides.y = -1;
		}

		/* TODO: Corner Collisions
		if(angle == cornerLeftTopAngle)
			std::cout << "cornerLeftTopAngle" << std::endl;
		if (angle == cornerRightTopAngle)
			std::cout << "cornerRightTopAngle" << std::endl;
		if (angle == cornerLeftBottomAngle)
			std::cout << "cornerLeftBottomAngle" << std::endl;
		if (angle == cornerRightBottomAngle)
			std::cout << "cornerRightBottomAngle" << std::endl;
		*/

	}


	return collisionSides;
}

/** GetHorizontalAngleBetween: Calculates the angle between 2 vectors.
 * @return double Angle in degrees. (0 Degree = X position of colliders center on the left; Clockwise; 0-360 Degree;)
*/
double GetHorizontalAngleBetween(sf::Vector2f objectOne, sf::Vector2f objectTwo) {
	// Calculate PI
	const double PI = atan(1) * 4;
	// Calculate Angle in Degrees
	double angle = std::atan2(objectTwo.y - objectOne.y, objectTwo.x - objectOne.x) * 180 / PI;

	if (angle < 0) {
		angle += 360; // Ensure that the angles value is in between 0-360
	}

	return angle;
}

#pragma endregion

/**
 * main: Starting point of the application
*/
int main()
{
	WindowLifeCycle(); // Contains a loop which is executed until the game is exited
}