#include "pch.h"
#include "GameObjectType.h"


std::map<std::string, GameObjectType*> GameObjectType::gameObjectTypes;
std::map<std::string, sf::Texture*> GameObjectType::gameObjectTypeTextures;
const std::string GameObjectType::defaultGameObjectTypeChar = "0";

bool GameObjectType::IsValidGOTypeChar(std::string c) {
	std::map<std::string, GameObjectType*>::iterator it = gameObjectTypes.find(c);
	if (it != gameObjectTypes.end())
	{
		return true;
	}
	return false;
}

GameObjectType::GameObjectType(std::string pGameObjectId, std::string pName, std::string gameObjectSprite) : gameObjectId(pGameObjectId), name(pName), spriteLoc(gameObjectSprite) {

}

GameObjectType::GameObjectType(std::string pGameObjectId, std::string pName, std::string gameObjectSprite, int pMaxAllowedPerLevel) : gameObjectId(pGameObjectId), name(pName), spriteLoc(gameObjectSprite){
	maxAllowedPerLevel = pMaxAllowedPerLevel;
}

GameObjectType::GameObjectType() : gameObjectId(""), name(" "), spriteLoc(" ")
{
}


GameObjectType::~GameObjectType()
{
}
