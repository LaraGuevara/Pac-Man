#pragma once
#include "Globals.h"
#include "Scene.h"
#include "Intro.h"

enum class GameState { MENU, MAIN_MENU, INTRO, PLAYING, SETTINGS, CREDITS };

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();

private:
    AppStatus BeginPlay();
    AppStatus BeginIntro();
    void FinishPlay();
    void FinishIntro();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene* scene;
    Intro* intro;

    const Texture2D* img_menu;
    const Texture2D* img_menu_empty;
    const Texture2D* img_menu_blinky;
    const Texture2D* img_menu_pinky;
    const Texture2D* img_menu_inky;
    /*const Texture2D* img_menu_clyde;*/
    const Texture2D* menu1;
    
    int counter = 0;

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};