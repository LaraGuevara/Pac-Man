#include "Intro.h"
#include <stdio.h>
#include "Globals.h"

Intro::Intro() 
{
	PacMan = nullptr;
	Blinky = nullptr;
	Pinky = nullptr;
	Inky = nullptr;
	/*Clyde = nullptr;*/
	Dot = nullptr;
	introScene = nullptr;

	camera.target = { 0, 0 };				
	camera.offset = { 0, 0 };	
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
}

Intro::~Intro()
{
	if (PacMan != nullptr) {
		PacMan->Release();
		delete PacMan;
		PacMan = nullptr;
	}
	if (Blinky != nullptr) {
		Blinky->Release();
		delete Blinky;
		Blinky = nullptr;
	}
	if (Inky != nullptr) {
		Inky->Release();
		delete Inky;
		Inky = nullptr;
	}
	if (Pinky != nullptr) {
		Pinky->Release();
		delete Pinky;
		Pinky = nullptr;
	}
	/*if (Clyde != nullptr) {
		Clyde->Release();
		delete Clyde;
		Clyde = nullptr;
	}*/
	if (Dot != nullptr) {
		delete Dot;
		Dot = nullptr;
	}
	if (introScene != nullptr) {
		introScene->Release();
		delete introScene;
		introScene = nullptr;
	}
}

AppStatus Intro::Init() 
{
	PacMan = new Player({ 0,0 }, State::IDLE, Look::LEFT);
	if (PacMan == nullptr)
	{
		LOG("Failed to allocate memory for intro PacMan");
		return AppStatus::ERROR;
	}
	Inky = new Enemy({ 0,0 }, State_e::IDLE, Look_e::LEFT, EnemyType::INKY);
	if (Inky == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}
	Blinky = new Enemy({ 0,0 }, State_e::IDLE, Look_e::LEFT, EnemyType::BLINKY);
	if (Blinky == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}
	Pinky = new Enemy({ 0,0 }, State_e::IDLE, Look_e::LEFT, EnemyType::PINKY);
	if (Pinky == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}
	/*Clyde = new Enemy({ 0,0 }, State_e::IDLE, Look_e::LEFT, EnemyType::CLYDE);
	if (Clyde == nullptr)
	{
		LOG("Failed to allocate memory for intro enemy");
		return AppStatus::ERROR;
	}*/

	if(PacMan->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro pacman");
		return AppStatus::ERROR;
	}
	if (Inky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}
	if (Blinky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}
	/*if (Clyde->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}*/
	if (Pinky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise intro enemy");
		return AppStatus::ERROR;
	}

	introScene = new TileMap();
	if (introScene == nullptr)
	{
		LOG("Failed to allocate memory for Level");
		return AppStatus::ERROR;
	}
	if (introScene->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	if (LoadIntro() != AppStatus::OK)
	{
		LOG("Failed to load intro");
		return AppStatus::ERROR;
	}
	
	PacMan->SetTileMap(introScene);

	return AppStatus::OK;
}

AppStatus Intro::LoadIntro() 
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int* map = nullptr;
	size = LEVEL_WIDTH * LEVEL_HEIGHT;

	map = new int[size] {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE + TILE_SIZE / 2;
				playerX = pos.x;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				playerY = pos.y;
				PacMan->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::PELLET)
			{
				pos.x = x * TILE_SIZE;
				dotX = pos.x;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				dotY = pos.y;
				Dot = new Object(pos, ObjectType::PELLET);
				map[i] = 0;
			}
			++i;
		}
	}
	
	introScene->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	delete[] map;
	return AppStatus::OK;
}

void Intro::Update() 
{

	if (end) {
		turn = false;
		PacMan->SetPos({ playerX, playerY });
		Dot = new Object({dotX, dotY}, ObjectType::PELLET);
		isDot = true;

		Blinky->introCaught = false;
		isBlinky = false;
		Pinky->introCaught = false;
		isPinky = false;
		Inky->introCaught = false;
		isInky = false;
		/*Clyde->introCaught = false;
		isClyde = false;*/

		end = false;
		timer = 0;

		if (loop) {
			loop = false;
			loopCheck = true;
		}
	}
	else {
		Point p1, p2;
		AABB box;

		if (timer == 10) {
			Blinky->SetPos({ playerX, playerY });
			isBlinky = true;
		}
		if (timer == 20) {
			Pinky->SetPos({ playerX, playerY });
			isPinky = true;
		}
		if (timer == 30) {
			Inky->SetPos({ playerX, playerY });
			isInky = true;
		}
		/*if (timer == 40) {
			Clyde->SetPos({ playerX, playerY });
			isClyde = true;
		}*/

		end = PacMan->IntroUpdate(turn);
		if (end) loop = true;

		if (isBlinky) Blinky->IntroUpdate(turn);
		if (isInky) Inky->IntroUpdate(turn);
		if (isPinky) Pinky->IntroUpdate(turn);
		/*if (isClyde) Clyde->IntroUpdate(turn);*/

		CheckCollisions();
		timer++;
	}
}

void Intro::Render() 
{
	BeginMode2D(camera);
	introScene->Render();
	PacMan->DrawPlayer();
	Blinky->DrawPlayer();
	Inky->DrawPlayer();
	Pinky->DrawPlayer();
	/*Clyde->DrawPlayer();*/
	if(isDot)Dot->Draw();
	EndMode2D();
}

void Intro::CheckCollisions()
{
	AABB player_box, obj_box, enemy_box;

	player_box = PacMan->GetHitbox();

	if (isDot) {
		obj_box = Dot->GetHitbox();
		if (player_box.TestAABB(obj_box)) {
			delete Dot;
			turn = true;
			isDot = false;
		}
	}

	/*if (isClyde) {
		enemy_box = Clyde->GetHitbox();
		if (player_box.TestAABB(enemy_box)) Clyde->introCaught = true;
	}*/

	if (isInky) {
		enemy_box = Inky->GetHitbox();
		if (player_box.TestAABB(enemy_box)) Inky->introCaught = true;
	}

	if (isPinky) {
		enemy_box = Pinky->GetHitbox();
		if (player_box.TestAABB(enemy_box)) Pinky->introCaught = true;
	}
	
	if (isBlinky) {
		enemy_box = Blinky->GetHitbox();
		if (player_box.TestAABB(enemy_box)) Blinky->introCaught = true;
	}
}

void Intro::Release() 
{
	PacMan->Release();
	Inky->Release();
	Blinky->Release();
	Pinky->Release();
	/*Clyde->Release();*/
	introScene->Release();
}