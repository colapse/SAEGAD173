// SimpleSidescrollerGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>

bool playerIsGrounded();

sf::Vector2i windowSize(800, 600);
sf::Vector2f IsObjectWithinAreaVector(sf::FloatRect objectBounds, sf::FloatRect areaBounds);
sf::Vector2f IsObjectWithinAreaVectorInclusive(sf::FloatRect objectBounds, sf::FloatRect areaBounds);
sf::Vector2f GetCollisionSideVector(sf::FloatRect objectOne, sf::FloatRect collider);

sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "SFML works!");
sf::RectangleShape player(sf::Vector2f(50, 50));
sf::RectangleShape gameArea(sf::Vector2f(800, 600));

float gravity = 400;
float groundFriction = 0.009;
float airFriction = 0.0001;
float deltaTime = 0;
float playerGroundSpeed = 40;
float playerAirSpeed = 10;

float jumpForce = -10;
float maxJumpForce = -300;
float accumulatedJumpForce = 0;
float jumpForceMultiplier = 1;

sf::Vector2f playerMovement(0,0);


bool inputJump = false;
bool inputLeft = false;
bool inputRight = false;

bool playerGrounded = false;

float jumpCooldown = 0;

class GameObject {
	sf::Vector2f pos;
public:
	sf::RectangleShape shape;
	bool isSolid = true;

	GameObject() {
		shape = sf::RectangleShape(sf::Vector2f(50,50));
	}

	void SetPosition(sf::Vector2f pos) {
		this->pos = pos;
		shape.setPosition(pos);
	}
};
std::vector<GameObject> gameObjects(0);

int main()
{
	gameArea.setFillColor(sf::Color::Blue);
	player.setPosition(50,500);
	player.setFillColor(sf::Color::Green);


	GameObject box1 = GameObject();
	box1.shape.setFillColor(sf::Color::Black);
	box1.SetPosition(sf::Vector2f(150, 550));
	gameObjects.push_back(box1);

	GameObject box2 = GameObject();
	box2.shape.setFillColor(sf::Color::Yellow);
	box2.SetPosition(sf::Vector2f(250, 550));
	gameObjects.push_back(box2);

	GameObject box3 = GameObject();
	box3.shape.setFillColor(sf::Color::Red);
	box3.SetPosition(sf::Vector2f(350, 550));
	box3.isSolid = false;
	gameObjects.push_back(box3);

	sf::Clock clock;
	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			switch (event.type) {
			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::Space) {
					inputJump = false;
				}
				if (event.key.code == sf::Keyboard::Left) {
					inputLeft = false;
				}

				if (event.key.code == sf::Keyboard::Right) {
					inputRight = false;
				}
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Space) {
					inputJump = true;
				}
				if (event.key.code == sf::Keyboard::Left) {
					inputLeft = true;
				}

				if (event.key.code == sf::Keyboard::Right) {
					inputRight = true;
				}
				break;
			}
		}

		playerGrounded = playerIsGrounded();

		// Friction
		float friction = playerGrounded ? groundFriction : airFriction;
		if (playerMovement.x > 0 && !inputRight) {
			if (playerMovement.x - playerGroundSpeed* friction >= 0) {
				playerMovement.x -= playerGroundSpeed * friction;
			}
			else {
				playerMovement.x = 0;
			}
			
		}else if (playerMovement.x < 0 && !inputLeft) {
			if (playerMovement.x + playerGroundSpeed * friction <= 0) {
				playerMovement.x += playerGroundSpeed * friction;
			}
			else {
				playerMovement.x = 0;
			}
		}

		// Garvity
		if(playerMovement.y < gravity)
			playerMovement.y += gravity * deltaTime;

		// Input Movement
		float speed = playerGrounded ? playerGroundSpeed : playerAirSpeed;
		if (inputLeft && playerMovement.x >= -speed) {
			playerMovement.x -= speed;
		}
		else if (inputRight && playerMovement.x <= speed) {
			playerMovement.x += speed;
		}

		// Jump
		if (inputJump && /*playerGrounded &&*/ jumpCooldown <= 0 && accumulatedJumpForce >= maxJumpForce) {
			
			playerMovement.y += jumpForce * jumpForceMultiplier;
			accumulatedJumpForce += jumpForce * jumpForceMultiplier;
			jumpForceMultiplier += 0.5;
			jumpCooldown = 0.01;
		}

		// Reset
		if ((!inputJump && accumulatedJumpForce !=0 && playerGrounded)/* || (inputJump && accumulatedJumpForce <= maxJumpForce)*/) {
			//playerMovement.y += accumulatedJumpForce;
			accumulatedJumpForce = 0;
			jumpForceMultiplier = 1;
		}

		// Border "Collisioncheck"
		sf::Vector2f WithinBounds = IsObjectWithinAreaVector(player.getGlobalBounds(), gameArea.getGlobalBounds());
		if (WithinBounds.x == -1 && playerMovement.x < 0) {
			playerMovement.x = 0;
		}else if (WithinBounds.x == 1 && playerMovement.x > 0) {
			playerMovement.x = 0;
		}
		if (WithinBounds.y == -1 && playerMovement.y < 0) {
			playerMovement.y = 0;
		}else if (WithinBounds.y == 1 && playerMovement.y > 0) {
			playerMovement.y = 0;
		}
		
		if (gameObjects.size() > 0) {
			for (GameObject go : gameObjects) {
				if (go.isSolid) {
					sf::Vector2f collisions = GetCollisionSideVector(player.getGlobalBounds(), go.shape.getGlobalBounds());
					//playerMovement.x *= collisions.x==0?1:0;
					//playerMovement.y *= collisions.y == 0 ? 1 : 0;
					if (collisions.x == -1 && playerMovement.x > 0)
						playerMovement.x = 0;
					else if (collisions.x == 1 && playerMovement.x < 0)
							playerMovement.x = 0;
					if (collisions.y == -1 && playerMovement.y < 0)
						playerMovement.y = 0;
					else if (collisions.y == 1 && playerMovement.y > 0)
						playerMovement.y = 0;
					//std::cout << std::to_string(go.shape.getFillColor().toInteger()) + " " + std::to_string(playerMovement.x) + " " + std::to_string(playerMovement.y) << std::endl;
					//std::cout << go.shape.getFillColor().toInteger() << " HOI " << std::endl;
				}
			}
		}
		/*
		if (player.getPosition().y >= windowSize.y-10 && playerMovement.y > 0) {
			playerMovement.y = 0;
		}*/

		player.move(playerMovement * deltaTime);

		//Cooldown
		if (jumpCooldown > 0) {
			jumpCooldown -= deltaTime;
		}

		window.clear();
		window.draw(gameArea);

		if (gameObjects.size() > 0) {
			for (GameObject go : gameObjects) {
				window.draw(go.shape);
			}
		}

		window.draw(player);
		window.display();

	}
}

