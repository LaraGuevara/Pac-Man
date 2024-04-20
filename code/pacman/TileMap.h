#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	EMPTY = -1,
	// -: check if player has left the screen
	EXIT_LEFT = -3, EXIT_RIGHT = -2,
	//  0: air tile
	AIR = 0,

	// 0 < id < 50: static tiles (in tileset order)
	DOUBLECORNER_TR = 1, DOUBLECORNER_TL, DOUBLEWALL_R, DOUBLEWALL_L, DOUBLECORNER_BR, DOUBLECORNER_BL,
	CORNERWALL_BR, CORNERWALL_BL, CORNERWALL_TR, CORNERWALL_TL, 
	DOUBLEWALL_TR, DOUBLEWALL_TL, DOUBLEWALL_BR, DOUBLEWALL_BL,
	WALL_BR, WALL_BL, 

	LARGECORNER_BR = 17, LARGECORNER_BL, LARGECORNER_TR, LARGECORNER_TL,
	WALL_TR, WALL_TL, 
	CORNER_TR, CORNER_TL, 
	WALL_R, WALL_L,
	CORNER_BR, CORNER_BL,
	SQUARECORNER_TR, SQUARECORNER_TL, SQUARECORNER_BR, SQUARECORNER_BL,

	SQUAREWALL_L = 33, SQUAREWALL_R,
	TOPCORNER_L, TOPCORNER_R, BOTTOMCORNER_L, BOTTOMCORNER_R,
	SMALLCORNER_TR, SMALLCORNER_TL, SMALLCORNER_BR, SMALLCORNER_BL,
	TOPCORNERWALL_L, TOPCORNERWALL_R,

	// 50 <= id < 100: special tiles
	DOT = 50, LARGE_DOT, PELLET,

	//Intervals
	STATIC_FIRST = DOUBLECORNER_TR,
	STATIC_LAST = TOPCORNERWALL_R,
	SOLID_FIRST = DOUBLECORNER_TR,
	SOLID_LAST = TOPCORNERWALL_R,
	SPECIAL_FIRST = DOT,
	SPECIAL_LAST = PELLET,

	PLAYER = 100,

	//enemies
	BLINKY = 101, PINKY, INKY, CLYDE
	

	/*ENTITY_FIRST = PLAYER,
	ENTITY_LAST = PLAYER*/
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void Release();

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	bool TestCollisionWallUp(const AABB& box) const;
	bool TestCollisionWallDown(const AABB& box) const;

	Tile TestSideExit(const AABB& box) const;

	bool win = false;
	bool lose = false;

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;

	//Tile map
	Tile* map;

	//Size of the tile map
	int size, width, height;

	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	//Tile sheet
	const Texture2D* img_tiles;
	const Texture2D* img_tiles_white;

	//count for win screen
	int delay = 30;
	int white = 0;
	int flash = 4;
};