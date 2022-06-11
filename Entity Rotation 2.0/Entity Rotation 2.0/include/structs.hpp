// @File: 		structs.hpp
// @Project: 	Entity Rotation 2.0
// @Author:		Justin Byrne
// @Date:		Created 6/11/22 at 7:57 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#ifndef structs_hpp
#define structs_hpp

#include <algorithm>
#include <map>
#include <cmath>

#include "helpers.hpp"

#define PI 3.141592653589
#define SIN(x) sin(x * PI / 180)
#define COS(x) cos(x * PI / 180)

std::map<int, int> hash =
{
    {   1,  1 }, {   2,  1 }, {   3,  1 }, {   4,  1 },
    {   5,  2 }, {   6,  2 }, {   7,  2 }, {   8,  2 }, {   9,  2 }, {  10,  2 },
    {  11,  3 }, {  12,  3 }, {  13,  3 }, {  14,  3 }, {  15,  3 }, {  16,  3 },
    {  17,  4 }, {  18,  4 }, {  19,  4 }, {  20,  4 }, {  21,  4 }, {  22,  4 }, {   23, 4 }, {  24,  4 },
    {  25,  5 }, {  26,  5 }, {  27,  5 }, {  28,  5 }, {  29,  5 }, {  30,  5 }, {   31, 5 }, {  32,  5 }, {  33,  5 }, {  34,  5 }, {  35,  5 }, {  36,  5 }, {  37,  5 }, {  38,  5 }, {  39,  5 }, {  40,  5 },
    {  41,  6 }, {  42,  6 }, {  43,  6 }, {  44,  6 }, {  45,  6 }, {  46,  6 }, {   47, 6 }, {  48,  6 }, {  49,  6 }, {  50,  6 }, {  51,  6 }, {  52,  6 }, {  53,  6 }, {  54,  6 },
    {  55,  7 }, {  56,  7 }, {  57,  7 }, {  58,  7 }, {  59,  7 }, {  60,  7 }, {   61, 7 }, {  62,  7 }, {  63,  7 }, {  64,  7 }, {  65,  7 }, {  66,  7 }, {  67,  7 }, {  68,  7 }, {  69,  7 }, {  70,  7 },
    {  71,  8 }, {  72,  8 }, {  73,  8 }, {  74,  8 }, {  75,  8 }, {  76,  8 }, {   77, 8 }, {  78,  8 }, {  79,  8 }, {  80,  8 }, {  81,  8 }, {  82,  8 }, {  83,  8 }, {  84,  8 },
    {  85,  9 }, {  86,  9 }, {  87,  9 }, {  88,  9 }, {  89,  9 }, {  90,  9 }, {   91, 9 }, {  92,  9 }, {  93,  9 }, {  94,  9 }, {  95,  9 }, {  96,  9 }, {  97,  9 }, {  98,  9 }, {  99,  9 }, { 100,  9 },
    { 101, 10 }, { 102, 10 }, { 103, 10 }, { 104, 10 }, { 105, 10 }, { 106, 10 }, { 107, 10 }, { 108, 10 }, { 109, 10 }, { 110, 10 }, { 111, 10 }, { 112, 10 }, { 113, 10 }, { 114, 10 },
    { 115, 11 }, { 116, 11 }, { 117, 11 }, { 118, 11 }, { 119, 11 }, { 120, 11 }, { 121, 11 }, { 122, 11 }, { 123, 11 }, { 124, 11 }, { 125, 11 }, { 126, 11 }, { 127, 11 }, { 128, 11 }, { 129, 11 }, { 130, 11 },
    { 131, 12 }, { 132, 12 }, { 133, 12 }, { 134, 12 }, { 135, 12 }, { 136, 12 }, { 137, 12 }, { 138, 12 }, { 139, 12 }, { 140, 12 }, { 141, 12 }, { 142, 12 }, { 143, 12 }, { 144, 12 },
    { 145, 13 }, { 146, 13 }, { 147, 13 }, { 148, 13 }, { 149, 13 }, { 150, 13 }, { 151, 13 }, { 152, 13 }, { 153, 13 }, { 154, 13 }, { 155, 13 }, { 156, 13 }, { 157, 13 }, { 158, 13 }, { 159, 13 }, { 160, 13 },
    { 161, 14 }, { 162, 14 }, { 163, 14 }, { 164, 14 }, { 165, 14 }, { 166, 14 }, { 167, 14 }, { 168, 14 }, { 169, 14 }, { 170, 14 }, { 171, 14 }, { 172, 14 }, { 173, 14 }, { 174, 14 },
    { 175, 15 }, { 176, 15 }, { 177, 15 }, { 178, 15 }, { 179, 15 }, { 180, 15 }
};

