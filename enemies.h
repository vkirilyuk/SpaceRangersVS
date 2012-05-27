//---------------------------------------------------------------------------

#ifndef enemiesH
#define enemiesH

#include <d3dx9.h>
#include "types.h"

#include <map>
using namespace std;

#include "enemyShip.h"
#include "TextureManager.h"

class EnemyBuilder {
private:
	map<string, EnemyShip> data;
	TextureManager * textureManager;
public:
	EnemyBuilder(TextureManager * textureManager);
	bool loadConfiguration(string path);
	EnemyShip get(string type);
};

class EnemyManager {
private:
	vector<EnemyShip> enemies;
public:
	EnemyManager();
};

#endif
