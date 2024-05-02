#pragma once
#include "SDL.h"
#include "SDL_image.h"

#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include "size.h"


class Image {
public:
    Image(int id, SDL_Texture* texture, const SDL_Rect& srcRect, const SDL_Rect& destRect, bool visible)
        : id(id), texture(texture), srcRect(srcRect), destRect(destRect), visible(visible) {}
    ~Image() { SDL_DestroyTexture(texture); }

    int getId() const { return id; }
    void setTexture(SDL_Texture* texture) { SDL_DestroyTexture(this->texture); texture = texture; }
    SDL_Texture* getTexture() const { return texture; }
    const SDL_Rect& getSourceRect() const { return srcRect; }
    const SDL_Rect& getDestinationRect() const { return destRect; }
    bool isVisible()  { return visible; }
    void setPosition(SDL_Point pos) { destRect.x = pos.x; destRect.y = pos.y; }
    void show() { visible = true; }
    void hide() { visible = false; }
private:
    int id;
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    bool visible;
};

class ImageManager {
public:
    ImageManager();
    ~ImageManager();

    void init(SDL_Renderer* renderer);

    // Méthodes pour gérer les images
    int getLastId() { return nextId--; }
    int loadImageFromFile(int layer, const std::string& filename, SDL_Point pos = {0,0}, Size size = {0,0}, SDL_Rect srcRect = {0,0,0,0}, bool visible = true);
    int loadImageFromTexture(int layer, SDL_Texture* texture, SDL_Point pos = { 0,0 },SDL_Rect srcRect = {0,0,0,0}, bool visible = true);
    int loadImageFromSurface(int layer, SDL_Surface* surface, SDL_Point pos = { 0,0 });
    std::shared_ptr<Image> getImage(const int id);
    void deleteImage(const int id);
    void clean();
    void renderImages();

private:
    std::unordered_map<int, std::vector<std::shared_ptr<Image>>> imageLayers; // Utilisation de std::map pour préserver l'ordre
    std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> textures;
    SDL_Renderer* renderer;
    int nextId;

    SDL_Texture* loadTexture(const std::string& filename);
};

// Implémentation des méthodes de la classe ImageManager
