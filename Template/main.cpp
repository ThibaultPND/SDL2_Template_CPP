// Template.cpp : définit le point d'entrée de l'Application.
//
#define SDL_MAIN_HANDLED
#include "application.h"

int main(int argc, char* argv[]) {
    try {
        Application App;
        App.init("Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, false);
        
        while (App.running()) {
            App.handleEvents();
            App.update();
            App.render();
        }

        App.clean();
    }
    catch (const std::exception& e) {
        std::cerr << "An exception occurred:\n" << e.what() << "\n" << std::endl;
        return 1; // Indique une erreur
    }

    return 0; // Indique que tout s'est bien passé
}
