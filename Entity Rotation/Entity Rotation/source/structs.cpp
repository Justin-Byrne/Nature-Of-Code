//
//  structs.cpp
//  Entity Rotation
//
//  Created by Justin Byrne on 5/31/22.
//

#include "../include/structs.hpp"

struct POINT
{
    int x, y;
    
    POINT ( int x, int y )
    {
        this->x = x;
        this->y = y;
    }
    
    ~POINT ( ) { };
};

struct ROTATION
{
    int origin, destination;
    
    ROTATION ( int origin, int destination )
    {
        this->origin      = origin;
        this->destination = destination;
    }
    
    ~ROTATION ( ) { };
};
