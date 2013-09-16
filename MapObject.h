#pragma once
#include "gameobject.h"
#include "HexMap.h"
#include <ClanLib/display.h>

//Basisklasse für ein Objekt auf der Karte
class MapObject : public GameObject
{
public:
	MapObject(World* world, const HexMap& hexMap, CL_Point position, CL_Colorf color);
	virtual ~MapObject(void);

	void draw();
	
	CL_Point getPosition();
protected:
	CL_Point position;
	CL_Colorf color;

	HexMap hexMap;
};