//
//  structs.hpp
//  Entity Rotation
//
//  Created by Justin Byrne on 5/31/22.
//

#include <algorithm>

#ifndef structs_hpp
#define structs_hpp

struct RGB
{
    int red, green, blue;
    
    RGB ( int red, int green, int blue )
    {
        this->red   = std::clamp ( red,   0, 255 );
        this->green = std::clamp ( green, 0, 255 );
        this->blue  = std::clamp ( blue,  0, 255 );
    }
    
    // Constructors (Generic) ... //
    
    RGB ( ) { }
    
    ~RGB ( ) { }
};

struct POINT
{
    int x, y;
    
    POINT ( int x, int y )
    {
        this->x = x;
        this->y = y;
    }
    
    // Constructors (Generic) ... //
    
    POINT ( ) { }
    
    ~POINT ( ) { }
};

struct ROTATION
{
    int origin, destination;
    
    ROTATION ( int origin, int destination )
    {
        this->origin      = origin;
        this->destination = destination;
    }
    
    // Constructors (Generic) ... //
    
    ROTATION ( ) { }
    
    ~ROTATION ( ) { }
    
    // Functions ............................................................ //
    
    /// Sets (both) the origin and destination values for the Rotation struct
    /// @param          origin                  Origin of the rotation; starting point
    /// @param          destination          Destination of the rotation; ending point
    void set ( int origin, int destination )
    {
        this->origin      = std::clamp ( origin,      0, 360 );
        this->destination = std::clamp ( destination, 0, 360 );
    }
};

#endif /* structs_hpp */