int get_step_divisor ( int degree_distance )
{
    return hash [ degree_distance ];
}

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
    int MIN = 0, MAX = 360;
    
    int a, b, distance, step = -1;
    
    bool clockwise;
    
    DEGREE ( int A, int B )
    {
        this->a = std::clamp ( A, MIN, MAX );
        this->b = std::clamp ( B, MIN, MAX );
        
        set_clockwise ( );
        set_distance  ( );
    }
    
    // Constructors (Generic) ....... //
    
    DEGREE  ( ) { }
    
    ~DEGREE ( ) { }
    
    // Functions ............................................................ //
    
    // > .. Setters ................. //
    
    void set_step ( )
    {
        this->step = ( this->step == -1 ) ? this->get_step_divisor ( this->distance ) : this->step;
    }
    
    void set_distance ( )
    {
        int raw_difference = ( this->a > this->b )
                                ? this->a - this->b
                                : this->b - this->a;
        
        int mod_difference = std::fmod ( raw_difference, 360 );
        
        this->distance     = ( mod_difference > 180 )
                                ? 360 - mod_difference
                                : mod_difference;
    }
    
    void set_clockwise ( )
    {
        int distance  = this->b % MAX - this->a % MAX;
        
            distance += ( distance > 180 )
                            ? ( - MAX )
                            : ( distance <= -180 )
                                ? MAX
                                : distance;
        
        this->clockwise = ( distance == MIN )
                            ? generate_random ( MIN, MAX )
                            : ( distance > MIN )
                                ? true
                                : false;
    }
    
    // > .. Getters ................. //
    
    COORDINATE rotate ( COORDINATE &origin, int &degree, int point_length = 35 )
    {
        COORDINATE coordinate = { origin.x + point_length, origin.y };

        double radians        = convert_to_radian ( degree );

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
    
    int get_step_divisor ( int degree_distance )
    {
        return hash [ degree_distance ];
    }
    
    // > .. Iterators ............... //
    
    void advance ( )    // right || counter clockwise
    {
        set_step ( );

        validate_step_v_distance ( );
        
        this->a = ( ( this->a + this->step ) > 360 )
                    ? this->step - ( 360 - this->a )
                    : this->a + this->step;
        
        decrement_distance ( );
    }
    
    void regress ( )    // left || clockwise
    {
        set_step ( );

        validate_step_v_distance ( );
        
        this->a = ( ( this->a - this->step ) < 0 )
                    ? 360 - ( this->step - this->a )
                    : this->a - this->step;
        
        decrement_distance ( );
    }
    
    void decrement_distance ( )
    {
        this->distance = this->distance - this->step;
    }
    
    // > .. Conversions ............. //
    
    double convert_to_radian ( int degree )
    {
        return ( degree * PI / 180 );
    }

    int convert_to_degree ( float radian )
    {
        return ( radian * 180 ) / PI;
    }
    
    // > .. Validators .............. //
    
    void validate_step_v_distance ( )
    {
        if ( this->distance < this->step ) this->step = this->distance;
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
