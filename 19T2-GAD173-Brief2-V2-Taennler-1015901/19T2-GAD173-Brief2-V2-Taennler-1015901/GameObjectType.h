#pragma once
#ifndef string
#include <string>
#endif // !string
#ifndef map
#include <map>
#endif // !map
#ifndef SFML_GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#endif // !SFML/Graphics.hpp

class GameObjectType
{
public:
	static std::map<char, GameObjectType*> gameObjectTypes;
	static std::map<char, sf::Texture*> gameObjectTypeTextures;

	static bool IsValidGOTypeChar(char c);
	const char gameObjectId;
	const std::string name;
	const std::string spriteLoc;
	int maxAllowedPerLevel = -1; // -1 = unlimited

	GameObjectType(char pGameObjectId, std::string pName, std::string gameObjectSprite);
	GameObjectType(char pGameObjectId, std::string pName, std::string gameObjectSprite, int pMaxAllowedPerLevel);
	GameObjectType();
	~GameObjectType();
};

