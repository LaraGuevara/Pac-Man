#pragma once
#include <raylib.h>
#include "Player.h"
#include "Enemys.h"
#include "TileMap.h"
#include "Object.h"
#include "UI.h"
#include "Text.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };
enum class UITypes { FRUIT, LIVES};

//total amount of created levels
#define LEVELS 2

//amount of items in a level divided by 5 (for the siren)
#define FRACTION5_ITEMS 48

//siren length
#define SIREN_LENGTH 48

//font color
#define CYANBLUE CLITERAL(Color){0, 255, 255, 255}

//timers
#define PELLETTIME 480;
#define FRUITTIME 900;

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();

    bool EndGame = false;
    bool intro = false;

private:
    AppStatus LoadLevel(int stage);

    void CheckCollisions();
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    Player* player;

    //enemies
    Enemy* blinky;
    Enemy* inky;
    //Enemy* clyde;
    //Enemy* pinky;

    //UI
    UI* fruitUI;
    UI* livesUI;

    TileMap* level;
    std::vector<Object*> objects;

    //efects
    Sound sound_intro, sound_munch1, sound_munch2, sound_pellet;
    //background sirem
    Sound sirens[5];
    //siren length for looping
    int siren_length = SIREN_LENGTH;
    //for changing between the sirens
    int siren = 0;

    bool collectPellet = false;
    int pellet_timer = 480;

    //game check
    bool EndLevel = false;
    bool win = false;
    bool lose = false;
    int level_count = 1;

    //munch 1 or 2 check
    bool munch1 = true;

    //check for godmode
    bool god_mode = false;

    //intro frame length
    int intro_count = 240;

    //player and enemies intial position save
    int playerX, playerY;
    int blinkyX, blinkyY;
    /*int pinkyX, pinkyY;*/
    int inkyX, inkyY;
    /*int clydeX, clydeY;*/

    Text* font;

    Camera2D camera;
    DebugMode debug;
};