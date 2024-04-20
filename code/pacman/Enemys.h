#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 16x16
#define ENEMY_FRAME_SIZE		16

//Logical model size: 12x28
#define ENEMY_PHYSICAL_WIDTH	8
#define ENEMY_PHYSICAL_HEIGHT	8

//Horizontal speed and vertical speed while falling down
#define ENEMY_SPEED			2

//Logic states
enum class State_e { IDLE, WALKING, DYING };
enum class Look_e { RIGHT, LEFT, UP, DOWN };
enum class EnemyType {BLINKY, PINKY, INKY, CLYDE};

//Rendering states
enum class EnemyAnim {
	IDLE,
	WALKING_LEFT, WALKING_RIGHT, WALKING_UP, WALKING_DOWN, DYING,
	 HIDDEN,
	NUM_ANIMATIONS
};

class Enemy : public Entity
{
public:
	Enemy(const Point& p, State_e s, Look_e view, EnemyType t);
	~Enemy();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void Update();
	void Release();
	void DrawDebug(const Color& col) const;
	void WinLose();

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;

	//Player mechanics
	void Move();

	//Animation management
	void SetAnimation(int id);
	EnemyAnim GetAnimation();
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingUp();
	void StartWalkingDown();
	void StartDying();
	void ChangeAnimRight();
	void ChangeAnimLeft();
	void ChangeAnimUp();
	void ChangeAnimDown();

	State_e state;
	Look_e look;
	EnemyType type;
	TileMap* map;

	
};
