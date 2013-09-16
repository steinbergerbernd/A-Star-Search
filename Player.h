#pragma once
#include "MapObject.h"
#include "WayPoint.h"
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::pair;
using std::string;

class Player : public MapObject
{
public:
	Player(World* world, const HexMap& hexMap, CL_Point position);
	virtual ~Player(void);

	void draw();
	void update(float elapsed);
private:
	map<string, WayPoint*> wayPointsFinalized;
	map<string, WayPoint*> wayPointsToDo;
	WayPoint* currentWayPoint;

	//Weg für Spieler berechnen
	void calculateWayPoints();
	//Speicherfreigabe für die Wegpunkt-Maps
	void freeWayPointMap(map<string, WayPoint*> wayPointMap);
	//Kosten eines Gegners zu Feld summieren
	void addEnemyCosts(string key, WayPoint* wayPoint);
};

