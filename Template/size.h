#pragma once

struct Size {
    int width;
    int height;

    // Constructeur par défaut
    Size() : width(0), height(0) {}

    // Constructeur avec des valeurs initiales
    Size(int w, int h) : width(w), height(h) {}
};