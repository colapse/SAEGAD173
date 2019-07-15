#include "pch.h"
#include "GameObjectType.h"


std::map<char, GameObjectType*> GameObjectType::gameObjectTypes;
std::map<char, sf::Texture*> GameObjectType::gameObjectTypeTextures;

bool GameObjectType::IsValidGOTypeChar(char c) {
	std::map<char, GameObjectType*>::iterator it = gameObjectTypes.find(c);
	if (it != gameObjectTypes.end())
	{
		return true;
	}
	return false;
}

GameObjectType::GameObjectType(char pGameObjectId, std::string pName, std::string gameObjectSprite) : gameObjectId(pGameObjectId), name(pName), spriteLoc(gameObjectSprite) {

}

GameObjectType::GameObjectType(char pGameObjectId, std::string pName, std::string gameObjectSprite, int pMaxAllowedPerLevel) : gameObjectId(pGameObjectId), name(pName), spriteLoc(gameObjectSprite){
	maxAllowedPerLevel = pMaxAllowedPerLevel;
}

GameObjectType::GameObjectType() : gameObjectId(' '), name(" "), spriteLoc(" ")
{
}


GameObjectType::~GameObjectType()
{
}
