//
//  colors.cpp
//  Walker 3.0
//
//  Created by Justin Byrne on 6/4/22.
//

#include "../include/colors.hpp"

#include <math.h>

double range_old = RANGE_OLD_MAX - RANGE_OLD_MIN;
double range_new = RANGE_NEW_MAX - RANGE_NEW_MIN;

int i     = 0;                      // iterates the next color
int calls = 0;                      // tracks amount of times 'get_color_value()' is called to identify when to shift 'i'

int get_color_value ( int phase )
{
    double sin_value = sin( ( FREQUENCY * i + phase ) * AMPLITUDE + CENTER );
    double new_value = ( ( ( sin_value - RANGE_OLD_MIN ) * range_new ) / range_old ) + RANGE_NEW_MIN;
    
    switch ( calls )
    {
        case 2:   calls = 0; i++;  break;
        default:  calls++;         break;
    }
    
    return int ( (int) round( new_value ) );
}

int * get_color_values ( )
{
    return new int[3] { get_color_value( PHASE_1 ), get_color_value( PHASE_2 ), get_color_value( PHASE_3 ) };
}

int * get_gray_values ( )
{
    return new int[3] { get_color_value( PHASE_1 ), get_color_value( PHASE_1 ), get_color_value( PHASE_1 ) };
}
