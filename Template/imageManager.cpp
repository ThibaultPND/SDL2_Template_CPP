#include "ImageManager.h"

ImageManager::ImageManager() : renderer(0), nextId(0) {}

ImageManager::~ImageManager() {
    clean();
}

void ImageManager::init(SDL_Renderer* renderer) {
    this->renderer = renderer;
}

int ImageManager::loadImageFromSurface(int layer, SDL_Surface* surface, SDL_Point pos) {
    if (!surface)
        return -1;
    int id = nextId++;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return loadImageFromTexture(layer, texture, pos);
}
int ImageManager::loadImageFromTexture(int layer, SDL_Texture* texture, SDL_Point pos,SDL_Rect srcRect, bool visible) {
    if (!texture)
        return -1;
    int id = nextId++;

    Size size;
    SDL_QueryTexture(texture, NULL, NULL, &size.width, &size.height);

    SDL_Rect dstRect = {
        .x = pos.x,
        .y = pos.y,
        .w = size.width,
        .h = size.height
    };

    std::shared_ptr<Image> image = std::make_shared<Image>(id, texture, srcRect, dstRect, visible);
    imageLayers[layer].push_back(image);

    return id; // Retourner l'identifiant de l'image ajoutée
}
int ImageManager::loadImageFromFile(int layer, const std::string& filename, SDL_Point pos, Size size, SDL_Rect srcRect, bool visible) {
    // Charger la texture à partir du fichier
    SDL_Texture* texture = loadTexture(filename);
    if (!texture) {
        return -1; // Échec du chargement de la texture
    }
    return this->loadImageFromTexture(layer, texture, pos, srcRect, visible);
}

std::shared_ptr<Image> ImageManager::getImage(const int id) {
    // Parcourir tous les calques et chercher l'image avec l'identifiant spécifié
    for (auto& pair : imageLayers) {
        auto& layer = pair.second;
        for (const auto& image : layer) {
            if (image->getId() == id) {
                return image;
            }
        }
    }
    return nullptr; // Image non trouvée
}
void ImageManager::deleteImage(const int id) {
    for (auto& pair : imageLayers) {
        auto& layer = pair.second;
        layer.erase(std::remove_if(layer.begin(), layer.end(),
            [id](const std::shared_ptr<Image>& image) {
                return image->getId() == id;
            }),
            layer.end());
    }
}

void ImageManager::clean() {
    // Supprimer toutes les textures chargées
    for (const auto& pair : textures) {
        SDL_DestroyTexture(pair.second.get());
    }
    textures.clear();
}

void ImageManager::renderImages() {
    // Rendre les images dans l'ordre des calques
    int count = 0;
    for (const auto& pair : imageLayers) {
        for (const auto& image : pair.second) {
            count++;
            if (image->isVisible()) {

                // Rendre l'image uniquement si elle est visible
                SDL_Rect srcRect = image->getSourceRect();
                SDL_Rect* pSrcRect = (srcRect.w == 0 && srcRect.h == 0) ? NULL : &srcRect;
                SDL_RenderCopy(renderer, image->getTexture(), pSrcRect, &image->getDestinationRect());
            }
        }
    }
}


SDL_Texture* ImageManager::loadTexture(const std::string& filename) {
    // Vérifier si la texture a déjà été chargée
    auto it = textures.find(filename);
    if (it != textures.end()) {
        return it->second.get(); // La texture existe déjà, retourner le pointeur brut
    }

    // Charger la texture à partir du fichier
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (!surface) {
        std::cerr << "Error loading image from file '" << filename << "': " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Error creating texture from file '" << filename << "': " << SDL_GetError() << std::endl;
        return nullptr;
    }

    // Stocker la texture dans le map pour éviter les doublons
    textures[filename] = std::shared_ptr<SDL_Texture>(texture, [](SDL_Texture* ptr) {
        SDL_DestroyTexture(ptr); // Foncteur de destruction personnalisé pour SDL_Texture
    });

    return texture;
}
