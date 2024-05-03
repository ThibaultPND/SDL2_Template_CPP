#pragma once
#include "imageManager.h"
#include "fontManager.h"
#include "application.h"

#include <functional>
class Application;

class Button
{
public:
	Button(const std::string& buttonName, const std::shared_ptr<Image>& image, std::function<void(Application& app)> onClick, bool visible = true)
		: buttonName(buttonName), image(image), onClick(onClick), visible(visible), hovering(false) {}
	~Button() {
		SDL_DestroyTexture(hoverTexture);
	}

	const std::string& getName() const { return buttonName; }

	bool isMouseOver(int x, int y) const {
		SDL_Rect rect = image->getDestinationRect();
		return (x >= rect.x && x < rect.x + rect.w &&
			y >= rect.y && y < rect.y + rect.h);
	}
	void click(Application& app) {
		if (onClick)
		{
			onClick(app);
		}
	}
	void setHoverTexture(SDL_Texture *hoverTexture) {
		this->hoverTexture = hoverTexture;
	}

	void setPosition(int x, int y) {
		image->setPosition({ x,y });
	}
private:
	std::string buttonName;
	std::shared_ptr<Image> image;
	SDL_Texture* hoverTexture;
	bool hovering;
	std::function<void(Application& app)> onClick;
	bool visible;
};

enum class ShapeType {
	Round,
	Cubic
};

class ButtonManager
{
public:
	ButtonManager(ImageManager& imageManager, FontManager& fontManager)
		: imageManager(imageManager), fontManager(fontManager) {}

	~ButtonManager() {}

	int addButtonFromFile(std::string buttonName, const std::string& imagePath, std::function<void(Application& app)> onClick = [&](Application& app) { std::cout << "click !" << std::endl; }) {
		if (buttonExist(buttonName))
			return 1;

		int imageId = imageManager.loadImageFromFile(0, imagePath, { 255,255 });
		buttons.push_back(Button(buttonName, imageManager.getImage(imageId), onClick));
		return 0;
	}
	int addButtonFromImage(std::string buttonName, const int imageId, SDL_Point position = {0,0}, std::function<void(Application& app)> onClick = [&](Application& app) {}) {
		if (buttonExist(buttonName))
			return 1;
		
		const auto& image = imageManager.getImage(imageId);
		buttons.push_back(Button(buttonName, imageManager.getImage(imageId), onClick));
		return 0;
	}
	// TODO Ajouter la police du texte.
	int addCustomButtonWithText(std::string buttonName, std::function<void(Application& app)> onClick,std::string text, SDL_Point buttonPos = {0,0}, SDL_Color buttonColor = {0,0,0,0xFF}, Size size = {50,50}, std::string fontName = "", int fontSize = 24, SDL_Color fontColor = {0,0,0,0xFF}, SDL_Color hoverColor = { 0,0,0,0xFF }) {
		if (buttonExist(buttonName))
			return 1;

		const TTF_Font* font = fontManager.get(fontName, fontSize);
		if (!font) {

			font = fontManager.add("assets/fonts/" + fontName + ".ttf", fontSize);
			if (!font) { 
				return 1;
			}
		}
		SDL_Surface* buttonSurface = SDL_CreateRGBSurfaceWithFormat(0, size.width, size.height, 32, SDL_PIXELFORMAT_RGBA32);
		Uint32 pixelColor = SDL_MapRGBA(buttonSurface->format, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
		SDL_FillRect(buttonSurface, nullptr, pixelColor);

		if (DrawTextOnSurface(buttonSurface, text, fontSize, fontName, fontColor)){
			std::cerr << "Erreur lors de l'ajout du texte \""<< text<<"\" sur le bouton '" << buttonName << "'." << std::endl;
		}
		int imageId = imageManager.loadImageFromSurface(1, buttonSurface, buttonPos);
		addButtonFromImage(buttonName, imageId, buttonPos, onClick);
	}

	bool buttonExist(std::string buttonName) {
		for (auto& button : buttons)
		{
			if (button.getName() == buttonName) {
				return true;
			}
		}
		return false;
	}
	void handleClick(Application& app) {

		SDL_Point mouse;
		SDL_GetMouseState(&mouse.x, &mouse.y);

		for (auto& button : buttons)
		{
			if (button.isMouseOver(mouse.x, mouse.y))
			{
				button.click(app);
			}
		}
	}
	const Button* get(const std::string& buttonName) const {
		for (const auto& button : buttons) {
			if (button.getName() == buttonName) {
				return &button;
			}
		}
		return nullptr; // Aucun bouton trouvé avec le nom spécifié
	}
	bool setPosition(const std::string& buttonName, int x, int y) {
		for (auto& button : buttons) {
			if (button.getName() == buttonName) {
				button.setPosition(x, y); 
				return true;
			}
		}
		return false; // Retourne false si aucun bouton n'est trouvé avec le nom spécifié
	}
	
private:
	std::vector<Button> buttons;
	ImageManager& imageManager;
	FontManager& fontManager;

	int DrawTextOnSurface(SDL_Surface* &surface, std::string& text, int size, std::string& fontName, SDL_Color textColor = { 0,0,0,255 })
	{
		TTF_Font* textFont = fontManager.get(fontName, size);
		if (!textFont){
			return 1; // Aucunes polices associé.
		}
		SDL_Surface* textSurface = TTF_RenderText_Blended(textFont, text.c_str(), textColor);
		if (!textSurface)
		{
			return 1;
		}
		SDL_Rect textRect = {
			.x = (surface->w - textSurface->w) / 2,
			.y = (surface->h - textSurface->h) / 2,
			.w = textSurface->w,
			.h = textSurface->h
		};
		if (SDL_BlitSurface(textSurface, nullptr, surface, &textRect)){
			return 1;
		}

		SDL_FreeSurface(textSurface);
		return 0;
	}
};