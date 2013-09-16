#include "MapObject.h"
#include "World.h"

//Radius für den Kreis des Objektes
const float radius = 20.0f;

MapObject::MapObject(World* world, const HexMap& hexMap, CL_Point position, CL_Colorf color)
	: GameObject(world), hexMap(hexMap), position(position), color(color)
{
}


MapObject::~MapObject(void)
{
}

CL_Point MapObject::getPosition()
{
	return position;
}

//Zeichnen des Objektes in Form eines Kreises
void MapObject::draw()
{
	float x = (float)(position.x * (hexMap.getHexagonWidth() - hexMap.getIndenting()) + hexMap.getHexagonWidth() / 2.0f);
	float y = (float)(position.y * hexMap.getHexagonHeight() + hexMap.getHexagonHeight() / 2.0f);
	if(position.x % 2 == 1)
		y += hexMap.getHexagonHeight() / 2.0f;

	CL_Draw::circle(world->get_gc(), x, y, radius, color);
}