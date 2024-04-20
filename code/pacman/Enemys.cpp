#include "Enemys.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Enemy::Enemy(const Point& p, State_e s, Look_e view, EnemyType t) :
	Entity(p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
{
	state = s;
	look = view;
	map = nullptr;
	type = t;
	
}
Enemy::~Enemy()
{
}
AppStatus Enemy::Initialise()
{
	const int n = ENEMY_FRAME_SIZE;
	int k;
	switch (type) {
	case EnemyType::BLINKY:
		k = n * 2;
		break;
	case EnemyType::PINKY:
		k = n;
		break;
	case EnemyType::INKY:
		k = 0;
		break;
	case EnemyType::CLYDE:
		k = n * 3;
		break;
	}

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMY, "game_sprites/Arcade - Pac-Man - General Sprites-allghosts.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	

	render = new Sprite(data.GetTexture(Resource::IMG_ENEMY));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for enemy sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)EnemyAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)EnemyAnim::IDLE, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::IDLE, { n*4, (float)k, n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::HIDDEN, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::HIDDEN, { n * 4, 5*n, n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::WALKING_RIGHT, { 0, (float)k, n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::WALKING_LEFT, { 2 * n, (float)k, n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::WALKING_UP, { 4 * n, (float)k, n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::WALKING_DOWN, { 6 * n, (float)k, n, n });

	

	return AppStatus::OK;
}

void Enemy::WinLose() {
	SetAnimation((int)EnemyAnim::HIDDEN);
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
	SetAnimation((int)EnemyAnim::IDLE);
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
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_RIGHT); break;
	}
}
void Enemy::ChangeAnimLeft()
{
	look = Look_e::LEFT;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_LEFT); break;
	}
}
void Enemy::ChangeAnimUp()
{
	look = Look_e::UP;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_UP); break;
	}
}
void Enemy::ChangeAnimDown()
{
	look = Look_e::DOWN;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE);    break;
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
			look = Look_e::RIGHT;
			StartWalkingRight();
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
			look = Look_e::LEFT;
			StartWalkingLeft();
		}

	}
}

void Enemy::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}


void Enemy::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}