#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 16x16
#define ENEMY_FRAME_SIZE		16

//Logical model size: 12x28
#define ENEMY_PHYSICAL_WIDTH	8
#define ENEMY_PHYSICAL_HEIGHT	8

//Horizontal speed and vertical speed while falling down
#define ENEMY_SPEED			1

//Logic states
enum class State_e { IDLE, WALKING, PELLET, EYES };
enum class Look_e { RIGHT, LEFT, UP, DOWN };
enum class Mode_e { CHASE, SCATTER };
enum class EnemyType {BLINKY, PINKY, INKY, CLYDE};

//Rendering states
enum class EnemyAnim {
	IDLE,
	WALKING_LEFT, WALKING_RIGHT, WALKING_UP, WALKING_DOWN, PELLET, PELLET_FLASH,
	 HIDDEN, EYES_LEFT, EYES_RIGHT, EYES_UP, EYES_DOWN,
	NUM_ANIMATIONS
};

class Enemy : public Entity
{
public:
	Enemy(const Point& p, State_e s, Look_e view, EnemyType t);
	~Enemy();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void Intro(int count);
	void Pellet(bool ifPellet, int count);
	void SetTarget(Point t);
	void SetTargetExit();
	void SetHome(Point t);
	void SetHomeExit(Point t);
	bool IsDead();
	bool caught = false;
	bool useDoor = false;
	Point GetEnemyPos();

	void Update(Point pacmanDir, Point pacmanPos, Point blinkypos);
	void Release();
	void DrawDebug(const Color& col) const;
	void WinLose();

	bool introCaught = false;
	void IntroUpdate(bool turn);

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;

	//Enemy mechanics
	float GetTargetDistance(Point dir);
	void Move(Point pacmanDir, Point pacmanPos, Point blinkypos);
	void UpdateTarget(Point pacmanDir, Point pacmanPos, Point blinkypos);

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

	void UpdateLook(int anim_id);
	int current_step;
	int current_frames;

	State_e state;
	Look_e look;
	Mode_e mode;
	EnemyType type;
	TileMap* map;

	Sound sound_retreat;

	Point home;
	Point home_exit;
	Point target;

	bool vulnearble = false;
	bool flash = true;
	int delay = 30;

};
