#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>

Game::Game()
{
    state = GameState::MENU;
    scene = nullptr;
    intro = nullptr;
    img_menu = nullptr;
    img_menu_empty = nullptr;
    img_menu_blinky = nullptr;
    img_menu_pinky = nullptr;
    img_menu_inky = nullptr;
    img_menu_clyde = nullptr;
    menu1 = nullptr;

    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
    if (intro != nullptr)
    {
        intro->Release();
        delete intro;
        intro = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h, w2, h2;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Pac-Man");

    int monitor = GetCurrentMonitor();
    w2 = (float)GetMonitorWidth(monitor);
    h2 = (float)GetMonitorHeight(monitor);

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { w2/2 - w/2, h2/2 - h/2, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    SetWindowSize(w2, h2);
    ToggleFullscreen();

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();

    if (data.LoadTexture(Resource::IMG_MENU, "Menus/homescreen.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);

    if (data.LoadTexture(Resource::IMG_MENU_EMPTY, "Menus/homescreenempty.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_empty = data.GetTexture(Resource::IMG_MENU_EMPTY);

    if (data.LoadTexture(Resource::IMG_MENU_CLYDE, "Menus/homescreenclyde.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_clyde = data.GetTexture(Resource::IMG_MENU_CLYDE);

    if (data.LoadTexture(Resource::IMG_MENU_BLINKY, "Menus/homescreenblinky.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_blinky = data.GetTexture(Resource::IMG_MENU_BLINKY);

    if (data.LoadTexture(Resource::IMG_MENU_PINKY, "Menus/homescreenpinky.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_pinky = data.GetTexture(Resource::IMG_MENU_PINKY);

    if (data.LoadTexture(Resource::IMG_MENU_INKY, "Menus/homescreeninky.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_inky = data.GetTexture(Resource::IMG_MENU_INKY);


    //----------------------------------------------------------
    
    if (data.LoadTexture(Resource::IMG_MENU1, "Menus/First menu.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    menu1 = data.GetTexture(Resource::IMG_MENU1);

    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    scene = new Scene();
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
AppStatus Game::BeginIntro()
{
    intro = new Intro();
    if (intro == nullptr)
    {
        LOG("Failed to allocate memory for intro");
        return AppStatus::ERROR;
    }
    if (intro->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise intro");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}
void Game::FinishIntro()
{
    intro->Release();
    delete intro;
    intro = nullptr;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if (WindowShouldClose()) return AppStatus::QUIT;

    switch (state)
    {

    case GameState::MENU:
     if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
     if(IsKeyPressed(KEY_SPACE))
     {
         if(BeginIntro() != AppStatus::OK) return AppStatus::ERROR;
         state = GameState::MAIN_MENU;
     }
     break;

    case GameState::MAIN_MENU:


        if (IsKeyPressed(KEY_ESCAPE))
        {
            FinishIntro();
            counter = 0;
            state = GameState::MENU;
        }
        else if (IsKeyPressed(KEY_SPACE))
        {
            FinishIntro();
            counter = 0;
            if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
            scene->intro = true;
            state = GameState::PLAYING;
        }
        else {
            if (counter >= 840) {
                intro->Update();
                if (intro->loopCheck) counter = 0;
            }
            counter++;
        }
        break;

    case GameState::PLAYING:
        if (IsKeyPressed(KEY_ESCAPE) || scene->EndGame)
        {
            FinishPlay();
            if (BeginIntro() != AppStatus::OK) return AppStatus::ERROR;
            state = GameState::MAIN_MENU;
        }
        else
        {
            //Game logic
            scene->Update();
        }
        break;
    }
    return AppStatus::OK;
}
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);

    switch (state)
    {
    case GameState::MENU:
          
        DrawTexture(*menu1, 0, 0, WHITE);
        break;

    case GameState::MAIN_MENU:
        if (counter <= 60) {
            DrawTexture(*img_menu_empty, 0, 0, WHITE);
        }
        else if (counter <= 240) {
            DrawTexture(*img_menu_blinky, 0, 0, WHITE);
            if (counter <= 120) DrawRectangle(7 * TILE_SIZE, 7 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            else if (counter <= 180) DrawRectangle(18 * TILE_SIZE, 7 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
        }
        else if (counter <= 420) {
            DrawTexture(*img_menu_pinky, 0, 0, WHITE);
            if (counter <= 300) DrawRectangle(7 * TILE_SIZE, 9 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            else if (counter <= 360) DrawRectangle(18 * TILE_SIZE, 9 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
        }
        else if (counter <= 600) {
            DrawTexture(*img_menu_inky, 0, 0, WHITE);
            if (counter <= 480) DrawRectangle(7 * TILE_SIZE, 13 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            else if (counter <= 540) DrawRectangle(18 * TILE_SIZE, 13 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
        }
        else if (counter <= 780) {
            DrawTexture(*img_menu_clyde, 0, 0, WHITE);
            if (counter <= 660) DrawRectangle(7 * TILE_SIZE, 15 * TILE_SIZE, 20 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
            else if (counter <= 720) DrawRectangle(18 * TILE_SIZE, 15 * TILE_SIZE, 10 * TILE_SIZE, 2 * TILE_SIZE, BLACK);
        }
        else {
            DrawTexture(*img_menu, 0, 0, WHITE);
            if (counter == 840) intro->loopCheck = false;
            if (counter >= 840) {
                intro->Render();
            }
        }
        break;

    case GameState::PLAYING:
        scene->Render();
        break;
    }

    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}
void Game::Cleanup()
{
    UnloadResources();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);
    data.ReleaseTexture(Resource::IMG_MENU_BLINKY);
    data.ReleaseTexture(Resource::IMG_MENU_PINKY);
    data.ReleaseTexture(Resource::IMG_MENU_INKY);
    data.ReleaseTexture(Resource::IMG_MENU_CLYDE);
    data.ReleaseTexture(Resource::IMG_MENU_EMPTY);
    data.ReleaseTexture(Resource::IMG_MENU1);

    UnloadRenderTexture(target);
}