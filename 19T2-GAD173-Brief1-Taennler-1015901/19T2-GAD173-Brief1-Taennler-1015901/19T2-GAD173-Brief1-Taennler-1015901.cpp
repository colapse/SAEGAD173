// 19T2-GAD173-Brief1-Taennler-1015901.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <Windows.h>
#include <string>

struct Brick {
private:
	int colorChangeStep = 7;
	char brickType;
	int healthPoints;
	void SetHealth(int health) {
		this->healthPoints = health;

		if (health <= 0)
			return;

		int r = baseColor.r - (health * (baseColor.r / colorChangeStep));
		int g = baseColor.g - (health * (baseColor.g / colorChangeStep));
		int b = baseColor.b - (health * (baseColor.b / colorChangeStep));

		if(rect != nullptr)
		rect->setFillColor(sf::Color::Color(r, g, b, 255));
	}


public:
	sf::RectangleShape * rect;
	sf::Color baseColor = sf::Color::White;
	void SetBrickType(char brickType) {
		this->brickType = brickType;

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

	void DecreaseHealth() {
		SetHealth(healthPoints - 1);
	}

	Brick(float width, float height, char brickType, sf::Vector2f pos) {
		rect = new sf::RectangleShape(sf::Vector2f(width, height));
		rect->setPosition(pos);
		SetBrickType(brickType);
	}

	~Brick() {
		delete rect;
	}

	bool IsAlive() {
		return healthPoints > 0;
	}
};

void WindowLifeCycle();
void MovePaddle();
bool isInBounds(sf::FloatRect objectBounds, sf::FloatRect areaBounds);
void UpdateText();
void InitGrid();
void SpawnBricks();
void SpawnBrick(int verticalPos, int horizontalPos);
void MoveBall(int ballNo);
void RemoveBall(int ballNo);
int GetActiveBallCount();
void AddPoints(int amount);
void ReduceLife();
void RemoveBrick(Brick *brick);
sf::Vector2i GetActiveBrickArrayLocation(Brick *brick);
sf::Vector2f isObjectWithinAreaVector(sf::FloatRect objectBounds, sf::FloatRect areaBounds);
sf::Vector2f getCollisionSideVector(sf::FloatRect objectOne, sf::FloatRect collider);

sf::CircleShape* SpawnBall();

sf::RenderWindow renderWindow;
sf::RectangleShape playArea;
sf::RectangleShape paddle;

sf::Font font;
sf::Text txtLifes;
sf::Text txtPoints;

sf::RectangleShape boxPressKeyToSpawnBall;
sf::Text txtPressKeyToSpawnBall;

int lifeCount = 3;
int pointCount = 0;

const int initialBallCount = 1;
float ballSize = 5;
sf::Vector2f ballSpawnPoint;
//sf::CircleShape * balls = new sf::CircleShape[initialBallCount];
std::vector<sf::CircleShape*> balls;
//sf::Vector2f * curBallMoveVectors[initialBallCount];
std::vector<sf::Vector2f*> curBallMoveVectors;

float maxBallSpeed = 150;
float defaultBallSpeed = 100;
float paddleSpeed = 200;
float brickWidth = 0;
float brickHeight = 0;
float brickPaddleMinSpace = 150;
float brickSpacing = 10;

int brickPointValue = 1;

// Wonderful solution
std::string exeDir = "C:\\Users\\lucat\\source\\repos\\19T2-GAD173-Brief1-Taennler-1015901\\x64\\Debug\\";
// font from https://www.dafont.com/
std::string fontName = "Radiant.ttf";

const int gridHorizontalBricks = 11;
const int gridVerticalBricks = 11;
/*int grid[gridVerticalBricks][gridHorizontalBricks] = {
		{0,0,0,0,0,1,0,0,0,0,0},
		{0,0,0,0,1,1,1,0,0,0,0},
		{0,0,0,1,1,1,1,1,0,0,0},
		{0,0,1,1,1,1,1,1,1,0,0},
		{0,1,1,1,1,1,1,1,1,1,0},
		{1,1,1,1,1,1,1,1,1,1,1},
		{0,1,1,1,1,1,1,1,1,1,0},
		{0,0,1,1,1,1,1,1,1,0,0},
		{0,0,0,1,1,1,1,1,0,0,0},
		{0,0,0,0,1,1,1,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,0}
};*/
char grid[gridVerticalBricks][gridHorizontalBricks] = {
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

class Level {
	int gridSizeX = 0;
	int gridSizeY = 0;
	int initialBallCount = 1;
	std::vector<std::vector<char>> grid;

public:
	
	Level(int gridSizeX, int gridSizeY, int initialBallCount){
		this->initialBallCount = initialBallCount;
		grid.resize(gridSizeY);
		for (int i = 0; i < gridSizeY; i++) {
			grid[i].resize(gridSizeX);
		}
	}

	Level(std::vector<std::vector<char>> pGrid) : grid(pGrid) {}

	char GetValueAtCoord(int x, int y) {
		if (x >= gridSizeX || y >= gridSizeY)
			return 0;

		return grid[y][x];
	}

	int GetInitialBallCount() {
		return initialBallCount;
	}

}



//sf::RectangleShape * activeBricks[gridVerticalBricks][gridHorizontalBricks]; DEL
Brick * activeBricks[gridVerticalBricks][gridHorizontalBricks];



int main()
{
	
	WindowLifeCycle();
}

float deltaTime = 0;
void WindowLifeCycle() {
	renderWindow.create(sf::VideoMode(800, 540), "Breakout - GAD173 - Brief 1");
	
	if (!font.loadFromFile(exeDir + fontName))
	{
		std::cout << "Couldn't load font!" << std::endl;
	}
	// Init Text String (Required to determine an adequate position [not perfect pos tho since the string length will change with diff. numbers])
	UpdateText();
	// Init Lifes Text
	txtLifes.setFont(font);
	txtLifes.setCharacterSize(24);
	txtLifes.setOrigin(txtLifes.getLocalBounds().width/2, 0);
	txtLifes.setPosition(200,8);
	txtLifes.setFillColor(sf::Color::Color(219, 219, 219, 255));
	// Init Points Text
	txtPoints.setFont(font);
	txtPoints.setCharacterSize(24);
	txtPoints.setOrigin(txtLifes.getLocalBounds().width / 2, 0);
	txtPoints.setPosition(600, 8);
	txtPoints.setFillColor(sf::Color::Color(219, 219, 219, 255));
	
	// Init play area
	playArea = sf::RectangleShape(sf::Vector2f(780, 480));
	playArea.setPosition(10,50);
	playArea.setFillColor(sf::Color::Color(219,219,219,255));

	// Init paddle
	paddle = sf::RectangleShape(sf::Vector2f(50,10));
	paddle.setPosition(playArea.getPosition().x + playArea.getLocalBounds().width/2 - paddle.getLocalBounds().width/2, playArea.getPosition().y+playArea.getLocalBounds().height-2* paddle.getLocalBounds().height);
	paddle.setFillColor(sf::Color::Black);
	
	// Init boxPressKeyToSpawnBall & co
	boxPressKeyToSpawnBall = sf::RectangleShape(sf::Vector2f(playArea.getGlobalBounds().width / 1.7, 50));
	boxPressKeyToSpawnBall.setPosition(sf::Vector2f((playArea.getGlobalBounds().left + playArea.getGlobalBounds().width) / 2 - playArea.getGlobalBounds().width / 1.7 / 2, (playArea.getGlobalBounds().top + playArea.getGlobalBounds().height) / 2 - 25));
	boxPressKeyToSpawnBall.setFillColor(sf::Color::Color(33,33,33,200));

	txtPressKeyToSpawnBall.setString("Press [Space] to spawn ball.");
	txtPressKeyToSpawnBall.setFillColor(sf::Color::White);
	txtPressKeyToSpawnBall.setFont(font);
	txtPressKeyToSpawnBall.setCharacterSize(20);
	txtPressKeyToSpawnBall.setPosition(sf::Vector2f((playArea.getGlobalBounds().left + playArea.getGlobalBounds().width) / 2 - txtPressKeyToSpawnBall.getGlobalBounds().width / 2, (playArea.getGlobalBounds().top + playArea.getGlobalBounds().height) / 2 - 10));
	// Init Grid
	InitGrid();

	// Init Ball Spawn Point
	ballSpawnPoint = sf::Vector2f(playArea.getGlobalBounds().left + (playArea.getLocalBounds().width/2), paddle.getGlobalBounds().top - 100);

	balls.resize(initialBallCount);
	curBallMoveVectors.resize(initialBallCount);
	// Init Ball
	for (int i = 0; i < initialBallCount; i++) {
		SpawnBall();
		//balls[i] = new sf::CircleShape(SpawnBall());
		//curBallMoveVectors[i] = new sf::Vector2f(0, 1);
	}
	
	sf::Clock Clock;
	int offset = 0;
	

	while (renderWindow.isOpen())
	{
		//float Time = (float)Clock.getElapsedTime().asMilliseconds();
		deltaTime = Clock.restart().asSeconds();
		sf::Event event;
		while (renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				renderWindow.close();
		}

		MovePaddle();

		// Handle Balls
		std::size(balls);
		for (int i = 0; i < std::size(balls); i++) {
			if (balls[i] != nullptr) {
				MoveBall(i);
			}
		}
		/*
		if (fmod(Time, 1) == 0) {

		}*/


		renderWindow.clear();
		renderWindow.draw(txtLifes);
		renderWindow.draw(txtPoints);
		renderWindow.draw(playArea);
		renderWindow.draw(paddle);
		
		// Render Active Bricks
		if (std::size(activeBricks) > 0) {
			for (int i = 0; i < std::size(activeBricks); i++) {
				for (int y = 0; y < std::size(activeBricks[i]); y++) {
					if (activeBricks[i][y] != nullptr) {
						renderWindow.draw(*activeBricks[i][y]->rect);
					}
				}
			}
		}
		// Render Active Balls
		if (std::size(balls) > 0) {
			for (sf::CircleShape *cs : balls) {
				if (cs != nullptr)
					renderWindow.draw(*cs);
			}
		}

		int activeBalls = 0;
		for (int i = 0; i < std::size(balls); i++) {
			if (balls[i] != nullptr)
				activeBalls++;
		}
		if (activeBalls == 0) {
			renderWindow.draw(boxPressKeyToSpawnBall);
			renderWindow.draw(txtPressKeyToSpawnBall);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				SpawnBall();
			}

		}
		

		renderWindow.display();
	}
}

void InitGrid() {
	// Get the longth of the longest horizontal grid row
	int maxBricksHor = 0;
	for(int i = 0; i < std::size(grid); i++)
	{
		int arrSize = std::size(grid[i]);
		if (arrSize > maxBricksHor) {
			maxBricksHor = arrSize;
		}
	}

	// Determine grid width & height depending on the size of the playArea & amount of bricks (+1 Brick width/height empty space left/right/top/bottom)
	brickWidth = playArea.getLocalBounds().width / (maxBricksHor) - brickSpacing;
	brickHeight = (playArea.getLocalBounds().height - brickPaddleMinSpace - (3 * paddle.getLocalBounds().height)) / (std::size(grid)) - brickSpacing;

	SpawnBricks();
}

void SpawnBricks() {
	// Spawn Bricks
	for (int verti = 0; verti < std::size(grid); verti++)
	{
		int arrSize = std::size(grid[verti]);
		for (int hori = 0; hori < arrSize; hori++) {
			if(grid[verti][hori] > 0)
				SpawnBrick(verti, hori);
		}
	}
}

void SpawnBrick(int verticalPos, int horizontalPos) {

	//sf::RectangleShape* brick = new sf::RectangleShape(sf::Vector2f(brickWidth, brickHeight));
	//brick->setFillColor(sf::Color::Black);
	float vertPos = playArea.getPosition().y + (brickSpacing / 2) + (verticalPos) * (brickHeight + brickSpacing);
	float horPos = playArea.getPosition().x + (brickSpacing / 2) + (horizontalPos) * (brickWidth + brickSpacing);
	//brick->setPosition(horPos, vertPos);
	Brick * brick = new Brick(brickWidth, brickHeight, grid[verticalPos][horizontalPos], sf::Vector2f(horPos, vertPos));

	activeBricks[verticalPos][horizontalPos] = brick;
}

sf::CircleShape* SpawnBall() {
	sf::CircleShape* ball = new sf::CircleShape(ballSize);
	sf::Vector2f pos = ballSpawnPoint;
	pos.x += (GetActiveBallCount()-1)*ballSize+0.1f;
	ball->setPosition(pos);
	ball->setFillColor(sf::Color::Black);

	// Find a free spot in the balls array
	int ballsIndex = -1;
	for (int i = 0; i < std::size(balls); i++) {
		if (balls[i] == nullptr) {
			ballsIndex = i;
		}
	}
	// If no free spot in balls array available, inrease array size
	if (ballsIndex == -1) {
		int ballsSize = std::size(balls);
		balls.resize(ballsSize + 1);
		curBallMoveVectors.resize(ballsSize + 1);
		ballsIndex = ballsSize;
	}


	balls[ballsIndex] = ball;
	curBallMoveVectors[ballsIndex] = new sf::Vector2f(0, 1);
	return ball;
}

int GetActiveBallCount() {
	int count = 0;
	for (int i = 0; i < std::size(balls); i++) {
		if (balls[i] != nullptr)
			count++;
	}
	return count;
}

void MoveBall(int ballNo) {
	if (balls[ballNo] == nullptr) {
		return;
	}
	if (curBallMoveVectors[ballNo] == nullptr) {
		curBallMoveVectors[ballNo] = new sf::Vector2f(0, 1);
	}

	// Check for border collisions
	sf::Vector2f playerAreaCheck = isObjectWithinAreaVector(balls[ballNo]->getGlobalBounds(), playArea.getGlobalBounds());
	// Ball hit left/right wall. Invert x direction
	if (playerAreaCheck.x != 0)
		curBallMoveVectors[ballNo]->x *= -1;
	// Ball hit top wall. Invert y direction
	if (playerAreaCheck.y < 0) // >
		curBallMoveVectors[ballNo]->y *= -1;
	
	// Ball hit bottom wall. Reduce Lifes.
	if (playerAreaCheck.y > 0) {
		ReduceLife();
		RemoveBall(ballNo);
		return;
	}
	
	// Check Brick Collisions
	for (int i = 0; i < std::size(activeBricks); i++) {
		for (Brick *brick : activeBricks[i]) {
			if (brick == nullptr) {
				continue;
			}
				
			sf::Vector2f brickCollisionCheck = getCollisionSideVector(balls[ballNo]->getGlobalBounds(), brick->rect->getGlobalBounds());
			if (brickCollisionCheck.x != 0 || brickCollisionCheck.y != 0) {
				brick->DecreaseHealth();
				AddPoints(brickPointValue);
				if(!brick->IsAlive())
					RemoveBrick(brick);
				std::cout << std::to_string(brickCollisionCheck.x) + "  " + std::to_string(brickCollisionCheck.y) << std::endl;
				if (brickCollisionCheck.x != 0) {
					//curBallMoveVectors[ballNo]->x *= -1;
					curBallMoveVectors[ballNo]->x *= -1;
				}

				if (brickCollisionCheck.y != 0) {
					curBallMoveVectors[ballNo]->y *= -1;
					curBallMoveVectors[ballNo]->x *= -1;
				}
			}
		}
	}
	
	// Check Paddle Collisions
	sf::Vector2f paddleCollisionCheck = getCollisionSideVector(balls[ballNo]->getGlobalBounds(), paddle.getGlobalBounds());
	//TODO Distance of the ball to the middle of the paddle
	if (paddleCollisionCheck.x != 0) {
		//TODO DUPLICATE CODE
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			curBallMoveVectors[ballNo]->x -= 2;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			curBallMoveVectors[ballNo]->x += 2;
		}
		curBallMoveVectors[ballNo]->x *= -1.2;
		
	}
		
