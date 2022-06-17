// @File: 		structs.hpp
// @Project: 	Walker 5.0
// @Author:		Justin Byrne
// @Date:		Created 6/16/22 at 8:04 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#ifndef structs_hpp
#define structs_hpp

#include <algorithm>
#include <map>
#include <cmath>

#define PI 3.141592653589
#define SIN(x) sin(x * PI / 180)
#define COS(x) cos(x * PI / 180)

int generate_random ( int lower, int upper );

extern std::map<int, int> hash;

struct COORDINATE
{
    double x, y;
    
    COORDINATE ( double x, double y )
    {
        this->x = x;
        this->y = y;
    }
    
    // Constructors (Generic) ... //
    
    COORDINATE  ( ) { }
    
    ~COORDINATE ( ) { }
    
    // > .. Getters .................................... //
    
    double get_distance_from ( COORDINATE point_a, COORDINATE point_b )
    {
        return std::sqrt ( ( point_a.x - point_b.x ) * ( point_a.x - point_b.x ) +
                           ( point_a.y - point_b.y ) * ( point_a.y - point_b.y ) );
    }
};

struct ANGLE
{
    int MIN = 0, MAX = 360;
    
    int a, b, distance, step = -1;
    
    bool clockwise;
    
    ANGLE ( int A, int B )
    {
        this->a = std::clamp ( A, MIN, MAX );
        this->b = std::clamp ( B, MIN, MAX );
        
        set_clockwise ( );
        set_distance  ( );
    }
    
    // Constructors (Generic) ... //
    
    ANGLE  ( ) { }
    
    ~ANGLE ( ) { }
    
    // Functions ............................................................ //
    
    // > .. Setters .................................... //
    
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
    
    // > .. Getters .................................... //
    
    COORDINATE rotate ( COORDINATE & origin, int & degree, int step = 0 )
    {
        COORDINATE coordinate = { origin.x + step, origin.y };

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
    
    int get_angle ( COORDINATE origin, COORDINATE destination )
    {
        int result = std::fmod ( this->convert_to_degree ( std::atan2 ( destination.y - origin.y, destination.x - origin.x ) ), 360 );
        
        if ( result < 0 ) result += 360;
        
        return result;
    }
    
    // > .. Iterators .................................. //
    
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
    
    // > .. Conversions ................................ //
    
    double convert_to_radian ( int degree )
    {
        return ( degree * PI / 180 );
    }

    int convert_to_degree ( float radian )
    {
        return ( radian * 180 ) / PI;
    }
    
    // > .. Validators ................................. //
    
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
    
    RGB  ( ) { }
    
    ~RGB ( ) { }
};

#endif /* structs_hpp */
