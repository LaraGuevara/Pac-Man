#include <raylib.h>
#include "UI.h"
#include "ResourceManager.h"
#include "Entity.h"
#include "Sprite.h"

UI::UI(const Point& p, int t): Entity(p, (16 + (32 * t)), 16, (16 + (32 * t)), 16)
{
	type = t;
}

UI::~UI() {
	Release();
}

AppStatus UI::Initialise() 
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ITEMS, "game_sprites/Arcade - Pac-Man - object Sprites.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (type == 1) {
		render = new Sprite(data.GetTexture(Resource::IMG_ITEMS));
		if (render == nullptr)
		{
			LOG("Failed to allocate memory for items sprite");
			return AppStatus::ERROR;
		}

		float k = 16;
		float n = 16 * 3;

		Sprite* sprite = dynamic_cast<Sprite*>(render);
		sprite->SetNumberAnimations((int)UIElements::ICON_NUM);

		sprite->SetAnimationDelay((int)UIElements::LIVES_ICON1, ANIM_DELAY);
		sprite->AddKeyFrame((int)UIElements::LIVES_ICON1, { 0, 3*k, n, k });

		sprite->SetAnimationDelay((int)UIElements::LIVES_ICON2, ANIM_DELAY);
		sprite->AddKeyFrame((int)UIElements::LIVES_ICON2, { 0, 2*k, n, k });

		sprite->SetAnimationDelay((int)UIElements::LIVES_ICON3, ANIM_DELAY);
		sprite->AddKeyFrame((int)UIElements::LIVES_ICON3, { 0, k, n, k });

		sprite->SetAnimationDelay((int)UIElements::LIVES_ICONNONE, ANIM_DELAY);
		sprite->AddKeyFrame((int)UIElements::LIVES_ICONNONE, { k, 3*k, n, k });

		sprite->SetAnimation((int)UIElements::LIVES_ICON3);

	}
	else if (type == 0) {
		render = new Sprite(data.GetTexture(Resource::IMG_ITEMS));
		if (render == nullptr)
		{
			LOG("Failed to allocate memory for items sprite");
			return AppStatus::ERROR;
		}
		float c = 16;

		Sprite* sprite = dynamic_cast<Sprite*>(render);
		sprite->SetNumberAnimations(2);

		sprite->SetAnimationDelay((int)UIElements::FRUIT1, ANIM_DELAY);
		sprite->AddKeyFrame((int)UIElements::FRUIT1, { 0, 0, c, c });

		sprite->SetAnimationDelay((int)UIElements::FRUIT2, ANIM_DELAY);
		sprite->AddKeyFrame((int)UIElements::FRUIT2, { c, 0, c, c });

		sprite->SetAnimation((int)UIElements::FRUIT1);
	}

	return AppStatus::OK;
}

void UI::RenderUI(int level, int lives)
{
	if (type == 1) RenderUILives(lives);
	else if (type == 0) RenderUIFruit(level);

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}

void UI::RenderUIFruit(int level)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	switch (level) {
	case 1:
		sprite->SetAnimation((int)UIElements::FRUIT1);
		break;
	case 2:
		sprite->SetAnimation((int)UIElements::FRUIT2);
		break;
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
	default:
		sprite->SetAnimation((int)UIElements::LIVES_ICONNONE);
	}
}

void UI::Release() 
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_ITEMS);

	render->Release();
}