#pragma once
#include "mapobject.h"

//Ziel für den Spieler
class Target : public MapObject
{
public:
	Target(World* world, const HexMap& hexMap, CL_Point position);
	virtual ~Target(void);
};

