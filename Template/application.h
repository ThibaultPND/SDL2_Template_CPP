// Template.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets.

#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "imageManager.h"
#include "buttonManager.h"
#include "fontManager.h"

#include <iostream>
#include <string>

class Application {
public:
    Application();
    ~Application();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running();

    void setButtonPosition(const std::string& buttonName, SDL_Point);
    // void addImage(int layer, const std::string& filename, SDL_Rect srcRect, SDL_Rect dstRect, bool visible);
    // Image* getImage(const int id);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;

    ButtonManager buttonManager;
    ImageManager imageManager;
    FontManager fontManager;
};


// TODO: Référencez ici les en-têtes supplémentaires nécessaires à votre programme.
// Rajouter un système de boutons, les boutons ont une fonction cliquable.
