#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, 0 };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	ResourceManager& data = ResourceManager::Instance();
	sound_intro = data.GetSound(AudioResource::AUD_INTRO);
	sound_munch1 = data.GetSound(AudioResource::AUD_MUNCH1);
	sound_munch2 = data.GetSound(AudioResource::AUD_MUNCH2);

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
AppStatus Scene::Init()
{

	//Create player
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}

	//Create level 
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);

	PlaySound(sound_intro);

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int* map = nullptr;
	Object *obj;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 0)
	{
		map = new int[size] {
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 2, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 44, 43, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 1,
			 4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 3,
			 4,  0, 40, 15, 15, 39,  0, 40, 15, 15, 15, 39,  0, 25, 26,  0, 40, 15, 15, 15, 39,  0, 40, 15, 15, 39,  0, 3,
			 4,  0, 25,  0,  0, 26,  0, 25,  0,  0,  0, 26,  0, 25, 26,  0, 25,  0,  0,  0, 26,  0, 25,  0,  0, 26,  0, 3,
			 4,  0, 28, 21, 21, 27,  0, 28, 21, 21, 21, 27,  0, 37, 38,  0, 28, 21, 21, 21, 27,  0, 28, 21, 21, 27,  0, 3,
			 4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 3,
			 4,  0, 40, 15, 15, 39,  0, 35, 36,  0, 40, 15, 15, 15, 15, 15, 15, 39,  0, 35, 36,  0, 40, 15, 15, 39,  0, 3,
			 4,  0, 28, 21, 21, 27,  0, 25, 26,  0, 28, 21, 21, 36, 35, 21, 21, 27,  0, 25, 26,  0, 28, 21, 21, 27,  0, 3,
			 4,  0,  0,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0,  0,  0, 3,
			 6, 13, 13, 13, 13, 23,  0, 25, 28, 15, 15, 39,  0, 25, 26,  0, 40, 15, 15, 27, 26,  0, 24, 13, 13, 13, 13, 5,
			 0,  0,  0,  0,  0,  4,  0, 25, 40, 21, 21, 27,  0, 37, 38,  0, 28, 21, 21, 39, 26,  0,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4,  0, 25, 26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 26,  0,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4,  0, 25, 26,  0, 30, 13, 34, 70, 71, 33, 13, 29,  0, 25, 26,  0,  3,  0,  0,  0,  0, 0,
			11, 11, 11, 11, 11, 27,  0, 37, 38,  0,  3,  0,  0,  0,  0,  0,  0,  4,  0, 37, 38,  0, 28, 11, 11, 11, 11, 11,
			-3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0, -2,
			13, 13, 13, 13, 13, 23,  0, 35, 36,  0,  3,  0,  0,  0,  0,  0,  0,  4,  0, 35, 36,  0, 24, 13, 13, 13, 13, 13,
			 0,  0,  0,  0,  0,  4,  0, 25, 26,  0, 32, 11, 11, 11, 11, 11, 11, 31,  0, 25, 26,  0,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4,  0, 25, 26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 26, 0,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4,  0, 25, 26,  0, 40, 15, 15, 15, 15, 15, 15, 39,  0, 25, 26,  0,  3,  0,  0,  0,  0, 0,
			 2, 11, 11, 11, 11, 27,  0, 37, 38,  0, 28, 21, 21, 36, 35, 21, 21, 27,  0, 37, 38,  0, 28, 11, 11, 11, 11, 1,
			 4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 3,
			 4,  0, 40, 15, 15, 39,  0, 40, 15, 15, 15, 39,  0, 25, 26,  0, 40, 15, 15, 15, 39,  0, 40, 15, 15, 39,  0, 3,
			 4,  0, 28, 21, 39, 26,  0, 28, 21, 21, 21, 27,  0, 37, 38,  0, 28, 21, 21, 21, 27,  0, 25, 40, 21, 27,  0, 3,
			 4,  0,  0,  0, 25, 26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 26,  0,  0,  0, 3,
			 8, 15, 39,  0, 25, 26,  0, 35, 36,  0, 40, 15, 15, 15, 15, 15, 15, 39,  0, 35, 36,  0, 25, 26,  0, 40, 15, 7,
			10, 21, 27,  0, 37, 38,  0, 25, 26,  0, 28, 21, 21, 36, 35, 21, 21, 27,  0, 25, 26,  0, 37, 38,  0, 28, 21, 9,
			 4,  0,  0,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0, 25, 26,  0,  0,  0,  0,  0,  0, 3,
			 4,  0, 40, 15, 15, 15, 15, 27, 28, 15, 15, 39,  0, 25, 26,  0, 40, 15, 15, 27, 28, 15, 15, 15, 15, 39,  0, 3,
			 4,  0, 28, 21, 21, 21, 21, 21, 21, 21, 21, 27,  0, 37, 38,  0, 28, 21, 21, 21, 21, 21, 21, 21, 21, 27,  0, 3,
			 4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 3,
			 6, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 5,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0 
		};
	}
	else if (stage == 1)
	{
		map = new int[size] {
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 2, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 44, 43, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 1,
			 4, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 3,
			 4, 50, 40, 15, 15, 39, 50, 40, 15, 15, 15, 39, 50, 25, 26, 50, 40, 15, 15, 15, 39, 50, 40, 15, 15, 39, 50, 3,
			 4, 52, 25,  0,  0, 26, 50, 25,  0,  0,  0, 26, 50, 25, 26, 50, 25,  0,  0,  0, 26, 50, 25,  0,  0, 26, 52, 3,
			 4, 50, 28, 21, 21, 27, 50, 28, 21, 21, 21, 27, 50, 37, 38, 50, 28, 21, 21, 21, 27, 50, 28, 21, 21, 27, 50, 3,
			 4, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 3,
			 4, 50, 40, 15, 15, 39, 50, 35, 36, 50, 40, 15, 15, 15, 15, 15, 15, 39, 50, 35, 36, 50, 40, 15, 15, 39, 50, 3,
			 4, 50, 28, 21, 21, 27, 50, 25, 26, 50, 28, 21, 21, 36, 35, 21, 21, 27, 50, 25, 26, 50, 28, 21, 21, 27, 50, 3,
			 4, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 3,
			 6, 13, 13, 13, 13, 23, 50, 25, 28, 15, 15, 39,  0, 25, 26,  0, 40, 15, 15, 27, 26, 50, 24, 13, 13, 13, 13, 5,
			 0,  0,  0,  0,  0,  4, 50, 25, 40, 21, 21, 27,  0, 37, 38,  0, 28, 21, 21, 39, 26, 50,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4, 50, 25, 26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 26, 50,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4, 50, 25, 26,  0, 30, 13, 34, 70, 71, 33, 13, 29,  0, 25, 26, 50,  3,  0,  0,  0,  0, 0,
			11, 11, 11, 11, 11, 27, 50, 37, 38,  0,  3,  0,  0,  0,  0,  0,  0,  4,  0, 37, 38, 50, 28, 11, 11, 11, 11, 11,
			-3,  0,  0,  0,  0,  0, 50,  0,  0,  0,  3,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0, 50,  0,  0,  0,  0,  0, -2,
			13, 13, 13, 13, 13, 23, 50, 35, 36,  0,  3,  0,  0,  0,  0,  0,  0,  4,  0, 35, 36, 50, 24, 13, 13, 13, 13, 13,
			 0,  0,  0,  0,  0,  4, 50, 25, 26,  0, 32, 11, 11, 11, 11, 11, 11, 31,  0, 25, 26, 50,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4, 50, 25, 26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 26, 50,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4, 50, 25, 26,  0, 40, 15, 15, 15, 15, 15, 15, 39,  0, 25, 26, 50,  3,  0,  0,  0,  0, 0,
			 2, 11, 11, 11, 11, 27, 50, 37, 38,  0, 28, 21, 21, 36, 35, 21, 21, 27,  0, 37, 38, 50, 28, 11, 11, 11, 11, 1,
			 4, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 3,
			 4, 50, 40, 15, 15, 39, 50, 40, 15, 15, 15, 39, 50, 25, 26, 50, 40, 15, 15, 15, 39, 50, 40, 15, 15, 39, 50, 3,
			 4, 50, 28, 21, 39, 26, 50, 28, 21, 21, 21, 27, 50, 37, 38, 50, 28, 21, 21, 21, 27, 50, 25, 40, 21, 27, 50, 3,
			 4, 52, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 50, 100,  0, 50, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 52, 3,
			 8, 15, 39, 50, 25, 26, 50, 35, 36, 50, 40, 15, 15, 15, 15, 15, 15, 39, 50, 35, 36, 50, 25, 26, 50, 40, 15, 7,
			10, 21, 27, 50, 37, 38, 50, 25, 26, 50, 28, 21, 21, 36, 35, 21, 21, 27, 50, 25, 26, 50, 37, 38, 50, 28, 21, 9,
			 4, 50, 50, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 25, 26, 50, 50, 50, 50, 50, 50, 3,
			 4, 50, 40, 15, 15, 15, 15, 27, 28, 15, 15, 39, 50, 25, 26, 50, 40, 15, 15, 27, 28, 15, 15, 15, 15, 39, 50, 3,
			 4, 50, 28, 21, 21, 21, 21, 21, 21, 21, 21, 27, 50, 37, 38, 50, 28, 21, 21, 21, 21, 21, 21, 21, 21, 27, 50, 3,
			 4, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 3,
			 6, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 5,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0 
		};

		player->InitScore();
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}

	//Entities and objects
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
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::DOT)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::DOT);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::PELLET)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::PELLET);
				objects.push_back(obj);
				map[i] = 0;
			}
			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);

	delete[] map;

	return AppStatus::OK;
}
void Scene::Update()
{
	Point p1, p2;
	AABB box;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//insta win/lose
	if (IsKeyPressed(KEY_F2))       EndLevel = true;
	if (IsKeyPressed(KEY_F3))       lose = true;

	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE))		LoadLevel(1);

	if (EndLevel) {
		level_count++;
		level->win = true;
		player->Win();
		win = true;
		LoadLevel(0);
		EndLevel = false;
	}

	if (intro) 
	{
		if (intro_count <= 0) intro = false;
		else 
		{
			player->Intro(intro_count);
			--intro_count;
		}
	}
	else if (win) {
		if (!level->win) {
			win = false;
			if (level_count > (int)LEVELS) {
				EndGame = true;
			}
			else {
				LoadLevel(level_count);
			}
		}
	}
	else if (lose) {
		player->Lose();
		if (!player->lose) {
			lose = false;
			if (player->GetLives() > 0) {
				LoadLevel(level_count);
			}
			else {
				EndGame = true;
			}
		}
	}
	else {
		level->Update();
		player->Update();
		CheckCollisions();
	}
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		player->DrawPlayer();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	player->Release();
	ClearLevel();
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box;
	int count = 0;
	
	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		count++;
		obj_box = (*it)->GetHitbox();
		if (player_box.TestAABB(obj_box))
		{
			player->IncrScore((*it)->Points());
			if ((*it)->Sounds() == (int)ObjectType::DOT) 
			{
				if (munch1) 
				{
					PlaySound(sound_munch1);
					munch1 = false;
				}
				else 
				{
					PlaySound(sound_munch2);
					munch1 = true;
				}
			}

			//Delete the object
			delete* it;
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it);
		}
		else
		{
			//Move to the next object
			++it;
		}
	}
	if (count == 0) {
		EndLevel = true;
		count = 0;
	}
	
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	//Temporal approach
	DrawText(TextFormat("SCORE : %d", player->GetScore()), 10, 10, 8, LIGHTGRAY);
	DrawText(TextFormat("LIVES : %d", player->GetLives()), 10, WINDOW_HEIGHT-10, 8, LIGHTGRAY);
}