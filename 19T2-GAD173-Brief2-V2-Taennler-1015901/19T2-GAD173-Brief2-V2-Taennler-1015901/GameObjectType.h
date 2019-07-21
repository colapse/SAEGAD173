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
	static std::map<std::string, GameObjectType*> gameObjectTypes;
	static std::map<std::string, sf::Texture*> gameObjectTypeTextures;
	static const std::string defaultGameObjectTypeChar;

	static bool IsValidGOTypeChar(std::string c);
	const std::string gameObjectId;
	const std::string name;
	const std::string spriteLoc;
	int maxAllowedPerLevel = -1; // -1 = unlimited

	GameObjectType(std::string pGameObjectId, std::string pName, std::string gameObjectSprite);
	GameObjectType(std::string pGameObjectId, std::string pName, std::string gameObjectSprite, int pMaxAllowedPerLevel);
	GameObjectType();
	~GameObjectType();
};

