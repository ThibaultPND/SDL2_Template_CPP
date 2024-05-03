#include "application.h"

Application::Application() : imageManager(),fontManager(), buttonManager(imageManager, fontManager){
    window = nullptr;
    renderer = nullptr;
    isRunning = false;
}

Application::~Application() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Application::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Erreur lors de l'initialisation de SDL : " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }

    if (TTF_Init() != 0) {
        std::cerr << "Erreur lors de l'initialisation de SDL_ttf : " << TTF_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (!window) {
        std::cerr << "Erreur lors de la création de la fenêtre : " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        isRunning = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Erreur lors de la création du rendu : " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        isRunning = false;
        return;
    }
    imageManager.init(renderer);


    fontManager.add("assets/fonts/maturasc.ttf");
    const auto& arialFont = fontManager.get("maturasc", 24);
    buttonManager.addCustomButtonWithText(
        "start", // Button name
        [](Application& app) {app.setButtonPosition("start", { std::rand() % 600,std::rand() % 700 }); }, // Callback
        "DEPART", // Texte
        { 59,53 }, // initialPosition
        { 128,255,128,255 }, // Background Colour
        { 200,100 }, // Size
        "maturasc", // FontName
        24, // Text size
        { 255,255,255,255 } // Text color
    );
    std::cout << std::rand() << std::rand() << std::endl;
    
    isRunning = true;
}

void Application::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            buttonManager.handleClick(*this);
            break;
        default:
            break;
    }
}

void Application::update() {
}

void Application::render() {
    // Effacer l'écran
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    imageManager.renderImages();

    // Actualiser l'écran
    SDL_RenderPresent(renderer);
}

void Application::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_Quit();
}

bool Application::running() {
    return isRunning;
}

void Application::setButtonPosition(const std::string& buttonName, SDL_Point pos) {
    buttonManager.setPosition("start", pos.x, pos.y);
}