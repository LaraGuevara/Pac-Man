#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	8
#define OBJECT_FRAME_SIZE		8

#define POINTS_DOT	10
#define POINTS_PELLET	20
#define POINTS_CHERRY	100
#define POINTS_STRAWBERRY	300

enum class ObjectType { DOT, PELLET, CHERRY, STRAWBERRY };

class Object : public Entity
{
public:
	Object(const Point& p, ObjectType t);
	Object(const Point& p, int fruit);
	~Object();

	void DrawDebug(const Color& col) const;
	int Points() const;
	int Sounds();

private:
	ObjectType type;
};