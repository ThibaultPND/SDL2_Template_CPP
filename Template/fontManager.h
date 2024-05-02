#pragma once
#include <iostream>
#include <string>
#include <map>

#include "fontManager.h"

class FontManager
{
public:
	FontManager(){
	}

	~FontManager(){
		// ! Error here
		/*for (auto& pair : mFont)
		{
			TTF_CloseFont(pair.second);
		}*/
	}

	TTF_Font* add(const std::string& filepath, int size = 24) {
		std::string filename = extractFilename(filepath);
		
		std::string key = filename + "_" + std::to_string(size);

		if (mFont.find(key) != mFont.end()){
			return nullptr;
		}

		TTF_Font* font = TTF_OpenFont(filepath.c_str(), size);
		if (!font) {
			std::cerr << "Erreur lors du chargement de la police d'écriture '" << key << "'. Vérifier votre installation." << std::endl;
			return nullptr;
		}
		mFont[key] = font;
		return font;
	}
	int remove(std::string& filename, int size) {
		
		std::string key = extractFilename(filename) + '_' + std::to_string(size);

		auto it = mFont.find(key);
		if (it != mFont.end()) {
			TTF_CloseFont(it->second);
			mFont.erase(it);
		}
		else {
			std::cerr << "Vous avez essayé de supprimer la police avec la clé " << key << ". La clé n'existe pas." << std::endl;
			return -1;
		}
		return 0;
	}
	TTF_Font* get(const std::string& fontName, int size) {
		// Construction de la clé en incluant le nom de la police et la taille de la police
		std::string key = fontName + ".ttf_" + std::to_string(size);

		// Recherche de la clé dans la carte
		auto it = mFont.find(key);
		if (it != mFont.end()) {
			return it->second;
		}
		else {
			return nullptr;
		}
	}

private:
	std::map<std::string, TTF_Font*> mFont;

	std::string extractFilename(const std::string& path) const {
		// Trouver la dernière occurrence du caractère '/' dans le chemin
		auto lastSlashPos = path.find_last_of('/');

		// Si le caractère '/' n'est pas trouvé, utiliser le chemin complet
		if (lastSlashPos == std::string::npos) {
			return path;
		}

		// Extraire le nom de fichier en supprimant le chemin complet
		return path.substr(lastSlashPos + 1);
	}
};
