#pragma once
#include "Object.h"
#include "Camera.h"

// Define the player class
class Player : public Game::Object
{
public:
	void SelfUpdate(float dt);
	int health = 100;
	int maxHealth = health;
	float speed = 10;
	float jumpHeight = 10;
	Camera* camera = nullptr;
private:
	std::string name = "Cal";
};