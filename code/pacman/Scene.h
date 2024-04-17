#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };
#define LEVELS 1

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
    TileMap* level;
    std::vector<Object*> objects;
    Sound sound_intro, sound_munch1, sound_munch2;

    //game check
    bool EndLevel = false;
    bool win = false;
    int level_count = 1;

    //munch 1 or 2 check
    bool munch1 = true;

    //intro frame length
    int intro_count = 240;

    Camera2D camera;
    DebugMode debug;
};