	if (paddleCollisionCheck.y != 0) {

		//TODO DUPLICATE CODE
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			curBallMoveVectors[ballNo]->x -= 0.5;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			curBallMoveVectors[ballNo]->x += 0.5;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			curBallMoveVectors[ballNo]->x *= 1.2;

		// TODO Paddle-ball distance not well yet
		float dist = (balls[ballNo]->getGlobalBounds().left+(balls[ballNo]->getGlobalBounds().width/2))- (paddle.getGlobalBounds().left + (paddle.getGlobalBounds().width / 2));
		float distNormalized = dist / paddle.getLocalBounds().width;

		curBallMoveVectors[ballNo]->x = distNormalized * 2;
		

		curBallMoveVectors[ballNo]->y *= -1;
	}

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

void RemoveBrick(Brick *brick) {
	if (brick == nullptr)
		return;

	// Find location of rectangle in activeBlock array
	sf::Vector2i arrayLoc = GetActiveBrickArrayLocation(brick);

	// If it wasn't found, exit function
	if (arrayLoc.x < 0 || arrayLoc.y < 0)
		return;

	// Delete brick from memory
	delete activeBricks[arrayLoc.x][arrayLoc.y];

	// Set array entry to null
	activeBricks[arrayLoc.x][arrayLoc.y] = nullptr;
}

// @return: x=vertical postition, y=horizontal position, -1=not found
sf::Vector2i GetActiveBrickArrayLocation(Brick *brick) {/*sf::RectangleShape *rs*/
	sf::Vector2i location(-1, -1);
	if (brick == nullptr)
		return location;

	for (int verti = 0; verti < std::size(activeBricks); verti++) {
		for (int hori = 0; hori < std::size(activeBricks[verti]); hori++) {
			if (activeBricks[verti][hori] == brick) {
				location.x = verti;
				location.y = hori;
			}
		}
	}

	return location;
}

void RemoveBall(int ballNo) {
	delete balls[ballNo];
	delete curBallMoveVectors[ballNo];

	for (int i = ballNo; i < std::size(balls) - i; i++) {
		//std::remove(std::begin(balls), std::end(balls), &balls[i]);
		if (std::size(balls) > 1) {
			balls[i] = balls[i + 1];
			curBallMoveVectors[i] = curBallMoveVectors[i + 1];
		}
		
	}
	balls[std::size(balls) - 1] = nullptr;
	curBallMoveVectors[std::size(curBallMoveVectors) - 1] = nullptr;
}

void ReduceLife() {
	lifeCount--;
	UpdateText();

	if (lifeCount <= 0) {
		//TODO end/reset game.
		std::cout << "GAME OVER [TODO]" << std::endl;
	}
}

void AddPoints(int amount) {
	pointCount += amount;
	UpdateText();
}

void UpdateText() {
	txtPoints.setString("Lifes: "+std::to_string(lifeCount));
	txtLifes.setString("Points: " + std::to_string(pointCount));
}

void MovePaddle() {
	float multiplicator = 1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		multiplicator = 2;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		sf::FloatRect newPaddleBounds = paddle.getGlobalBounds();
		newPaddleBounds.left -= paddleSpeed * multiplicator*deltaTime;
		// Check if the paddle is in bounds with the playerArea bounds
		if(isInBounds(newPaddleBounds, playArea.getGlobalBounds()))
			paddle.move(-paddleSpeed * multiplicator*deltaTime, 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		sf::FloatRect newPaddleBounds = paddle.getGlobalBounds();
		newPaddleBounds.left += paddleSpeed * multiplicator*deltaTime;
		// Check if the paddle is in bounds with the playerArea bounds
		if (isInBounds(newPaddleBounds, playArea.getGlobalBounds()))
			paddle.move(paddleSpeed * multiplicator*deltaTime, 0);
	}
}

bool isInBounds(sf::FloatRect objectBounds, sf::FloatRect areaBounds) {
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

// @return x=-1: left, x=1: right, y=-1: top, y=1: bottom, x/y=0: in bounds
sf::Vector2f isObjectWithinAreaVector(sf::FloatRect objectBounds, sf::FloatRect areaBounds) {
	sf::Vector2f outOfBoundsVec(0,0);
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

// @return x=-1: left, x=1: right, y=-1: top, y=1: bottom, x/y=0: no collision
sf::Vector2f getCollisionSideVector(sf::FloatRect objectOne, sf::FloatRect collider) {
	
	sf::Vector2f collisionSides(0, 0);

	if (objectOne.intersects(collider)) {
		sf::Vector2f objectOneMidPoint(objectOne.left + objectOne.width/2, objectOne.top + objectOne.height/2);
		sf::Vector2f colliderMidPoint(collider.left + collider.width/2, collider.top + collider.height/2);
		
		// Collision on left
		if (objectOne.left >= (collider.left + collider.width)) {
			collisionSides.x = -1;
		}
		// Collision on right
		if ((objectOne.left + objectOne.width) >= collider.left) {
			collisionSides.x = 1;
		}
		// Collision on top
		if (objectOne.top >=(collider.top + collider.height)) {
			collisionSides.y = -1;
		}
		// Collision on bottom
		if ((objectOne.top + objectOne.height) >= collider.top) {
			collisionSides.y = 1;
		}/*
		// Collision on left
		if (objectOne.left >= (collider.left + collider.width) && objectOne.left <= collider.left+collider.width-15 && (objectOne.top > collider.top + 15 || objectOne.top + objectOne.height < collider.top+collider.height - 15)) {
			collisionSides.x = -1;
		}
		// Collision on right
		if ((objectOne.left + objectOne.width) >= collider.left && objectOne.left + objectOne.width >= collider.left + 15 && (objectOne.top > collider.top + 15 || objectOne.top + objectOne.height < collider.top + collider.height - 15)) {
			collisionSides.x = 1;
		}
		// Collision on top
		if (objectOne.top >= (collider.top + collider.height) && objectOne.top <= collider.top + collider.height - 15 && (objectOne.left > collider.left + 15 || objectOne.left + objectOne.width < collider.left + collider.width - 15 )) {
			collisionSides.y = -1;
		}
		// Collision on bottom
		if ((objectOne.top + objectOne.height) >= collider.top && objectOne.top + objectOne.height <= collider.top + 15 && (objectOne.left > collider.left + 15 || objectOne.left + objectOne.width < collider.left + collider.width - 15)) {
			collisionSides.y = 1;
		}*/
	}


	return collisionSides;
}

// Programm ausführen: STRG+F5 oder "Debuggen" > Menü "Ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
