#pragma once
#include <raylib.h>
#include "Globals.h"
#include "Entity.h"

enum class UIElements {
	LIVES_ICON1, LIVES_ICON2, LIVES_ICON3, LIVES_ICONNONE, ICON_NUM, FRUIT1, FRUIT2, FRUITEMPTY
};

class UI : public Entity {
public:
	UI(const Point &p, int t);
	~UI();

	AppStatus Initialise();
	void RenderUI(int level, bool fruit, int lives);
	void RenderUIFruit(int level, bool fruit);
	void RenderUILives(int lives);
	void Release();

private:
	int type;
};