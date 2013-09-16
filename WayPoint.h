#pragma once
#include <ClanLib/display.h>

//Struktur f�r einen Wegpunkt
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