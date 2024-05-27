#pragma once

#include "raylib.h"
#include "Globals.h"
#include <unordered_map>
#include <string>

enum class Resource {
    IMG_MENU1,
    IMG_MENU,
    IMG_PLAYER,
    IMG_TILES,
    IMG_TILES_WHITE,
    IMG_ENEMY,
    IMG_ITEMS,
    IMG_FONT
};

enum class AudioResource {
    AUD_INTRO,
    AUD_MUNCH1,
    AUD_MUNCH2,
    AUD_DEATH,
    AUD_SIREN1,
    AUD_SIREN2,
    AUD_SIREN3,
    AUD_SIREN4,
    AUD_SIREN5,
    AUD_NUM
};

class ResourceManager {
public:
    //Singleton instance retrieval
    static ResourceManager& Instance()
    {
        static ResourceManager instance; //Guaranteed to be initialized only once
        return instance;
    }

    //Load and unload texture
    AppStatus LoadTexture(Resource id, const std::string& file_path);
    void LoadSounds();

    void ReleaseTexture(Resource id);

    //Get texture by key
    const Texture2D* GetTexture(Resource id) const;
    Sound GetSound(AudioResource id) const;

    //Release resources
    void Release();

    //Ensure Singleton cannot be copied or assigned
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    //Private constructor to prevent instantiation
    ResourceManager();
    //Destructor
    ~ResourceManager();

    //Dictionary to store loaded textures
    std::unordered_map<Resource, Texture2D> textures;
    Sound sounds[(int)AudioResource::AUD_NUM];
};