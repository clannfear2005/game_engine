#include "Game.hpp"

class TextureManager
{
    public:
        static SDL_Texture* LoadTexture(const char* fileName); // Static as it belongs to the class itself - does not need to create object
        static void Draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest);
    private:
};