#pragma hdrstop

#include "enemies.h"

#include <json/json.h>
#include <fstream>

EnemyBuilder::EnemyBuilder(TextureManager * textureManager) {
	this->textureManager = textureManager;
}

bool EnemyBuilder::loadConfiguration(string path) {
	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse(ifstream(path), root);
	if (!parsingSuccessful) {
		std::cout << "Failed to parse configuration\n"
			<< reader.getFormattedErrorMessages();
		return false;
	}
	Json::Value enemies = root["enemies"];
	for (Json::Value::iterator i = enemies.begin(); i != enemies.end(); i++) {
		Json::Value enemy = *i;

		EnemyShip enemyShip;
		enemyShip.dx = 0;
		enemyShip.dy = 1;
		enemyShip.speed = enemy["speed"].asDouble();
		enemyShip.HP = enemy["hp"].asInt();

		string texture_name = enemy["texture"].asString();
		enemyShip.setTexture( textureManager->get(texture_name) );
		enemyShip.animationCount = enemy["frames"].asInt();
		enemyShip.animationRect = CRect(0, 0, 64, 64);
		enemyShip.scaling = enemy["scaling"].asDouble();

		string id = enemy["id"].asString();
		data[id] = enemyShip;
	}
	return true;
}

EnemyShip EnemyBuilder::get(string type) {
	// data contains "empty" instances for all the enemies
	return data[type];
}
