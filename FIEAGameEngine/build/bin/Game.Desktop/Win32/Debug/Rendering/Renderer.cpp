#include "pch.h"
#include "Renderer.h"
#include "SDL2//SDL_ttf.h"
Renderer* Renderer::_instance = nullptr;

Renderer* Renderer::GetInstance()
{
    if (_instance == nullptr)
    {
        _instance = new Renderer;
    }
    return _instance;
}

void Renderer::LoadPNG(const std::string& imgPath, const std::string& name)
{
    assert(images.Find(name) == images.end());

    SDL_Texture* img = IMG_LoadTexture(renderer, imgPath.c_str());
    images.Insert(std::make_pair(name, img));
}

void Renderer::Draw(const std::string& imgPath, int xPos, int yPos, int w, int h)
{
    assert(renderer != nullptr);

    auto it = images.Find(imgPath);
    assert(it != images.end());

    SDL_Rect dest;
    dest.x = xPos;
    dest.y = yPos;
    dest.w = w;
    dest.h = h;
    SDL_RenderCopy(renderer, it->second, nullptr, &dest);
}

void Renderer::Initialize(const std::string& windowName, int winX, int winY, int w, int h)
{
    int32_t renFlags, winFlags;

    winFlags = 0;
    renFlags = SDL_RENDERER_ACCELERATED;

    window = SDL_CreateWindow(windowName.c_str(), winX, winY, w, h, winFlags);
    SDL_SetWindowFullscreen(window, 0);
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    renderer = SDL_CreateRenderer(window, -1, renFlags);
    
    IMG_Init(IMG_INIT_PNG);
}

void Renderer::BeginFrame()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Renderer::EndFrame()
{
    SDL_RenderPresent(renderer);
    //SDL_Delay(16);
}

void Renderer::renderScore(const std::string& score,int x,int y)
{
    TTF_Init();
   
    TTF_Font* sans = TTF_OpenFont("sans.ttf", 34);
    SDL_Color black = { 255,0,0 };
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(sans, score.c_str(), black);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = x;  //controls the rect's x coordinate 
    Message_rect.y = y; // controls the rect's y coordinte
    Message_rect.w = surfaceMessage->w; // controls the width of the rect
    Message_rect.h = surfaceMessage->h; // controls the height of the rect
    SDL_FreeSurface(surfaceMessage);
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
}
