// Week7InClass.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

void printGrid();
void printGrid2();
void printGrid3();

struct Brick {
	sf::RectangleShape* rect;
	bool type = false;

	Brick() {
		rect = new sf::RectangleShape(sf::Vector2f(50,50));
		rect->setFillColor(sf::Color::White);
	}

	void ToggleType() {
		type = !type;
		if(!type)
			rect->setFillColor(sf::Color::White);
		else
			rect->setFillColor(sf::Color::Black);
	}
};

const int sizeX = 5;
const int sizeY = 5;
std::vector<std::vector<Brick*>> grid(sizeY,std::vector<Brick*>(sizeX));
sf::Vector2i mousePos;


int main()
{
	sf::RenderWindow window(sf::VideoMode(250, 250), "Week 7 ex!");

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			grid[y][x]= new Brick();
			grid[y][x]->rect->setPosition(x*50,y*50);
			grid[y][x]->rect->setOutlineColor(sf::Color::Green);
			grid[y][x]->rect->setOutlineThickness(1);
		}
	}
	sf::Clock deltaClock;
	float printCooldown = 0;

	while (window.isOpen())
	{
		sf::Time dt = deltaClock.restart();
		if (printCooldown <= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
			//printGrid();
			//printGrid2();
			printGrid3();
			printCooldown = .5;
		}
		printCooldown -= dt.asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseMoved) {
				mousePos = sf::Mouse::getPosition(window);
			}
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseMoved:
				mousePos = sf::Mouse::getPosition(window);
				break;
			case sf::Event::MouseButtonReleased:
				int xPos = mousePos.x / 50;
				int yPos = mousePos.y / 50;
				if (grid[yPos][xPos] != nullptr) {
					grid[yPos][xPos]->ToggleType();
				}
					
				break;
			}
		}

		window.clear();
		
		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++) {
				if (grid[y][x] != nullptr)
					window.draw(*grid[y][x]->rect);
			}
		}
		window.display();
	}

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			delete grid[y][x];
		}
	}
}

void printGrid() {
	std::cout << "================ GRID ================" << std::endl;
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			std::cout << std::to_string(grid[y][x]->type) + " ";
		}
		std::cout << std::endl;
	}
	std::cout << "================ ==== ================" << std::endl;
}

void printGrid2() {
	std::cout << "================ GRID ================" << std::endl;

	for (int y = 0; y < sizeY; y++) {
		int zeroOffset = 0;
		int oneOffset = 0;
		for (int x = 0; x < sizeX; x++) {
			if (!grid[y][x]->type) {
				zeroOffset++;
				if (oneOffset != 0) {
					std::cout << std::to_string(oneOffset) + " on ";
					oneOffset = 0;
				}
				if (x == sizeX - 1) {
					std::cout << std::to_string(zeroOffset) + " off ";
				}
			}
			else {
				oneOffset++;
				if (zeroOffset != 0) {
					std::cout << std::to_string(zeroOffset) + " off ";
					zeroOffset = 0;
				}
				if (x == sizeX - 1) {
					std::cout << std::to_string(oneOffset) + " on ";
				}
			}
		}
		std::cout << std::endl;
	}
	std::cout << "================ ==== ================" << std::endl;
}

void printGrid3() {
	std::cout << "================ GRID ================" << std::endl;
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			if(grid[y][x]->type)
				std::cout << "("+std::to_string(x)+","+ std::to_string(y)+ ") ";
		}
		std::cout << std::endl;
	}
	std::cout << "================ ==== ================" << std::endl;
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
