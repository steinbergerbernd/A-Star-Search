#pragma once
#include <ClanLib/display.h>

//Struktur für einen Wegpunkt
struct WayPoint
{
	WayPoint(void) { };
	WayPoint(CL_Point position) : position(position) { };
	CL_Point position;
	WayPoint* previousWayPoint;
	int heuristic;
	int costs;
	int sum;
};