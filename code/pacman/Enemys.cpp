#include "Enemys.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Enemy::Enemy(const Point& p, State_e s, Look_e view) :
	Entity(p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
{
	state = s;
	look = view;
	map = nullptr;
	
}
Enemy::~Enemy()
{
}
AppStatus Enemy::Initialise()
{
	int i;
	const int n = ENEMY_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMY, "game_sprites/Arcade - Pac-Man - General Sprites-fantasmaazul.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	

	render = new Sprite(data.GetTexture(Resource::IMG_ENEMY));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)EnemyAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_RIGHT, { 0, 0, n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_LEFT, { 0, 2 * n, n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_UP, { 0, 4 * n, n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_DOWN, { 0, 6 * n, n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 3; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_RIGHT, { (float)i * n,  0, n, n });
	};

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 3; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_LEFT, { (float)i * n, 2 * n, n, n });
	};

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_UP, ANIM_DELAY);
	for (i = 0; i < 3; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_UP, { (float)i * n, 4 * n, n, n });
	};

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_DOWN, ANIM_DELAY);
	for (i = 0; i < 3; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_DOWN, { (float)i * n, 6 * n, n, n });
	};
	/*
	sprite->SetAnimationDelay((int)EnemyAnim::DYING, ANIM_DELAY);
	for (i = 0; i < 12; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::DYING, { (float)i * n, 4 * n, n, n });
	};*/

	//chae form dead to dying so one state has animation and the other is just tje dead character

	

	return AppStatus::OK;
}


void Enemy::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}

bool Enemy::IsLookingRight() const
{
	return look == Look_e::RIGHT;
}
bool Enemy::IsLookingLeft() const
{
	return look == Look_e::LEFT;
}
bool Enemy::IsLookingUp() const
{
	return look == Look_e::UP;
}
bool Enemy::IsLookingDown() const
{
	return look == Look_e::DOWN;
}
void Enemy::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
EnemyAnim Enemy::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (EnemyAnim)sprite->GetAnimation();
}
void Enemy::Stop()
{
	dir = { 0,0 };
	state = State_e::IDLE;
	if (IsLookingRight())	SetAnimation((int)EnemyAnim::IDLE_RIGHT);
	else if (IsLookingUp())  SetAnimation((int)EnemyAnim::IDLE_UP);
	else if (IsLookingDown())  SetAnimation((int)EnemyAnim::IDLE_DOWN);
	else					SetAnimation((int)EnemyAnim::IDLE_LEFT);
}
void Enemy::StartWalkingLeft()
{
	state = State_e::WALKING;
	look = Look_e::LEFT;
	SetAnimation((int)EnemyAnim::WALKING_LEFT);
}
void Enemy::StartWalkingRight()
{
	state = State_e::WALKING;
	look = Look_e::RIGHT;
	SetAnimation((int)EnemyAnim::WALKING_RIGHT);
}
void Enemy::StartWalkingUp()
{
	state = State_e::WALKING;
	look = Look_e::UP;
	SetAnimation((int)EnemyAnim::WALKING_UP);
}
void Enemy::StartWalkingDown()
{
	state = State_e::WALKING;
	look = Look_e::DOWN;
	SetAnimation((int)EnemyAnim::WALKING_DOWN);
}
void Enemy::StartDying()
{
	state = State_e::DYING;

	SetAnimation((int)EnemyAnim::DYING);
}
void Enemy::ChangeAnimRight()
{
	look = Look_e::RIGHT;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE_RIGHT);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_RIGHT); break;
	}
}
void Enemy::ChangeAnimLeft()
{
	look = Look_e::LEFT;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE_LEFT);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_LEFT); break;
	}
}
void Enemy::ChangeAnimUp()
{
	look = Look_e::UP;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE_UP);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_UP); break;
	}
}
void Enemy::ChangeAnimDown()
{
	look = Look_e::DOWN;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE_DOWN);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_DOWN); break;
	}
}
void Enemy::Update()
{
	//all movement in move
	Move();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Enemy::Move()
{
	AABB box;
	int prev_x = pos.x;
	int prev_y = pos.y;

	//checks which way the Enemy wants to turn next
	

	//checks if the turn is possible
	if (turn != look) {
		switch (turn) {
		/*case Look::UP:
			pos.y -= ENEMY_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallUp(box)) ChangeAnimUp();
			pos.y = prev_y;
			break;
		case Look::DOWN:
			pos.y += PLAYER_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallDown(box)) ChangeAnimDown();
			pos.y = prev_y;
			break;*/
		case Look_e::LEFT:
			pos.x -= ENEMY_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallLeft(box)) ChangeAnimLeft();
			pos.x = prev_x;
			break;
		case Look_e::RIGHT:
			pos.x += ENEMY_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallRight(box)) ChangeAnimRight();
			pos.x = prev_x;
			break;
		}
	}

	if (look == Look_e::LEFT)
	{
		pos.x += -ENEMY_SPEED;
		if (state == State_e::IDLE)StartWalkingLeft();
		else {
			if (!IsLookingLeft()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == State_e::WALKING) Stop();
		}
		if (pos.x == 0) {
			pos.x = WINDOW_WIDTH;
			ChangeAnimRight();
		}

	}
	else if (look == Look_e::RIGHT)
	{
		pos.x += ENEMY_SPEED;
		if (state == State_e::IDLE) StartWalkingRight();
		else
		{
			if (!IsLookingRight()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == State_e::WALKING) Stop();
		}
		if (pos.x == WINDOW_WIDTH - 8) {
			pos.x = 0;
			ChangeAnimLeft();
		}

	}
	/*else if (look == Look::UP) {
		pos.y -= PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingUp();
		else
		{
			if (!IsLookingUp()) ChangeAnimUp();
		}

		box = GetHitbox();
		if (map->TestCollisionWallUp(box))
		{
			pos.y = prev_y;
			if (state == State::WALKING) Stop();
		}
	}
	else if (look == Look::DOWN) {
		pos.y += PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingDown();
		else
		{
			if (!IsLookingDown()) ChangeAnimDown();
		}

		box = GetHitbox();
		if (map->TestCollisionWallDown(box))
		{
			pos.y = prev_y;
			if (state == State::WALKING) Stop();
		}
	}*/
}

void Enemy::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}