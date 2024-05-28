#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Player::Player(const Point& p, State s, Look view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	map = nullptr;
	score = 0;
}
Player::~Player()
{
}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "game_sprites/Arcade - Pac-Man - all pacman.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	sound_death = data.GetSound(AudioResource::AUD_DEATH);

	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 0, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { 0, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_UP, { 0, 2*n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_DOWN, { 0, 3 * n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 3; ++i) {
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i * n,  0, n, n });
	};

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 3; ++i) {
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i * n, n, n, n });
	};

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_UP, ANIM_DELAY);
	for (i = 0; i < 3; ++i){
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_UP, { (float)i * n, 2*n, n, n });
	};

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_DOWN, ANIM_DELAY);
	for (i = 0; i < 3; ++i){
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_DOWN, { (float)i * n, 3 * n, n, n });
    };

	sprite->SetAnimationDelay((int)PlayerAnim::DYING, ANIM_DELAY);
	for (i = 0; i < 12; ++i){
		sprite->AddKeyFrame((int)PlayerAnim::DYING, { (float)i * n, 4*n, n, n });
    };

	//chae form dead to dying so one state has animation and the other is just tje dead character

	sprite->SetAnimationDelay((int)PlayerAnim::CLOSED, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CLOSED, { 0, 4 * n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::HIDDEN, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::HIDDEN, { 4*n, 0, n, n });

	sprite->SetAnimation((int)PlayerAnim::HIDDEN);

	return AppStatus::OK;
}
bool Player::IntroUpdate(bool turn) 
{
	bool end = false;
	if (turn) {
		pos.x += PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingRight();
		else {
			if (!IsLookingRight()) ChangeAnimRight();
		}
	}
	else {
		pos.x += -PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingLeft();
		else {
			if (!IsLookingLeft()) ChangeAnimLeft();
		}
	}

	if (pos.x == WINDOW_WIDTH) end = true;
	return end;
}
void Player::InitScore()
{
	score = 0;
}
void Player::IncrScore(int n)
{
	score += n;
}
int Player::GetScore()
{
	return score;
}
int Player::GetLives() 
{
	return lives;
}
void Player::setLives(int l) 
{
	lives = l;
}
void Player::LoseLives() 
{
	--lives;
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void Player::Win() 
{
	SetAnimation((int)PlayerAnim::CLOSED);
}
void Player::Lose() {
	lose = true;
	if (count == 0) {
		PlaySound(sound_death);
		SetAnimation((int)PlayerAnim::DYING);
		LoseLives();
	}

	count++;

	if (count < 150) {
		if (count < 48) {
			Sprite* sprite = dynamic_cast<Sprite*>(render);
			sprite->Update();
		} else SetAnimation((int)PlayerAnim::HIDDEN);
	}
	else {
		lose = false;
		count = 0;
		SetAnimation((int)PlayerAnim::CLOSED);
	}
}
void Player::Intro(int count) {
	if(count <= 60) SetAnimation((int)PlayerAnim::CLOSED);
	else SetAnimation((int)PlayerAnim::HIDDEN);
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsLookingUp() const
{
	return look == Look::UP;
}
bool Player::IsLookingDown() const
{
	return look == Look::DOWN;
}
void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
PlayerAnim Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}
void Player::Stop()
{
	dir = { 0,0 };
	state = State::IDLE;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	else if (IsLookingUp())  SetAnimation((int)PlayerAnim::IDLE_UP);
	else if (IsLookingDown())  SetAnimation((int)PlayerAnim::IDLE_DOWN);
	else					SetAnimation((int)PlayerAnim::IDLE_LEFT);
}
void Player::StartWalkingLeft()
{
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::WALKING_LEFT);
}
void Player::StartWalkingRight()
{
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}
void Player::StartWalkingUp()
{
	state = State::WALKING;
	look = Look::UP;
	SetAnimation((int)PlayerAnim::WALKING_UP);
}
void Player::StartWalkingDown()
{
	state = State::WALKING;
	look = Look::DOWN;
	SetAnimation((int)PlayerAnim::WALKING_DOWN);
}
void Player::StartDying()
{
	state = State::DYING;
	
	SetAnimation((int)PlayerAnim::DYING);
}
void Player::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_RIGHT);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_RIGHT); break;
	}
}
void Player::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_LEFT);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_LEFT); break;
	}
}
void Player::ChangeAnimUp()
{
	look = Look::UP;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_UP);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_UP); break;
	}
}
void Player::ChangeAnimDown()
{
	look = Look::DOWN;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_DOWN);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_DOWN); break;
	}
}
void Player::Update()
{
	//all movement in move
	Move();   

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Player::Move()
{
	AABB box;
	int prev_x = pos.x;
	int prev_y = pos.y;

	//checks which way the player wants to turn next
	if (IsKeyPressed(KEY_UP) or IsKeyDown(KEY_UP))
	{
		turn = Look::UP;
		state = State::WALKING;
	}
	else if (IsKeyPressed(KEY_DOWN) or IsKeyDown(KEY_DOWN)) {
		turn = Look::DOWN;
		state = State::WALKING;
	}
	else if (IsKeyPressed(KEY_RIGHT) or IsKeyDown(KEY_RIGHT)) {
		turn = Look::RIGHT;
		state = State::WALKING;
	}
	else if (IsKeyPressed(KEY_LEFT) or IsKeyDown(KEY_LEFT)) {
		turn = Look::LEFT;
		state = State::WALKING;
	}

	//checks if the turn is possible
	if (turn != look) {
		switch (turn) {
		case Look::UP:
			pos.y -= PLAYER_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallUp(box)) ChangeAnimUp();
			pos.y = prev_y;
			break;
		case Look::DOWN:
			pos.y += PLAYER_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallDown(box)) ChangeAnimDown();
			pos.y = prev_y;
			break;
		case Look::LEFT:
			pos.x -= PLAYER_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallLeft(box)) ChangeAnimLeft();
			pos.x = prev_x;
			break;
		case Look::RIGHT:
			pos.x += PLAYER_SPEED;
			box = GetHitbox();
			if (!map->TestCollisionWallRight(box)) ChangeAnimRight();
			pos.x = prev_x;
			break;
		}
	}

	if (look == Look::LEFT)
	{
		pos.x += -PLAYER_SPEED;
		if (state == State::IDLE)StartWalkingLeft();
		else {
			if (!IsLookingLeft()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
		if (pos.x == 0) {
			pos.x = WINDOW_WIDTH;
			ChangeAnimRight();
		}
		
	}
	else if (look == Look::RIGHT)
	{
		pos.x += PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingRight();
		else
		{
			if (!IsLookingRight()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
		if (pos.x == WINDOW_WIDTH - 8) {
			pos.x = 0;
			ChangeAnimLeft();
		}
		
	}
	else if (look == Look::UP) {
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
	}
}
void Player::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
	DrawPixel(pos.x, pos.y-4, WHITE);
}
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}