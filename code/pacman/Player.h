#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 16x16
#define PLAYER_FRAME_SIZE		16

//Logical model size: 8x8 (tile size)
#define PLAYER_PHYSICAL_WIDTH	8
#define PLAYER_PHYSICAL_HEIGHT	8

//Horizontal speed
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
	void LoseLives();
	int GetLives();
	Point GetDirection();
	Point GetPosition();
	void setLives(int l);

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	void Win();
	void Lose();
	void Intro(int count);

	bool lose = false;

	bool introCaught = false;
	bool IntroUpdate(bool turn);

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
	//new var turn to check which way the player wants to turn (initialized as right since its the starting direction)
	Look turn = Look::RIGHT;

	TileMap* map;

	int score;
	int lives = 3;
	int count = 0;
	bool pellet = false;
	Sound sound_death;
};