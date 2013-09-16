#include "HexMap.h"
#include "World.h"

#include <limits.h>

const int hexagonWidth = 60;
const int hexagonHeight = 60;
const int mapWidth = 20;
const int mapHeight = 10;
//Einrückung für die Schrägen der Hexagons
const int indenting = 12;

//Initalisierung der Karte
HexMap::HexMap(World* world) : GameObject(world)
{
	fields = vector<vector<FieldType>>(mapHeight, vector<FieldType>(mapWidth, grass));

	fields[9][3] = water;
	fields[8][3] = water;
	fields[7][3] = water;
	fields[6][3] = water;
	fields[5][3] = water;
	fields[5][4] = water;
	fields[4][5] = water;
	fields[5][6] = water;
	fields[5][7] = water;
	fields[6][7] = water;
	fields[7][6] = water;
	fields[7][5] = water;
	fields[8][5] = water;
	fields[9][6] = water;
	fields[9][7] = water;

	fields[5][0] = mountain;
	fields[5][1] = mountain;
	fields[6][0] = mountain;
	fields[6][1] = mountain;
	fields[7][0] = mountain;

	fields[2][5] = mountain;
	fields[2][6] = mountain;
	fields[2][7] = mountain;
	fields[3][5] = mountain;
	fields[3][6] = mountain;
	fields[3][7] = mountain;
	fields[4][6] = mountain;
	fields[4][7] = mountain;

	fields[0][2] = forest;
	fields[0][3] = forest;
	fields[0][4] = forest;
	fields[1][3] = forest;
	fields[1][4] = forest;
	
	fields[3][3] = forest;
	fields[3][4] = forest;
	fields[4][3] = forest;
	fields[4][4] = forest;
}

HexMap::~HexMap(void)
{
}

int HexMap::getHexagonWidth()
{
	return hexagonWidth;
}

int HexMap::getHexagonHeight()
{
	return hexagonHeight;
}

int HexMap::getMapHeight()
{
	return mapHeight;
}

int HexMap::getMapWidth()
{
	return mapWidth;
}

int HexMap::getIndenting()
{
	return indenting;
}

//Zeichnen
void HexMap::draw()
{
	CL_GraphicContext gc = world->get_gc();
	CL_Pointf positions[6];
	CL_Pointf center;
	float xPosition, yPosition;
	CL_Colorf fieldColor;

	//Höhe für Zeilen durchlaufen
	for(int i = 0; i < mapHeight; ++i)
	{
		//Breite für Spalten durchlaufen
		for(int j = 0; j < mapWidth; ++j)
		{
			xPosition = (float)(j * (hexagonWidth - indenting));

			yPosition = (float)(hexagonHeight * i);

			//Bei jeder zweiten Spalte muss das Hexagon um ein halbes Hexagon nach unten verschoben werden
			if(j % 2 == 1)
				yPosition += hexagonHeight / 2.0f;

			//Eckpunkte des Hexagons setzen
			positions[0] = CL_Pointf(xPosition + indenting, yPosition);
			positions[1] = CL_Pointf(xPosition + hexagonWidth - indenting, yPosition);
			positions[2] = CL_Pointf(xPosition + hexagonWidth, yPosition + hexagonHeight / 2.0f);
			positions[3] = CL_Pointf(xPosition + hexagonWidth - indenting, yPosition + hexagonHeight);
			positions[4] = CL_Pointf(xPosition + indenting, yPosition + hexagonHeight);
			positions[5] = CL_Pointf(xPosition, yPosition + hexagonHeight / 2.0f);

			//Mittelpunkt des Hexagons
			center = CL_Pointf(xPosition + hexagonWidth / 2.0f, yPosition + hexagonHeight / 2.0f);
			fieldColor = getColorOfFieldType(fields[i][j]);

			//Eckpunkte durchlaufen und Linien und gefüllte Dreiecke zum Mittelpunkt hin zeichnen
			for(int k = 0; k < 6; ++k)
			{
				CL_Draw::triangle(gc, positions[k], positions[(k+1)%6], center, fieldColor);
				CL_Draw::line(gc, positions[k], positions[(k+1)%6], CL_Colorf::white);
			}
		}
	}
}

