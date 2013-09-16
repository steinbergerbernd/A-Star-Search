#pragma once
#include "gameobject.h"
#include "WayPoint.h"
#include <map>
#include <vector>
#include <string>
#include <ClanLib/display.h>

using std::map;
using std::vector;
using std::string;
using std::pair;

//Klasse für die Karte
class HexMap : public GameObject
{
public:
	//Geländetypen
	enum FieldType { forest, grass, water, mountain };

	HexMap(World* world);
	virtual ~HexMap(void);

	void draw();
	void update(float elapsed);

	//Nachbarn in Form von WayPoint-Zeigern liefern
	map<string, WayPoint*> getNeighbors(CL_Point position);
	//Nachbarn in Form von CL_Point-Objekten liefern
	map<string, CL_Point> getNeighborPoints(CL_Point position);

	int getHexagonWidth();
	int getHexagonHeight();
	int getMapWidth();
	int getMapHeight();
	int getIndenting();
private:
	vector<vector<FieldType>> fields;

	pair<string, WayPoint*> createNeighborPair(CL_Point position);
	pair<string, CL_Point> createNeighborPointPair(CL_Point position);

	CL_Colorf getColorOfFieldType(HexMap::FieldType fieldType);
	int getCostsOfFieldType(HexMap::FieldType fieldType);
};