bool playerIsGrounded() {
	bool isGrounded = IsObjectWithinAreaVectorInclusive(player.getGlobalBounds(), gameArea.getGlobalBounds()).y == 1 || player.getPosition().y == gameArea.getGlobalBounds().top + gameArea.getGlobalBounds().height;

	// TODO: DON'T CALL EVERY FRAME MULTIPLE TIMES!!!! Only max. 1 collisin check per frame!
	if (!isGrounded && gameObjects.size() > 0) {
		for (GameObject go : gameObjects) {
			if (go.isSolid) {
				sf::Vector2f collisions = GetCollisionSideVector(player.getGlobalBounds(), go.shape.getGlobalBounds());
				//playerMovement.x *= collisions.x==0?1:0;
				//playerMovement.y *= collisions.y == 0 ? 1 : 0;
				if (collisions.y == 1)
					isGrounded = true;
				//std::cout << std::to_string(go.shape.getFillColor().toInteger()) + " " + std::to_string(playerMovement.x) + " " + std::to_string(playerMovement.y) << std::endl;
				//std::cout << go.shape.getFillColor().toInteger() << " HOI " << std::endl;
			}
		}
	}
	//return player.getPosition().y <= windowSize.y - 10;//TODO
	return isGrounded;
}

/** IsObjectWithinAreaVector: Checks whether an object is within another one & returns a vector indicating on which side it is out of bounds
 * @return sf::Vector2f x=-1: left, x=1: right, y=-1: top, y=1: bottom, x/y=0: in bounds
*/
sf::Vector2f IsObjectWithinAreaVector(sf::FloatRect objectBounds, sf::FloatRect areaBounds) {
	sf::Vector2f outOfBoundsVec(0, 0);
	// Check Left Side
	if (objectBounds.left < areaBounds.left)
		outOfBoundsVec.x = -1;
	// Check Right Side
	if (objectBounds.left + objectBounds.width > areaBounds.left + areaBounds.width)
		outOfBoundsVec.x = 1;
	// Check Top Side
	if (objectBounds.top < areaBounds.top)
		outOfBoundsVec.y = -1;
	// Check Bottom Side
	if (objectBounds.top + objectBounds.height > areaBounds.top + areaBounds.height)
		outOfBoundsVec.y = 1;

	return outOfBoundsVec;
}

/** IsObjectWithinAreaVectorInclusive: Checks whether an object is within another one & returns a vector indicating on which side it is out of bounds (Border already == collision)
 * @return sf::Vector2f x=-1: left, x=1: right, y=-1: top, y=1: bottom, x/y=0: in bounds
*/
sf::Vector2f IsObjectWithinAreaVectorInclusive(sf::FloatRect objectBounds, sf::FloatRect areaBounds) {
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



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
