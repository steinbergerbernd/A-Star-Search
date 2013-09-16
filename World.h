#pragma once

#include "GameObject.h"
#include "HexMap.h"
#include "Player.h"
#include "Target.h"
#include "Enemy.h"

#include <ClanLib/display.h>

#include <list>
#include <string>
#include <sstream>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;

class World
{
public:
	World(CL_DisplayWindow &display_window);
	~World(void);

	CL_ResourceManager resources;
	CL_GraphicContext get_gc() { return graphicContext; }

	Target* getTarget();
	Player* getPlayer();
	vector<Enemy*> getEnemies();

	string pointToString(CL_Point point);

	bool getPauseState();

	void run();
private:
	CL_DisplayWindow window;
	CL_GraphicContext graphicContext;

	bool pause;

	std::list<GameObject*> objects;

	HexMap* hexMap;
	Player* player;
	Target* target;
	vector<Enemy*> enemies;

	void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);

	Enemy* createSoldier();
	Enemy* createTank();
	Enemy* createHelicopter();

	void addObject(GameObject* object);

	float getElapsedTime();

	void draw();
	void update();
};

