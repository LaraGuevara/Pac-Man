#include "Object.h"
#include "StaticImage.h"

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t;

	Rectangle rc;
	const int n = TILE_SIZE;
	const int k = TILE_SIZE + 1;
	switch (type)
	{
	case ObjectType::DOT: rc = { 13 * k, 2 * k, n, n }; break;
	case ObjectType::PELLET: rc = { 15 * k, 2 * k, n, n }; break;

	default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
}
Object::~Object()
{
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
int Object::Points() const
{
	if (type == ObjectType::DOT)		return POINTS_DOT;
	else if (type == ObjectType::PELLET)	return POINTS_PELLET;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}