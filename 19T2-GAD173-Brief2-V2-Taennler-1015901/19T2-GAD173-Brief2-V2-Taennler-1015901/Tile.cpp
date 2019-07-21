#include "pch.h"
#include "Tile.h"


Tile::Tile(int width, int height, TileType * pTileType, bool pActive) : active(pActive), tileType(pTileType) {
	//rect.setSize(sf::Vector2f(width, height));
	size = sf::Vector2i(width, height);
	SetTileType(tileType);
}

Tile::Tile() {
	if (TileType::tileTypes[0] != nullptr) {
		SetTileType(TileType::tileTypes[0]);
	}
	else
		active = false;
}

Tile::~Tile() {
	tileType = nullptr;
	if(sprite != nullptr){
		//delete sprite;
		sprite = nullptr;
	}
	if (gameObjectSprite != nullptr) {
		delete gameObjectSprite;
		gameObjectSprite = nullptr;
	}
}

void Tile::SetPosition(float x, float y) {
	sprite->setPosition(x, y);
	gameObjectSprite->setPosition(x, y);
	//sf::Vector2i intPos(x, y);
	//sprite.setTextureRect(sf::IntRect(intPos, size));
}

void Tile::SetPosition(sf::Vector2f pos) {
	sprite->setPosition(pos);
	gameObjectSprite->setPosition(pos);
	//sf::Vector2i intPos(pos.x, pos.y);
	//sprite.setTextureRect(sf::IntRect(intPos, size));
}

TileType * Tile::GetTileType() {
	return tileType;
}

GameObjectType * Tile::GetGameObjectType() {
	return gameObjectType;
}

sf::Sprite * Tile::GetSprite() {
	return sprite;
}

sf::Sprite * Tile::GetGameObjectSprite() {
	return gameObjectSprite;
}

void Tile::SetTileType(TileType * tileType) {
	if (tileType == nullptr) {
		return;
	}
	this->tileType = tileType;

	/*
	sf::Texture * texture = new sf::Texture();
	if ( !texture->loadFromFile(this->tileType->spriteLoc, sf::IntRect(0, 0, size.x, size.y)))
	{
		std::cout << "Couldn't load tile sprite! Tile ID: " + std::to_string(tileType->tileId) + " . " + this->tileType->spriteLoc + " ." << std::endl;
		if (sprite.getTexture()) {
			delete sprite.getTexture();
		}
	}
	else {
		std::cout << "Loaded texture " + std::to_string(tileType->tileId) + " . " + this->tileType->spriteLoc + " ." << std::endl;
		if (sprite.getTexture()) {
			delete sprite.getTexture();
		}

		sprite.setTexture(*texture);
	}*/
	if (TileType::tileTypeTextures[tileType->tileId] != nullptr)
		sprite->setTexture(*TileType::tileTypeTextures[tileType->tileId]);
	else {
		sf::Texture t;
		t.create(size.x, size.y);
		sprite->setTexture(t);
	}
}

void Tile::SetGameObjectType(GameObjectType * gameObjectType) {
	this->gameObjectType = gameObjectType;

	if (gameObjectType != nullptr && GameObjectType::gameObjectTypeTextures[gameObjectType->gameObjectId] != nullptr)
		gameObjectSprite->setTexture(*GameObjectType::gameObjectTypeTextures[gameObjectType->gameObjectId]);
	else
		gameObjectSprite->setTexture(sf::Texture());
}
