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
	if (data.LoadTexture(Resource::IMG_ENEMY, "Resources/game_sprites/Arcade - Pac-Man - General Sprites-allghosts.png") != AppStatus::OK)
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
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_RIGHT, { (float)i*n, (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_LEFT, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_LEFT, { 2 * n + ((float)i*n), (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_UP, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_UP, { 4 * n + ((float)i * n), (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_DOWN, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_DOWN, { 6 * n + ((float)i * n), (float)k, n, n });
	}

	sprite->SetAnimationDelay((int)EnemyAnim::PELLET, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::PELLET, { (float)i * n, 4*n , n, n });
	}

	sprite->SetAnimationDelay((int)EnemyAnim::PELLET_FLASH, ANIM_DELAY);
	for (int i = 0; i < 2; ++i) {
		sprite->AddKeyFrame((int)EnemyAnim::PELLET_FLASH, { 2*n +((float)i * n), 4*n , n, n });
	}

	return AppStatus::OK;
}

void Enemy::IntroUpdate(bool turn) 
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	if (turn) {
		pos.x += (ENEMY_SPEED - 1);
		if (state != State_e::PELLET) {
			state = State_e::PELLET;
			SetAnimation((int)EnemyAnim::PELLET);
		}

		if (introCaught) {
			SetAnimation((int)EnemyAnim::HIDDEN);
			state = State_e::WALKING;
		}
		else {
			if (state == State_e::IDLE) StartWalkingRight();
			else {
				if (!IsLookingRight()) ChangeAnimRight();
			}
		}
	}
	else {
		pos.x += -ENEMY_SPEED;
		if (state == State_e::IDLE) StartWalkingLeft();
		else {
			if (!IsLookingLeft()) ChangeAnimLeft();
		}
	}
}

void Enemy::UpdateLook(int anim_id)
{
	EnemyAnim anim = (EnemyAnim)anim_id;
	look = (anim == EnemyAnim::WALKING_LEFT) ? Look_e::LEFT : Look_e::RIGHT;
}

void Enemy::Intro(int count) {
	if (count <= 60) SetAnimation((int)EnemyAnim::IDLE);
	else SetAnimation((int)EnemyAnim::HIDDEN);
}

void Enemy::Pellet(bool ifPellet, int count) {
	if (ifPellet) {
		state = State_e::PELLET;
		vulnearble = true;
		if (count < 300) {
			if(flash) SetAnimation((int)EnemyAnim::PELLET_FLASH);
			else SetAnimation((int)EnemyAnim::PELLET);
			--delay;
			if (delay <= 0) {
				delay = 30;
				if (flash) flash = false;
				else flash = true;
			}

		}
		else {
			SetAnimation((int)EnemyAnim::PELLET);
		}
	}
	else {
		state = State_e::WALKING;
		switch (look) {
		case Look_e::DOWN:
			ChangeAnimDown();
			break;
		case Look_e::UP:
			ChangeAnimUp();
			break;
		case Look_e::RIGHT:
			ChangeAnimRight();
			break;
		case Look_e::LEFT:
			ChangeAnimLeft();
			break;
		}
		vulnearble = false;
	}
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
	if (state != State_e::PELLET) {
		state = State_e::IDLE;
		SetAnimation((int)EnemyAnim::IDLE);
	}
}
void Enemy::StartWalkingLeft()
{
	look = Look_e::LEFT;
	if (state != State_e::PELLET) {
		state = State_e::WALKING;
		SetAnimation((int)EnemyAnim::WALKING_LEFT);
	}
}
void Enemy::StartWalkingRight()
{
	look = Look_e::RIGHT;
	if (state != State_e::PELLET) {
		state = State_e::WALKING;
		SetAnimation((int)EnemyAnim::WALKING_RIGHT);
	}
}
void Enemy::StartWalkingUp()
{
	look = Look_e::UP;
	if (state != State_e::PELLET) {
		state = State_e::WALKING;
		SetAnimation((int)EnemyAnim::WALKING_UP);
	}
}
void Enemy::StartWalkingDown()
{
	look = Look_e::DOWN;
	if (state != State_e::PELLET) {
		state = State_e::WALKING;
		SetAnimation((int)EnemyAnim::WALKING_DOWN);
	}
}
void Enemy::StartDying()
{
	state = State_e::EYES;

	//SetAnimation((int)EnemyAnim::PELLET);
}
void Enemy::ChangeAnimRight()
{
	look = Look_e::RIGHT;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_RIGHT); break;
	case State_e::PELLET: SetAnimation((int)EnemyAnim::PELLET); break;
	}
}
void Enemy::ChangeAnimLeft()
{
	look = Look_e::LEFT;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_LEFT); break;
	case State_e::PELLET: SetAnimation((int)EnemyAnim::PELLET); break;
	}
}
void Enemy::ChangeAnimUp()
{
	look = Look_e::UP;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_UP); break;
	case State_e::PELLET: SetAnimation((int)EnemyAnim::PELLET); break;
	}
}
void Enemy::ChangeAnimDown()
{
	look = Look_e::DOWN;
	switch (state)
	{
	case State_e::IDLE:	 SetAnimation((int)EnemyAnim::IDLE);    break;
	case State_e::WALKING: SetAnimation((int)EnemyAnim::WALKING_DOWN); break;
	case State_e::PELLET: SetAnimation((int)EnemyAnim::PELLET); break;
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
	data.ReleaseTexture(Resource::IMG_ENEMY);

	render->Release();
}