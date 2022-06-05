//
//  structs.hpp
//  Walker 3.0
//
//  Created by Justin Byrne on 6/4/22.
//

#ifndef structs_hpp
#define structs_hpp

#include <algorithm>
#include "helpers.hpp"

#define PI 3.141592653589
#define SIN(x) sin(x * PI / 180)
#define COS(x) cos(x * PI / 180)

struct COORDINATE
{
    int x, y;
    
    COORDINATE ( int x, int y )
    {
        this->x = x;
        this->y = y;
    }
    
    // Constructors (Generic) ... //
    
    COORDINATE ( ) { }
    
    ~COORDINATE ( ) { }
};

struct DEGREE
{
    int MIN = 0;
    int MAX = 360;
    
    int a, b, distance;
    
    bool clockwise;
    
    DEGREE ( int degree )
    {
        this->a = std::clamp ( degree, MIN, MAX );
    }
    
    DEGREE ( int A, int B )
    {
        this->a = std::clamp ( A, MIN, MAX );
        this->b = std::clamp ( B, MIN, MAX );
        
        set_distance_n_direction ( );
    }
    
    // Constructors (Generic) ... //
    
    DEGREE ( ) { }
    
    ~DEGREE ( ) { }
    
    // Functions ............................................................ //
    
    void set_distance_n_direction ( )
    {
        int distance  = this->b % MAX - this->a % MAX;
        
            distance += ( distance > 180 )
                            ? ( - MAX )
                            : ( distance <= -180 )
                                ? MAX
                                : distance;
        
        this->distance  = distance;
        
        this->clockwise = ( distance == MIN )
                            ? generate_random ( MIN, MAX )
                            : ( distance > MIN )
                                ? true
                                : false;
    }
    
    double convertToRadian ( int degree )
    {
        return ( degree * PI / 180 );
    }

    int convertToDegree ( float radian )
    {
        return ( radian * 180 ) / PI;
    }
  
    COORDINATE rotate ( COORDINATE &origin, int &degree, int point_length = 35 )
    {
        COORDINATE coordinate = { origin.x + point_length, origin.y };

        double radians        = convertToRadian ( degree );

        double sine           = sin ( radians );
        double cosine         = cos ( radians );

        coordinate.x         -= origin.x;                                       // translate point back to origin
        coordinate.y         -= origin.y;

        double x_new          = coordinate.x * cosine - coordinate.y * sine;    // rotate point
        double y_new          = coordinate.x * sine   - coordinate.y * cosine;

        coordinate.x          = x_new + origin.x;                               // translate point back
        coordinate.y          = y_new + origin.y;

        return coordinate;
    }
};

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

#endif /* structs_hpp */
