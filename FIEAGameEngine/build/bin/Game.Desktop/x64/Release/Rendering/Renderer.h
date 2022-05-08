#pragma once
#include <string>

#include "Hashmap.h"

#include "SDL2/SDL.h"
#undef main
#include "SDL2/SDL_image.h"


class Renderer final
{
public:
	static Renderer* GetInstance();

	void LoadPNG(const std::string& imgPath, const std::string& name);

	void Draw(const std::string& imgPath, int xPos, int yPos, int w, int h);

	void Initialize(const std::string& windowName, int winX, int winY, int w, int h);

	void Destroy();

	void BeginFrame();

	void EndFrame();
	void renderScore(const std::string& score,int x,int y);
private:
	Renderer() = default;
	~Renderer() = default;
	Renderer(const Renderer& other) = delete;
	Renderer(Renderer&& other) = delete;
	Renderer& operator=(const Renderer& other) = delete;
	Renderer& operator=(Renderer&& other) = delete;
	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };
	FieaGameEngine::Hashmap<std::string, SDL_Texture*> images;
	static Renderer* _instance;

};

