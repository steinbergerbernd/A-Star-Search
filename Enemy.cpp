#include "Enemy.h"
#include "World.h"

//Radius für die Kreise, welche die umliegenden Gegnerfelder markieren
const float radius = 10.0f;

Enemy::Enemy(World* world, const HexMap& hexMap, EnemyType enemyType, vector<CL_Point> controlPoints)
	: MapObject(world, hexMap, controlPoints[0], CL_Colorf::red), enemyType(enemyType)
{
	this->controlPoints = controlPoints;
	elapsedLastMove = 0.0f;
	currentMoveIndex = 0;
	findEnemyFields(0, position);
}

Enemy::~Enemy(void)
{
}

//Zeichnen
void Enemy::draw()
{
	//Alle vom Gegner eingenommenen Felder durchlaufen
	for(map<string, EnemyField>::iterator it = enemyFields.begin(); it != enemyFields.end(); ++it)
	{
		//Wenn das Feld innerhalb der Karte ist und es sich nicht um die aktuelle Gegnerposition handelt --> kleinen Kreis zeichnen
		if((*it).second.position.x >= 0 && (*it).second.position.y >= 0 &&
			(*it).second.position.x < hexMap.getMapWidth() && (*it).second.position.y < hexMap.getMapHeight()
			&& (*it).second.position != position)
		{
			float x = (float)((*it).second.position.x * (hexMap.getHexagonWidth() - hexMap.getIndenting()) + hexMap.getHexagonWidth() / 2.0f);
			float y = (float)((*it).second.position.y * hexMap.getHexagonHeight() + hexMap.getHexagonHeight() / 2.0f);
			if((*it).second.position.x % 2 == 1)
				y += hexMap.getHexagonHeight() / 2.0f;

			CL_Draw::circle(world->get_gc(), x, y, radius, color);
		}
	}

	//Basisklassenaufruf --> Gegnerposition mit größerem Kreis zeichnen
	MapObject::draw();
}

void Enemy::update(float elapsed)
{
	//Wenn nicht Pause
	if(!world->getPauseState())
	{
		//vergangene Zeit summieren
		elapsedLastMove += elapsed;
		//benötigte Zeit für einen Schritt holen
		float moveSpeed = getMoveSpeed();

		//Wenn wieder ein Schritt auszuführen ist
		if(elapsedLastMove > moveSpeed)
		{
			//Anzahl zu gehender Schritte
			int moveCount = (int)(elapsedLastMove / moveSpeed);

			//Neuen Index berechnen
			currentMoveIndex = (currentMoveIndex + moveCount) % controlPoints.size();

			//Neue Position setzen
			position = controlPoints[currentMoveIndex];
			//Neue vom Gegner eingenommene Felder berechnen
			enemyFields.clear();
			findEnemyFields(0, position);

			elapsedLastMove -= moveSpeed * moveCount;
		}
	}
}

map<string, Enemy::EnemyField> Enemy::getEnemyFields()
{
	return enemyFields;
}

//vom Gegner eingenommene Felder berechnen
void Enemy::findEnemyFields(int radius, CL_Point fieldPosition)
{
	if(radius < getAttackRadius())
	{
		//Nachbarfelder holen und durchlaufen
		map<string, CL_Point> neighbors = hexMap.getNeighborPoints(fieldPosition);
		for(map<string, CL_Point>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
		{
			//Rekursion für den Radius aufrufen
			findEnemyFields(radius + 1, (*it).second);
			if(enemyFields.find((*it).first) == enemyFields.end())
				enemyFields.insert(createEnemyFieldPair((*it).first, (*it).second));
		}
	}
}

pair<string, Enemy::EnemyField> Enemy::createEnemyFieldPair(string key, CL_Point fieldPosition)
{
	EnemyField enemyField = EnemyField();
	enemyField.position = fieldPosition;
	int distance = abs(position.x - fieldPosition.x) + abs(position.y - fieldPosition.y);
	enemyField.costs = getCosts() / (distance + 1);

	return pair<string, Enemy::EnemyField>(key, enemyField);
}

//Kosten für den Gegnertyp
int Enemy::getCosts()
{
	switch(enemyType)
	{
		case soldier:
			return 100;
		case tank:
			return 200;
		case helicopter:
			return 300;
		default:
			return 0;
	}
}

//Angriffsradius des Gegners
int Enemy::getAttackRadius()
{
	switch(enemyType)
	{
		case soldier:
			return 1;
		case tank:
			return 2;
		case helicopter:
			return 3;
		default:
			return -1;
	}
}

//benötigte Zeit für einen Zug
float Enemy::getMoveSpeed()
{
	switch(enemyType)
	{
		case soldier:
			return 3.0f;
		case tank:
			return 2.0f;
		case helicopter:
			return 1.5f;
		default:
			return -1.0f;
	}
}