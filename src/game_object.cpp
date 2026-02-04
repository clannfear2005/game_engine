#include "game_object.hpp"
#include "texture_manager.hpp"

GameObject::GameObject(const char* texturesheet, int x, int y)
{
    objTexture = TextureManager::LoadTexture(texturesheet);

    xpos = x;
    ypos = y;
}

GameObject::~GameObject()
{}

void GameObject::Update()
{
    xpos++;
    ypos++;

    srcRect.h = 360;
    srcRect.w = 360;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = srcRect.w / 2; // Scaled destination rectangle width by 2x
    destRect.h = srcRect.h / 2; // Same for the height
}

void GameObject::Render()
{
    SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect); // Copy the texture to the target rectangle
}
