#pragma once

class World;

class GameObject
{
public:
	GameObject(World* world);
	virtual ~GameObject(void);

	virtual void draw() {};
	virtual void update(float elapsed) {}

protected:
	World* world;
};

