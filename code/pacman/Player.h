#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 16x16
#define PLAYER_FRAME_SIZE		16

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	8
#define PLAYER_PHYSICAL_HEIGHT	8

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			2

//Logic states
enum class State { INTRO, IDLE, WALKING, DYING, CLOSED };
enum class Look { RIGHT, LEFT, UP, DOWN };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT, IDLE_UP, IDLE_DOWN,
	WALKING_LEFT, WALKING_RIGHT, WALKING_UP, WALKING_DOWN, DYING,
	CLOSED, HIDDEN,
	NUM_ANIMATIONS
};

class Player : public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	void Win();
	void Intro(int count);

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;

	//Player mechanics
	void Move();

	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();
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

	State state;
	Look look;
	//new var turn to check which way the player wants to turn (initialized as up since its the starting direction)
	Look turn = Look::RIGHT;

	TileMap* map;

	int score;
};