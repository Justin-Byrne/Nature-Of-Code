//
//  helpers.cpp
//  Randomizer
//
//  Created by Justin Byrne on 5/8/22.
//

#include "../include/helpers.hpp"

#include <stdio.h>
#include <stdlib.h>

#pragma mark - RANDOM NUMBER GENERATION

/*!
    @brief                                  Generates a random number in the range passed
    @param          lower                   Lower bounds value
    @param          upper                   Upper bounds value
 */
int generate_random ( int lower, int upper )
{
    return ( rand () % (upper - lower + 1) ) + lower;
}

#pragma mark - ARRAY SHIFT

/*!
    @brief                                  Shifts an array by the supplied value(s)
    @param          array                   Array to shift
    @param          SIZE_1D                 Size of array: 1 dimensional
    @param          shift_left              Shift left: true | false
    @param          positions_to_shift      Amount of positions to shift
 */
void array_shift ( int * array, int SIZE_1D, bool shift_left, int positions_to_shift )
{
    int i = 0;
    int temp;
    
    while ( positions_to_shift )
    {
        if ( shift_left )                                   // - - - - - < SHIFT LEFT - - - - -
        {
            temp = array[0];                                // Copy beginning of array
            
            for ( i = 0; i < SIZE_1D; i++ )                 // SHIFT
            {
                array[i] = array[i + 1];
            }
            
            array[SIZE_1D - 1] = temp;                      // Replace end of array
        }
        else                                                // - - - - - > SHIFT RIGHT - - - - -
        {
            temp = array[SIZE_1D - 1];                      // Copy end of array
            
            for ( i = SIZE_1D - 1; i > 0; i-- )             // SHIFT
            {
                array[i] = array[i - 1];
            }
            
            array[0] = temp;                                // Replace beginning of array
        }
        
        positions_to_shift--;
    }
}

/*!
    @brief                                  Shifts an array by the supplied value(s)
    @param          array                   Array to shift
    @param          SIZE_1D                 Size of array: first dimension
    @param          SIZE_2D                 Size of array: second dimension
    @param          shift_left              Shift left: true | false
    @param          positions_to_shift      Amount of positions to shift
 */
void array_shift ( int array[][2], int SIZE_1D, int SIZE_2D, bool shift_left, int positions_to_shift )
{
    int i = 0, j = 0;
    int temp[SIZE_2D];
    
    while ( positions_to_shift )
    {
        if ( shift_left )                                   // - - - - - < SHIFT LEFT - - - - -
        {
            for ( j = 0; j < SIZE_2D; j++ )                 // Copy beginning of array
            {
                temp[j] = array[0][j];
            }

            for ( i = 0; i < SIZE_1D; i++ )                 // SHIFT
            {
                array[i][0] = array[i + 1][0];
                array[i][1] = array[i + 1][1];
            }
            
            for ( j = 0; j < SIZE_2D; j++)                  // Replace end of array
            {
                array[SIZE_1D - 1][j] = temp[j];
            }
        }
        else                                                // - - - - -  > SHIFT RIGHT - - - - -
        {
            for ( j = 0; j < SIZE_2D; j++ )                 // Copy end of array
            {
                temp[j] = array[SIZE_1D - 1][j];
            }

            for ( i = SIZE_1D - 1; i > 0; i-- )             // SHIFT
            {
                array[i][0] = array[i - 1][0];
                array[i][1] = array[i - 1][1];
            }

            for ( j = 0; j < SIZE_2D; j++)                  // Replace beginning of array
            {
                array[0][j] = temp[j];
            }
        }
        
        positions_to_shift--;
    }
}

#pragma mark - PERLIN NOISE

/*!
    @brief                              Perlin Noise
    @author                             John Nowl
    @see                                https://gist.github.com/nowl/828013
 */
static int SEED = 0;

static int hash[] =
{
    208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
    185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
    9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
    70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
    203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
    164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
    228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
    232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
    193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
    101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
    135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
    114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219
};

int noise2 ( int x, int y )
{
    int tmp = hash[ ( y + SEED ) % 256 ];
    
    return hash[ ( tmp + x ) % 256 ];
}

float lin_inter ( float x, float y, float s )
{
    return x + s * ( y - x );
}

float smooth_inter ( float x, float y, float s )
{
    return lin_inter ( x, y, s * s * ( 3 - 2 * s ) );
}

float noise2d ( float x, float y )
{
    int x_int    = x;
    int y_int    = y;
    
    float x_frac = x - x_int;
    float y_frac = y - y_int;
    
    int s        = noise2 ( x_int,     y_int     );
    int t        = noise2 ( x_int + 1, y_int     );
    int u        = noise2 ( x_int,     y_int + 1 );
    int v        = noise2 ( x_int + 1, y_int + 1 );
    
    float low    = smooth_inter ( s, t, x_frac );
    float high   = smooth_inter ( u, v, x_frac );
    
    return smooth_inter ( low, high, y_frac );
}

float perlin2d ( float x, float y, float freq, int depth )
{
    float xa  = x * freq;
    float ya  = y * freq;
    float amp = 1.0;
    float fin = 0;
    float div = 0.0;

    int i;
    
    for ( i = 0; i < depth; i++ )
    {
        div += 256 * amp;
        fin += noise2d ( xa, ya ) * amp;
        amp /= 2;
        xa *= 2;
        ya *= 2;
    }

    return fin / div;
}

int main ( int argc, char * argv[] )
{
    int x, y;
    
    for ( y = 0; y < 4000; y++ )
        for( x = 0; x < 4000; x++ )
            perlin2d ( x, y, 0.1, 4 );

    return 0;
}
