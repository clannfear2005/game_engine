#pragma once

#include "components.hpp"

class PositionComponent : public Component
{
    public:

        // Default
        PositionComponent()
        {
            xpos = 0;
            ypos = 0;
        }

        // Start at values x, y
        PositionComponent(int x, int y)
        {
            xpos = x;
            ypos = y;
        }
    
        int x() { return xpos; }
        void x(int x) { xpos = x; }
        int y() { return ypos; }
        void y(int y) { ypos = y; }

        void update() override
        {
            xpos++;
            ypos++;
        }

        void setPos(int x, int y)
        {
            xpos = x;
            ypos = y;
        }
        
    private:

        int xpos;
        int ypos;
};
