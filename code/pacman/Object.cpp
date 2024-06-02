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
Object::Object(const Point& p, int fruit) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE * 2, OBJECT_FRAME_SIZE * 2)
{
	Rectangle rc;
	const int n = 16;
	const int c = 8 + 1;
	switch (fruit)
	{
	case 1: 
		rc = { 0, 3 * c, n, n }; 
		type = ObjectType::CHERRY;
		break;
	case 2: 
		rc = { n, 3 * c, n, n }; 
		type = ObjectType::STRAWBERRY;
		break;

	default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc, true);
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
	else if (type == ObjectType::CHERRY)    return POINTS_CHERRY;
	else if (type == ObjectType::STRAWBERRY) return POINTS_STRAWBERRY;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}
int Object::Sounds()
{
	if (type == ObjectType::DOT)		return (int)ObjectType::DOT;
	else if (type == ObjectType::PELLET)	return (int)ObjectType::PELLET;
	else if (type == ObjectType::CHERRY)    return (int)ObjectType::CHERRY;
	else if (type == ObjectType::STRAWBERRY) return (int)ObjectType::STRAWBERRY;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}