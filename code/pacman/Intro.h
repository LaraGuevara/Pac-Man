#pragma once
#include <raylib.h>
#include "Player.h"
#include "Enemys.h"
#include "TileMap.h"
#include "Object.h"
#include "Text.h"

class Intro 
{
public:
	Intro();
	~Intro();

	AppStatus Init();
	void Update();
	void Render();
	void Release();

	bool loopCheck = false;

private:
	AppStatus LoadIntro();
	void CheckCollisions();

	int timer = 0;

	bool turn = false;
	bool end = true;
	bool loop = false;

	bool isPinky = false;
	bool isInky = false;
	bool isBlinky = false;
	bool isClyde = false;
	bool isDot = true;

	Player* PacMan;

	Enemy* Blinky;
	Enemy* Inky;
	Enemy* Pinky;
	Enemy* Clyde;

	Object* Dot;

	Camera2D camera;
	TileMap* introScene;
	int playerX, playerY;
	int dotX, dotY;
};