pair<string, WayPoint*> HexMap::createNeighborPair(CL_Point position)
{
	return pair<string, WayPoint*>(world->pointToString(position), new WayPoint(position));
}

pair<string, CL_Point> HexMap::createNeighborPointPair(CL_Point position)
{
	return pair<string, CL_Point>(world->pointToString(position), position);
}

//Nachbarpunkte in Form von CL_Point-Objekten berechnen
map<string, CL_Point> HexMap::getNeighborPoints(CL_Point position)
{
	int x = position.x;
	int y = position.y;
	int nextX = (x + 1) % mapWidth;

	map<string, CL_Point> neighborPoints = map<string, CL_Point>();

	//Positionen berechnen
	neighborPoints.insert(createNeighborPointPair(CL_Point(x + 1, y)));
	neighborPoints.insert(createNeighborPointPair(CL_Point(x, y + 1)));
	neighborPoints.insert(createNeighborPointPair(CL_Point(x - 1, y)));
	neighborPoints.insert(createNeighborPointPair(CL_Point(x, y - 1)));

	if(x % 2 == 0)
	{
		neighborPoints.insert(createNeighborPointPair(CL_Point(x + 1, y - 1)));
		neighborPoints.insert(createNeighborPointPair(CL_Point(x - 1, y - 1)));
	}
	else
	{
		neighborPoints.insert(createNeighborPointPair(CL_Point(x + 1, y + 1)));
		neighborPoints.insert(createNeighborPointPair(CL_Point(x - 1, y + 1)));
	}

	return neighborPoints;
}

//Nachbarpunkte in Form von WayPoint-Zeigern berechnen
map<string, WayPoint*> HexMap::getNeighbors(CL_Point position)
{
	int x = position.x;
	int y = position.y;
	int nextX = (x + 1) % mapWidth;

	map<string, WayPoint*> neighbors = map<string, WayPoint*>();

	//Positionen berechnen
	neighbors.insert(createNeighborPair(CL_Point(x + 1, y)));
	neighbors.insert(createNeighborPair(CL_Point(x, y + 1)));
	neighbors.insert(createNeighborPair(CL_Point(x - 1, y)));
	neighbors.insert(createNeighborPair(CL_Point(x, y - 1)));
	
	if(x % 2 == 0)
	{
		neighbors.insert(createNeighborPair(CL_Point(x + 1, y - 1)));
		neighbors.insert(createNeighborPair(CL_Point(x - 1, y - 1)));
	}
	else
	{
		neighbors.insert(createNeighborPair(CL_Point(x + 1, y + 1)));
		neighbors.insert(createNeighborPair(CL_Point(x - 1, y + 1)));
	}

	CL_Point playerPosition = world->getPlayer()->getPosition();

	//Kosten, Heuristik und Summe für die Felder berechnen
	for(map<string, WayPoint*>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
	{
		(*it).second->previousWayPoint = 0;
		if((*it).second->position.x >= 0 && (*it).second->position.y >= 0 && (*it).second->position.x < mapWidth && (*it).second->position.y < mapHeight)
		{
			(*it).second->costs = getCostsOfFieldType(fields[(*it).second->position.y][(*it).second->position.x]);
			(*it).second->heuristic = abs((*it).second->position.x - playerPosition.x) + abs((*it).second->position.y - playerPosition.y);
			(*it).second->sum = (*it).second->costs + (*it).second->heuristic;
		}
		else
			(*it).second->costs = (*it).second->heuristic = (*it).second->sum = -1;
	}

	return neighbors;
}

//Farbe für einen Geländetyp
CL_Colorf HexMap::getColorOfFieldType(HexMap::FieldType fieldType)
{
	switch(fieldType)
	{
		case forest:
			return CL_Colorf::forestgreen;
		case water:
			return CL_Colorf::blue;
		case mountain:
			return CL_Colorf::chocolate;
		case grass:
			return CL_Colorf::yellowgreen;
		default:
			return CL_Colorf::black;
	}
}

//Kosten für einen Geländetyp
int HexMap::getCostsOfFieldType(HexMap::FieldType fieldType)
{
	switch(fieldType)
	{
		case forest:
			return 20;
		case water:
			return 100;
		case mountain:
			return 200;
		case grass:
			return 5;
		default:
			return 1000;
	}
}

void HexMap::update(float elapsed)
{
}