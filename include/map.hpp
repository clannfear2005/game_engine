#include "game.hpp"

class Map
{
    public:

        Map();
        ~Map();

        void LoadMap( int arr[25][32] );
        void DrawMap();
    
    private:

        SDL_Rect rectSrc, rectDest;
        SDL_Texture* dirt;
        SDL_Texture* grass;

        int map[25][32];
};
