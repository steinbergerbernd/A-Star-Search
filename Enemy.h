#pragma once
#include "MapObject.h"
#include "WayPoint.h"
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;

class Enemy : public MapObject
{
public:
	struct EnemyField
	{
		CL_Point position;
		int costs;
	};

	//Typen von Gegnern
	enum EnemyType { tank, soldier, helicopter };

	Enemy(World* world, const HexMap& hexMap, EnemyType enemyType, vector<CL_Point> controlPoints);
	virtual ~Enemy(void);

	void draw();
	void update(float elapsed);

	//liefert alle Felder innerhalb des Gegnerradius
	map<string, EnemyField> getEnemyFields();

	int getCosts();
private:
	EnemyType enemyType;

	//Wegpunkte für den Gegner auf denen er patroulliert
	vector<CL_Point> controlPoints;

	map<string, EnemyField> enemyFields;

	//vergangene Zeit seit dem letzten Gegnerzug
	float elapsedLastMove;

	//Index des aktuellen Wegpunktes
	int currentMoveIndex;

	//setzt alle Felder innerhalb des Gegnerradius
	void findEnemyFields(int radius, CL_Point position);

	pair<string, EnemyField> createEnemyFieldPair(string key, CL_Point fieldPosition);

	float getMoveSpeed();
	int getAttackRadius();
};