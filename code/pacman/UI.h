#pragma once
#include <raylib.h>
#include "Globals.h"
#include "Entity.h"

enum class UIElements {
	LIVES_ICON1, LIVES_ICON2, LIVES_ICON3, LIVES_ICONNONE, ICON_NUM
};

class UI : public Entity {
public:
	UI(const Point &p);
	~UI();

	AppStatus Initialise();
	void RenderUI(int lives);
	void Release();

private:
	int type;
};