#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	player = nullptr;

	blinky = nullptr;
	inky = nullptr;

    level = nullptr;

	livesUI = nullptr;

	font = nullptr;
	
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, 0 };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	ResourceManager& data = ResourceManager::Instance();
	sound_intro = data.GetSound(AudioResource::AUD_INTRO);

	sound_munch1 = data.GetSound(AudioResource::AUD_MUNCH1);
	sound_munch2 = data.GetSound(AudioResource::AUD_MUNCH2);

	sound_pellet = data.GetSound(AudioResource::AUD_PELLET);
	sound_fruit = data.GetSound(AudioResource::AUD_FRUIT);

	sirens[0] = data.GetSound(AudioResource::AUD_SIREN1);
	sirens[1] = data.GetSound(AudioResource::AUD_SIREN2);
	sirens[2] = data.GetSound(AudioResource::AUD_SIREN3);
	sirens[3] = data.GetSound(AudioResource::AUD_SIREN4);
	sirens[4] = data.GetSound(AudioResource::AUD_SIREN5);

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	StopSound(sirens[siren]);
	if(IsSoundPlaying(sound_pellet)) StopSound(sound_pellet);

	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
	if (blinky != nullptr)
	{
		blinky->Release();
		delete blinky;
		blinky = nullptr;
	}
	if (inky != nullptr)
	{
		inky->Release();
		delete inky;
		inky = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	if (livesUI != nullptr) {
		livesUI->Release();
		delete livesUI;
		livesUI = nullptr;
	}
	if (font != nullptr) {
		delete font;
		font = nullptr;
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
	//cREATE ENEMY (BLUE)
	inky = new Enemy({0,0}, State_e::IDLE, Look_e::RIGHT, EnemyType::INKY);
	if (inky == nullptr)
	{
		LOG("Failed to allocate memory for enemy");
		return AppStatus::ERROR;
	}
	//cREATE ENEMY (RED)
	blinky = new Enemy({ 0,0 }, State_e::IDLE, Look_e::LEFT, EnemyType::BLINKY);
	if (blinky == nullptr)
	{
		LOG("Failed to allocate memory for enemy");
		return AppStatus::ERROR;
	}
	//create UI
	livesUI = new UI({10, (WINDOW_HEIGHT)});

	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}
	//initialize enemies

	if (inky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy");
		return AppStatus::ERROR;
	}
	if (blinky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy");
		return AppStatus::ERROR;
	}
	//initialize UI
	if (livesUI->Initialise() != AppStatus::OK) {
		LOG("Failed to initialise lives UI");
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
	inky->SetTileMap(level);
	blinky->SetTileMap(level);
	PlaySound(sound_intro);

	font = new Text();
	if (font == nullptr) {
		LOG("Failed to allocate memory for font");
		return AppStatus::ERROR;
	}
	if (font->Initialise(Resource::IMG_FONT, "Resources/game_sprites/Arcade - Pac-Man - font.png", ' ', 8) != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}

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
	else if (stage == 1 or stage == 2)
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
			 0,  0,  0,  0,  0,  4, 50, 25, 26,  103,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25, 26, 50,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4, 50, 25, 26,  0, 30, 13, 34, 70, 71, 33, 13, 29,  0, 25, 26, 50,  3,  0,  0,  0,  0, 0,
			11, 11, 11, 11, 11, 27, 50, 37, 38,  0,  3,  0,  0,  0,  0,  0,  0,  4,  0, 37, 38, 50, 28, 11, 11, 11, 11, 11,
			-3,  0,  0,  0,  0,  0, 50,  0,  0,  0,  3,  0,  0, 0,  0,  0,  0,  4,  0,  0,  0, 50,  0,  0,  0,  0,  0, -2,
			13, 13, 13, 13, 13, 23, 50, 35, 36,  0,  3,  0,  0,  0,  0,  0,  0,  4,  0, 35, 36, 50, 24, 13, 13, 13, 13, 13,
			 0,  0,  0,  0,  0,  4, 50, 25, 26,  0, 32, 11, 11, 11, 11, 11, 11, 31,  0, 25, 26, 50,  3,  0,  0,  0,  0, 0,
			 0,  0,  0,  0,  0,  4, 50, 25, 26,  0,  0,  0,  0, 53,  0,  0,  0,  0,  101, 25, 26, 50,  3,  0,  0,  0,  0, 0,
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
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  200, 0, 0,
			 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0 
		};

		if(stage == 1) player->InitScore();
		if (stage == 2) levelintro = true;
		siren = 0;
		fruitcounter = FRUITTIME;
		collectPellet = false;
		pellet_timer = PELLETTIME;
		if (IsSoundPlaying(sound_pellet)) StopSound(sound_pellet);
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
				playerX = pos.x;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				playerY = pos.y;
				player->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::INKY)
			{
				pos.x = x * TILE_SIZE + TILE_SIZE - 5;
				inkyX = pos.x;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				inkyY = pos.y;
				inky->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::BLINKY)
			{
				pos.x = x * TILE_SIZE - 2;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				blinkyX = pos.x;
				blinkyY = pos.y;
				blinky->SetPos(pos);
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
			else if (tile == Tile::FRUIT)
			{
				pos.x = x * TILE_SIZE + TILE_SIZE / 2;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				fruitX = pos.x;
				fruitY = pos.y;
				map[i] = 0;
			}
			else if (tile == Tile::FRUIT_ICON)
			{
				if(level_count == 1) map[i] = (int)Tile::FRUIT_ICON_1;
				if (level_count == 2) map[i] = (int)Tile::FRUIT_ICON_2;
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
	if (IsKeyPressed(KEY_F5)) {
		int objectX, objectY;
		bool checkTile = true;
		while (checkTile) {
			objectX = (int)GetRandomValue(7, LEVEL_WIDTH - 7);
			objectY = (int)GetRandomValue(7, LEVEL_HEIGHT - 7);
			AABB box({ objectX, objectY }, TILE_SIZE, TILE_SIZE);
			if (objectX <= 16 and objectX >= 11) {
				if (objectY >= 16 and objectY <= 18) {
					checkTile = true;
				}
				else {
					if (!level->SolidTest(box)) checkTile = false;
				}
			}
			else {
				if (!level->SolidTest(box)) checkTile = false;
			}
		}
		objectX = objectX * TILE_SIZE;
		objectY = objectY * TILE_SIZE + TILE_SIZE - 1;
		Object* obj = new Object({objectX, objectY}, level_count);
		objects.push_back(obj);
	}

	//godmode
	if (IsKeyPressed(KEY_F4)) {
		if (god_mode) god_mode = false;
		else god_mode = true;
	}

	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE)) {
		level_count = 1;
		LoadLevel(1);
	}
	if (IsKeyPressed(KEY_TWO)) {
		level_count = 2;
		LoadLevel(2);
	}

	if (EndLevel) {
		StopSound(sirens[siren]);
		collectPellet = false;
		pellet_timer = PELLETTIME;
		if(IsSoundPlaying(sound_pellet)) StopSound(sound_pellet);

		level->win = true;
		player->Win();
		inky->WinLose();
		blinky->WinLose();
		win = true;

		LoadLevel(0);
		EndLevel = false;
	}

	if (fruitcounter == 0) {
		Object* obj = new Object({ fruitX, fruitY }, level_count);
		objects.push_back(obj);
	}
	fruitcounter--;

	if (intro or levelintro) 
	{
		if (intro_count <= 0) {
			if (intro) player->setLives(2);
			if(intro) intro = false;
			if (levelintro) levelintro = false;
			intro_count = 240;
		}
		else 
		{
			player->Intro(intro_count);
			inky->Intro(intro_count);
			blinky->Intro(intro_count);
			--intro_count;
		}
	}
	else if (win) {
		if (!level->win) {
			level_count++;
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
		StopSound(sirens[siren]);
		player->Lose();
		if (!player->lose) {
			lose = false;
			if (player->GetLives() >= 0) {
				player->SetPos({ playerX, playerY });
				inky->SetPos({ inkyX, inkyY });
				blinky->SetPos({ blinkyX, blinkyY });
			}
			else {
				collectPellet = false;
				pellet_timer = PELLETTIME;
				if (IsSoundPlaying(sound_pellet)) StopSound(sound_pellet); StopSound(sound_pellet);
				EndGame = true;
			}
		}
	}
	else {
		//background siren
		if (!IsSoundPlaying(sirens[siren])) {
			if (siren != 0) {
				if (!IsSoundPlaying(sirens[siren - 1])) PlaySound(sirens[siren]);
			} else PlaySound(sirens[siren]);
		}

		if (collectPellet) {
			if (pellet_timer >= 0) {
				if (!IsSoundPlaying(sound_pellet)) PlaySound(sound_pellet);
				--pellet_timer;
			}
			else {
				collectPellet = false;
				pellet_timer = PELLETTIME;
				StopSound(sound_pellet);
			}
			blinky->Pellet(collectPellet, pellet_timer);
			inky->Pellet(collectPellet, pellet_timer);
		}

		level->Update();
		player->Update();
		inky->Update();
		blinky->Update();
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
		inky->DrawPlayer();
		blinky->DrawPlayer();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		inky->DrawDebug(GREEN);
		blinky->DrawDebug(GREEN);
	}

	RenderGUI();
	EndMode2D();
}
void Scene::Release()
{
    level->Release();
	player->Release();
	inky->Release();
	blinky->Release();
	livesUI->Release();
	ClearLevel();
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box, enemy_box;
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
            else if ((*it)->Sounds() == (int)ObjectType::PELLET) {
				collectPellet = true;
			}
			else if ((*it)->Sounds() == (int)ObjectType::CHERRY or (*it)->Sounds() == (int)ObjectType::STRAWBERRY) {
				PlaySound(sound_fruit);
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
	else if (count <= FRACTION5_ITEMS) siren = 4;
	else if (count <= FRACTION5_ITEMS * 2) siren = 3;
	else if (count <= FRACTION5_ITEMS * 3) siren = 2;
	else if (count <= FRACTION5_ITEMS * 4) siren = 1;

	if (!god_mode) {
		enemy_box = inky->GetHitbox();
		if (player_box.TestAABB(enemy_box)) {
			if(!collectPellet) lose = true;
		}
		else {
			enemy_box = blinky->GetHitbox();
			if (player_box.TestAABB(enemy_box)) {
				if (!collectPellet) lose = true;
			}
		}
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
	font->Draw(10, 5, TextFormat("1UP"));
	font->Draw(10, 13, TextFormat("%d", player->GetScore()));
	
	livesUI->RenderUI(player->GetLives());
	livesUI->DrawPlayer();

	if (intro or levelintro) font->Draw((WINDOW_WIDTH / 2)-22, (WINDOW_HEIGHT / 2)+15, TextFormat("READY!"), YELLOW);
	if (intro and intro_count > 60) font->Draw((WINDOW_WIDTH / 2) - 40, (WINDOW_HEIGHT / 2) - 32, TextFormat("PLAYER ONE"), CYANBLUE);

	if(god_mode) font->Draw(WINDOW_WIDTH - 125, 5, TextFormat("GOD MODE ACTIVE"));
}