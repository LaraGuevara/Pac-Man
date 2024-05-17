#include <raylib.h>
#include "UI.h"
#include "ResourceManager.h"
#include "Entity.h"
#include "Sprite.h"

//UI::UI(Point p, int t): Entity(p, 16, 16)
//{
//	type = t;
//}

UI::~UI() {
	Release();
}

AppStatus UI::Initialise() 
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ITEMS, "game_sprites/Arcade - Pac-Man - General Sprites-rewards.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_ITEMS));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for items sprite");
		return AppStatus::ERROR;
	}

	if (type == 1) {
		float k = 16;
		float n = 16 * 3;

		Sprite* sprite = dynamic_cast<Sprite*>(render);
		sprite->SetNumberAnimations((int)UIElements::ICON_NUM);

		sprite->SetAnimationDelay((int)UIElements::LIVES_ICON1, ANIM_DELAY);
		sprite->AddKeyFrame((int)UIElements::LIVES_ICON1, { 0, 3*k, n, n });

		sprite->SetAnimationDelay((int)UIElements::LIVES_ICON2, ANIM_DELAY);
		sprite->AddKeyFrame((int)UIElements::LIVES_ICON2, { 0, 2*k, n, n });

		sprite->SetAnimationDelay((int)UIElements::LIVES_ICON3, ANIM_DELAY);
		sprite->AddKeyFrame((int)UIElements::LIVES_ICON3, { 0, k, n, n });

	}
	else {
		float n = 16;

		Sprite* sprite = dynamic_cast<Sprite*>(render);
		sprite->SetNumberAnimations((int)UIElements::FRUIT_NUM);

		sprite->SetAnimationDelay((int)UIElements::FRUIT1, ANIM_DELAY);
		sprite->AddKeyFrame((int)UIElements::FRUIT1, { 0, 0, n, n });

		sprite->SetAnimationDelay((int)UIElements::FRUIT2, ANIM_DELAY);
		sprite->AddKeyFrame((int)UIElements::FRUIT2, { n, 0, n, n });
	}
}

void UI::RenderUI(int level, bool fruit, int lives)
{
	if (type == 1) RenderUILives(lives);
	else RenderUIFruit(level, fruit);
}

void UI::RenderUIFruit(int level, bool fruit)
{
	if (fruit) {
		Sprite* sprite = dynamic_cast<Sprite*>(render);
		if (level == 1) {
			sprite->SetAnimation((int)UIElements::FRUIT1);
		}
		else {
			sprite->SetAnimation((int)UIElements::FRUIT2);
		}
		sprite->Update();
	}
}

void UI::RenderUILives(int lives) 
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	switch (lives) {
	case 1:
		sprite->SetAnimation((int)UIElements::LIVES_ICON1);
		break;
	case 2:
		sprite->SetAnimation((int)UIElements::LIVES_ICON2);
		break;
	case 3:
		sprite->SetAnimation((int)UIElements::LIVES_ICON3);
		break;
	}
	sprite->Update();
}

void UI::Release() 
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_ITEMS);

	render->Release();
}