#include "Player.h"
#include "World.h"
#include "WayPoint.h"

//Radius für einen Wegpunkt
const float radius = 20.0f;

Player::Player(World* world, const HexMap& hexMap, CL_Point position) : MapObject(world, hexMap, position, CL_Colorf::white)
{
	currentWayPoint = 0;
}

//Speicher freigeben
Player::~Player(void)
{
	freeWayPointMap(wayPointsToDo);
	freeWayPointMap(wayPointsFinalized);
	wayPointsToDo.clear();
	wayPointsFinalized.clear();
	if(currentWayPoint != 0)
		free(currentWayPoint);
}

//Zeichnen
void Player::draw()
{
	WayPoint* nextWayPoint = currentWayPoint;

	//Alle Wegpunkte durchlaufen und zeichnen
	while(nextWayPoint != 0 && nextWayPoint->position != world->getTarget()->getPosition())
	{
		float x = (float)(nextWayPoint->position.x * (hexMap.getHexagonWidth() - hexMap.getIndenting()) + hexMap.getHexagonWidth() / 2.0f);
		float y = (float)(nextWayPoint->position.y * hexMap.getHexagonHeight() + hexMap.getHexagonHeight() / 2.0f);
		if(nextWayPoint->position.x % 2 == 1)
			y += hexMap.getHexagonHeight() / 2.0f;

		CL_Draw::circle(world->get_gc(), x, y, radius, CL_Colorf::orange);
		nextWayPoint = nextWayPoint->previousWayPoint;
	}

	MapObject::draw();
}

void Player::update(float elapsed)
{
	//Wenn nicht Pause
	if(!world->getPauseState())
	{
		//Wegpunkte löschen
		freeWayPointMap(wayPointsToDo);
		freeWayPointMap(wayPointsFinalized);
		wayPointsToDo.clear();
		wayPointsFinalized.clear();
		if(currentWayPoint != 0)
			free(currentWayPoint);

		//aktuellen Wegpunkt auf das Ziel setzen
		currentWayPoint = new WayPoint();
		currentWayPoint->position = world->getTarget()->getPosition();
		currentWayPoint->previousWayPoint = 0;
		currentWayPoint->costs = currentWayPoint->heuristic = currentWayPoint->sum = 0;
		//aktuellen Wegpunkt in ToDo-Liste einfügen
		wayPointsToDo.insert(pair<string, WayPoint*>(world->pointToString(currentWayPoint->position), currentWayPoint));

		//Wegpunkte berechnen
		calculateWayPoints();
	}
}

//Löschen der Wegpunkte
void Player::freeWayPointMap(map<string, WayPoint*> wayPointMap)
{
	for(map<string, WayPoint*>::iterator it = wayPointMap.begin(); it != wayPointMap.end(); ++it)
	{
		if((*it).second != 0)
		{
			if((*it).second == currentWayPoint)
				currentWayPoint = 0;
			free((*it).second);
			(*it).second = 0;
		}
	}
}

//Wegpunkte berechnen
void Player::calculateWayPoints()
{
	//Bis Startpunkt (Position des Spielers) gefunden wird
	while(currentWayPoint->position != position)
	{
		//aktuellen Knoten von ToDo in Finalized Liste verschieben
		string currentKey = world->pointToString(currentWayPoint->position);
		wayPointsFinalized.insert(pair<string, WayPoint*>(currentKey, currentWayPoint));
		wayPointsToDo.erase(currentKey);

		//Nachbarknoten holen
		map<string, WayPoint*> neighbors = hexMap.getNeighbors(currentWayPoint->position);

		//Nachbarknoten durchlaufen
		for(map<string, WayPoint*>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
		{
			//Wenn Knoten innerhalb der Karte und noch nicht in Finalized-Liste
			if((*it).second->sum != -1 && wayPointsFinalized.find((*it).first) == wayPointsFinalized.end())
			{
				//Nachbarknoten in ToDo-Liste suchen
				map<string, WayPoint*>::iterator toDoWayPoint = wayPointsToDo.find((*it).first);
				//Vorgängerknoten setzen und Kosten berechnen
				(*it).second->previousWayPoint = currentWayPoint;
				(*it).second->costs += currentWayPoint->costs;
				(*it).second->sum += currentWayPoint->sum;
				addEnemyCosts((*it).first, (*it).second);

				//Wenn Knoten noch nicht in ToDo-Liste, dann einfügen
				if(toDoWayPoint == wayPointsToDo.end())
					wayPointsToDo.insert(*it);
				//Wenn Knoten bereits in ToDo-Liste aber billiger, dann ersetzen
				else if((*toDoWayPoint).second->costs > (*it).second->costs)
				{
					free((*toDoWayPoint).second);
					(*toDoWayPoint).second = (*it).second;
				}
				//ansonsten Speicher wieder freigeben
				else
				{
					free((*it).second);
					(*it).second = 0;
				}
			}
			else
			{
				free((*it).second);
				(*it).second = 0;
			}
		}

		int cheapestSum = -1;

		//billigsten Wegpunkt aus ToDo-Liste ermitteln
		for(map<string, WayPoint*>::iterator it = wayPointsToDo.begin(); it != wayPointsToDo.end(); ++it)
		{
			if(cheapestSum == -1 || (*it).second->sum < cheapestSum)
			{
				cheapestSum = (*it).second->sum;
				currentWayPoint = (*it).second;
			}
		}
	}
}

//Kosten eines Gegners zu dem übergebenen Wegpunkt summieren
void Player::addEnemyCosts(string key, WayPoint* wayPoint)
{
	vector<Enemy*> enemies = world->getEnemies();
	for(vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
	{
		map<string, Enemy::EnemyField> enemyFields = (*it)->getEnemyFields();
		map<string, Enemy::EnemyField>::iterator enemyField = enemyFields.find(key);
		if(enemyField != enemyFields.end())
		{
			wayPoint->costs += enemyField->second.costs;
			wayPoint->sum += enemyField->second.costs;
		}
	}
}