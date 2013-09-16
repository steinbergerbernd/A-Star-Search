#include "World.h"
#include <ClanLib/display.h>
#include <ClanLib/core.h>

World::World(CL_DisplayWindow &displayWindow) : window(displayWindow)
{
	graphicContext = window.get_gc();
	
	//Initialisierung der Objekte
	pause = false;

	hexMap = new HexMap(this);
	player = new Player(this, *hexMap, CL_Point(0, 8));
	target = new Target(this, *hexMap, CL_Point(19, 4));
	
	//Gegner erzeugen
	enemies = vector<Enemy*>();
	enemies.push_back(createSoldier());
	enemies.push_back(createHelicopter());
	enemies.push_back(createTank());

	addObject(hexMap);
	addObject(player);
	addObject(target);
	for(vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
		addObject(*it);
}

World::~World(void)
{
	// Löschen aller GameObjects
	std::list<GameObject*>::iterator it;
	for(it = objects.begin(); it != objects.end(); ++it)
		delete (*it);
}

//Soldaten erzeugen
Enemy* World::createSoldier()
{
	vector<CL_Point> controlPoints = vector<CL_Point>();

	controlPoints.push_back(CL_Point(1,1));
	controlPoints.push_back(CL_Point(1,2));
	controlPoints.push_back(CL_Point(1,3));
	controlPoints.push_back(CL_Point(1,2));
	
	return new Enemy(this, *hexMap, Enemy::soldier, controlPoints);
}

//Panzer erzeugen
Enemy* World::createTank()
{
	vector<CL_Point> controlPoints = vector<CL_Point>();

	controlPoints.push_back(CL_Point(15,0));
	controlPoints.push_back(CL_Point(15,1));
	controlPoints.push_back(CL_Point(15,2));
	controlPoints.push_back(CL_Point(15,3));
	controlPoints.push_back(CL_Point(15,4));
	controlPoints.push_back(CL_Point(15,5));
	controlPoints.push_back(CL_Point(15,6));
	controlPoints.push_back(CL_Point(15,7));
	controlPoints.push_back(CL_Point(15,8));
	controlPoints.push_back(CL_Point(15,9));

	controlPoints.push_back(CL_Point(16,9));
	controlPoints.push_back(CL_Point(16,8));
	controlPoints.push_back(CL_Point(16,7));
	controlPoints.push_back(CL_Point(16,6));
	controlPoints.push_back(CL_Point(16,5));
	controlPoints.push_back(CL_Point(16,4));
	controlPoints.push_back(CL_Point(16,3));
	controlPoints.push_back(CL_Point(16,2));
	controlPoints.push_back(CL_Point(16,1));
	controlPoints.push_back(CL_Point(16,0));

	return new Enemy(this, *hexMap, Enemy::tank, controlPoints);
}

//Helikopter erzeugen
Enemy* World::createHelicopter()
{
	vector<CL_Point> controlPoints = vector<CL_Point>();

	controlPoints.push_back(CL_Point(8,0));
	controlPoints.push_back(CL_Point(8,1));
	controlPoints.push_back(CL_Point(8,2));
	controlPoints.push_back(CL_Point(8,3));
	controlPoints.push_back(CL_Point(8,4));
	controlPoints.push_back(CL_Point(8,5));
	controlPoints.push_back(CL_Point(8,6));
	controlPoints.push_back(CL_Point(8,7));
	controlPoints.push_back(CL_Point(8,8));
	controlPoints.push_back(CL_Point(8,9));

	controlPoints.push_back(CL_Point(9,9));
	controlPoints.push_back(CL_Point(9,8));
	controlPoints.push_back(CL_Point(9,7));
	controlPoints.push_back(CL_Point(9,6));
	controlPoints.push_back(CL_Point(9,5));
	controlPoints.push_back(CL_Point(9,4));
	controlPoints.push_back(CL_Point(9,3));
	controlPoints.push_back(CL_Point(9,2));
	controlPoints.push_back(CL_Point(9,1));
	controlPoints.push_back(CL_Point(9,0));

	return new Enemy(this, *hexMap, Enemy::helicopter, controlPoints);
}

// Objekt in den Container legen
void World::addObject(GameObject *object)
{
	objects.push_back(object);
}

void World::run(void)
{
	//Event für Tastatur
	CL_Slot slotKeyDown = window.get_ic().get_keyboard().sig_key_down().connect(this, &World::onKeyDown);

	//Bei Escape Schleife abbrechen und Programm beenden
	while (!window.get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE))
	{
		update();
		draw();

		window.flip(1);
		CL_KeepAlive::process();
	}
}

void World::onKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
	//Pausemodus umschalten
	if(key.id == CL_KEY_SPACE)
		pause = !pause;
}

bool World::getPauseState()
{
	return pause;
}

vector<Enemy*> World::getEnemies()
{
	return enemies;
}

Target* World::getTarget()
{
	return target;
}

Player* World::getPlayer()
{
	return player;
}

// Vergangene Zeit berechnen
float World::getElapsedTime()
{
	static unsigned int lastTime = 0;

	unsigned int newTime = CL_System::get_time();

	if (lastTime == 0)
		lastTime = newTime;

	int deltaTime = (newTime - lastTime);

	lastTime = newTime;

	return deltaTime * 0.001f;
}

//Aus CL_Point Objekt einen String in der Form "x;y" erzeugen
string World::pointToString(CL_Point point)
{
	stringstream stringConverter;

	stringConverter << point.x << ";" << point.y;

	return stringConverter.str();
}

//Updates ausführen
void World::update()
{	
	//Vergangene Zeit berechnen
	float elapsed = getElapsedTime();

	//Update aller GameObjects aufrufen
	std::list<GameObject *>::iterator it;
	for (it = objects.begin(); it != objects.end(); )
	{
		(*it)->update(elapsed);
		++it;
	}
}

// Zeichnen aller Komponenten
void World::draw()
{
	CL_Draw::fill(graphicContext, window.get_viewport(), CL_Colorf::black);

	std::list<GameObject *>::iterator it;
	for(it = objects.begin(); it != objects.end(); ++it)
		(*it)->draw();
}