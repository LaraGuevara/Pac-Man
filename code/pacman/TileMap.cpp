#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;
	const int k = TILE_SIZE + 1; //for the gray border in the tileset

	dict_rect[(int)Tile::AIR] = { 12*k,  2*k, n, n };

	dict_rect[(int)Tile::DOUBLECORNER_TR] = { 0,  0, n, n };
	dict_rect[(int)Tile::DOUBLECORNER_TL] = { k,  0, n, n };
	dict_rect[(int)Tile::DOUBLEWALL_R] = { 2*k, 0, n, n };
	dict_rect[(int)Tile::DOUBLEWALL_L] = { 3 * k, 0, n, n };
	dict_rect[(int)Tile::DOUBLECORNER_BR] = { 4 * k, 0, n, n };
	dict_rect[(int)Tile::DOUBLECORNER_BL] = { 5 * k, 0, n, n };
	dict_rect[(int)Tile::CORNERWALL_BR] = { 6 * k, 0, n, n };
	dict_rect[(int)Tile::CORNERWALL_BL] = { 7 * k, 0, n, n };
	dict_rect[(int)Tile::CORNERWALL_TR] = { 8 * k, 0, n, n };
	dict_rect[(int)Tile::CORNERWALL_TL] = { 9 * k, 0, n, n };
	dict_rect[(int)Tile::DOUBLEWALL_TR] = { 10 * k, 0, n, n };
	dict_rect[(int)Tile::DOUBLEWALL_TL] = { 11 * k, 0, n, n };
	dict_rect[(int)Tile::DOUBLEWALL_BR] = { 12 * k, 0, n, n };
	dict_rect[(int)Tile::DOUBLEWALL_BL] = { 13 * k, 0, n, n };
	dict_rect[(int)Tile::WALL_BR] = { 14 * k, 0, n, n };
	dict_rect[(int)Tile::WALL_BL] = { 15 * k, 0, n, n };

	dict_rect[(int)Tile::LARGECORNER_BR] = { 0,  k, n, n };
	dict_rect[(int)Tile::LARGECORNER_BL] = { k,  k, n, n };
	dict_rect[(int)Tile::LARGECORNER_TR] = { 2 * k, k, n, n };
	dict_rect[(int)Tile::LARGECORNER_TL] = { 3 * k, k, n, n };
	dict_rect[(int)Tile::WALL_TR] = { 4 * k, k, n, n };
	dict_rect[(int)Tile::WALL_TL] = { 5 * k, k, n, n };
	dict_rect[(int)Tile::CORNER_TR] = { 6 * k, k, n, n };
	dict_rect[(int)Tile::CORNER_TL] = { 7 * k, k, n, n };
	dict_rect[(int)Tile::WALL_R] = { 8 * k, k, n, n };
	dict_rect[(int)Tile::WALL_L] = { 9 * k, k, n, n };
	dict_rect[(int)Tile::CORNER_BR] = { 10 * k, k, n, n };
	dict_rect[(int)Tile::CORNER_BL] = { 11 * k, k, n, n };
	dict_rect[(int)Tile::SQUARECORNER_TR] = { 12 * k, k, n, n };
	dict_rect[(int)Tile::SQUARECORNER_TL] = { 13 * k, k, n, n };
	dict_rect[(int)Tile::SQUARECORNER_BR] = { 14 * k, k, n, n };
	dict_rect[(int)Tile::SQUARECORNER_BL] = { 15 * k, k, n, n };

	dict_rect[(int)Tile::SQUAREWALL_L] = { 0,  2*k, n, n };
	dict_rect[(int)Tile::SQUAREWALL_R] = { k,  2 * k, n, n };
	dict_rect[(int)Tile::TOPCORNER_L] = { 2 * k, 2 * k, n, n };
	dict_rect[(int)Tile::TOPCORNER_R] = { 3 * k, 2 * k, n, n };
	dict_rect[(int)Tile::BOTTOMCORNER_L] = { 4 * k, 2 * k, n, n };
	dict_rect[(int)Tile::BOTTOMCORNER_R] = { 5 * k, 2 * k, n, n };
	dict_rect[(int)Tile::SMALLCORNER_TR] = { 6 * k, 2 * k, n, n };
	dict_rect[(int)Tile::SMALLCORNER_TL] = { 7 * k, 2 * k, n, n };
	dict_rect[(int)Tile::SMALLCORNER_BR] = { 8 * k, 2 * k, n, n };
	dict_rect[(int)Tile::SMALLCORNER_BL] = { 9 * k, 2 * k, n, n };
	dict_rect[(int)Tile::TOPCORNERWALL_L] = { 10 * k, 2 * k, n, n };
	dict_rect[(int)Tile::TOPCORNERWALL_R] = { 11 * k, 2 * k, n, n };

}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "tilemap/Arcade - Pac-Man - Maze Parts-only_blue_tiles.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w * h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::Update()
{
	int y = 0;
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
		return Tile::AIR;
	}
	return map[x + y * width];
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionWallUp(const AABB& box) const
{
	return CollisionY(box.pos, box.height);
}
bool TileMap::TestCollisionWallDown(const AABB& box) const
{
	return CollisionY(box.pos + Point(0, box.height - 1), box.height);
}
Tile TileMap::TestSideExit(const AABB& box) const
{
	return GetTileIndex(box.pos.x, box.pos.y);
}
bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile))
			return true;
	}
	return false;
}
void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;
				rc = dict_rect[(int)tile];
				DrawTextureRec(*img_tiles, rc, pos, WHITE);

			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_TILES);

	dict_rect.clear();
}