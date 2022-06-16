// @File: 		helpers.cpp
// @Project: 	Walker 5.0
// @Author:		Justin Byrne
// @Date:		Created 6/16/22 at 8:05 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#include "../include/helpers.hpp"

#include <SDL2/SDL.h>
#include <stdlib.h>

#pragma mark - REFERENCES

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

#pragma mark - GENERICS

/// Generates a random number in the range passed
/// @param      lower               Lower bounds value
/// @param      upper               Upper bounds value
int generate_random ( int lower, int upper )
{
    return ( rand () % (upper - lower + 1) ) + lower;
}

#pragma mark - ARRAY

/// Shifts an array by the supplied value(s)
/// @param      array               Array to shift
/// @param      SIZE_1D             Size of array: 1 dimensional
/// @param      shift_left          Shift left: true | false
/// @param      positions_to_shift  Amount of positions to shift
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

/// Shifts an array by the supplied value(s)
/// @param      array               Array to shift
/// @param      SIZE_1D             Size of array: first dimension
/// @param      SIZE_2D             Size of array: second dimension
/// @param      shift_left          Shift left: true | false
/// @param      positions_to_shift  Amount of positions to shift
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

#pragma mark - SDL

/// Sets  foreground of the renderer
/// @param      color               RGB values for foreground
void set_render_draw_color ( SDL_Renderer * renderer, RGB color )
{
    SDL_SetRenderDrawColor (        // background color
        renderer,                   // rendering context
        color.red,                  // red value
        color.green,                // green value
        color.blue,                 // blue value
        SDL_ALPHA_OPAQUE            // alpha value
    );
}

/// Sets background and foreground of the renderer
/// @param      background          RGB values for background
/// @param      foreground          RGB values for foreground
void set_render_draw_colors ( SDL_Renderer * renderer, RGB background, RGB foreground )
{
    SDL_SetRenderDrawColor (        // background color
        renderer,                   // rendering context
        background.red,             // red value
        background.green,           // green value
        background.blue,            // blue value
        SDL_ALPHA_OPAQUE            // alpha value
    );

    SDL_RenderClear ( renderer );
    
    SDL_SetRenderDrawColor (        // background color
        renderer,                   // rendering context
        foreground.red,             // red value
        foreground.green,           // green value
        foreground.blue,            // blue value
        SDL_ALPHA_OPAQUE            // alpha value
    );
}


int SDL_RenderDrawCircle ( SDL_Renderer * renderer, COORDINATE coord, int radius )
{
    int offsetx = 0;
    int offsety = radius;
    int d       = radius -1;
    int status  = 0;

    while ( offsety >= offsetx )
    {
        status += SDL_RenderDrawPoint ( renderer, coord.x + offsetx, coord.y + offsety );
        status += SDL_RenderDrawPoint ( renderer, coord.x + offsety, coord.y + offsetx );
        status += SDL_RenderDrawPoint ( renderer, coord.x - offsetx, coord.y + offsety );
        status += SDL_RenderDrawPoint ( renderer, coord.x - offsety, coord.y + offsetx );
        status += SDL_RenderDrawPoint ( renderer, coord.x + offsetx, coord.y - offsety );
        status += SDL_RenderDrawPoint ( renderer, coord.x + offsety, coord.y - offsetx );
        status += SDL_RenderDrawPoint ( renderer, coord.x - offsetx, coord.y - offsety );
        status += SDL_RenderDrawPoint ( renderer, coord.x - offsety, coord.y - offsetx );

        if ( status < 0 )
        {
            status = -1;
            break;
        }

        if ( d >= 2*offsetx )
        {
            d       -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if ( d < 2 * (radius - offsety ) )
        {
            d       += 2 * offsety - 1;
            offsety -= 1;
        }
        else
        {
            d       += 2 * ( offsety - offsetx - 1 );
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

int SDL_RenderDrawCircle ( SDL_Renderer * renderer, COORDINATE coord, int radius, RGB color )
{
    set_render_draw_color ( renderer, color );
    
    int offsetx = 0;
    int offsety = radius;
    int d       = radius -1;
    int status  = 0;

    while ( offsety >= offsetx )
    {
        status += SDL_RenderDrawPoint ( renderer, coord.x + offsetx, coord.y + offsety );
        status += SDL_RenderDrawPoint ( renderer, coord.x + offsety, coord.y + offsetx );
        status += SDL_RenderDrawPoint ( renderer, coord.x - offsetx, coord.y + offsety );
        status += SDL_RenderDrawPoint ( renderer, coord.x - offsety, coord.y + offsetx );
        status += SDL_RenderDrawPoint ( renderer, coord.x + offsetx, coord.y - offsety );
        status += SDL_RenderDrawPoint ( renderer, coord.x + offsety, coord.y - offsetx );
        status += SDL_RenderDrawPoint ( renderer, coord.x - offsetx, coord.y - offsety );
        status += SDL_RenderDrawPoint ( renderer, coord.x - offsety, coord.y - offsetx );

        if ( status < 0 )
        {
            status = -1;
            break;
        }

        if ( d >= 2*offsetx )
        {
            d       -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if ( d < 2 * (radius - offsety ) )
        {
            d       += 2 * offsety - 1;
            offsety -= 1;
        }
        else
        {
            d       += 2 * ( offsety - offsetx - 1 );
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


int SDL_RenderFillCircle ( SDL_Renderer * renderer, COORDINATE coord, int radius )
{
    int offsetx = 0;
    int offsety = radius;
    int d       = radius -1;
    int status  = 0;

    while (offsety >= offsetx)
    {
        status += SDL_RenderDrawLine ( renderer, coord.x - offsety, coord.y + offsetx, coord.x + offsety, coord.y + offsetx );
        status += SDL_RenderDrawLine ( renderer, coord.x - offsetx, coord.y + offsety, coord.x + offsetx, coord.y + offsety );
        status += SDL_RenderDrawLine ( renderer, coord.x - offsetx, coord.y - offsety, coord.x + offsetx, coord.y - offsety );
        status += SDL_RenderDrawLine ( renderer, coord.x - offsety, coord.y - offsetx, coord.x + offsety, coord.y - offsetx );

        if ( status < 0 )
        {
            status = -1;
            break;
        }

        if ( d >= 2*offsetx )
        {
            d       -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if ( d < 2 * ( radius - offsety ) )
        {
            d       += 2 * offsety - 1;
            offsety -= 1;
        }
        else
        {
            d       += 2 * ( offsety - offsetx - 1 );
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

int SDL_RenderFillCircle ( SDL_Renderer * renderer, COORDINATE coord, int radius, RGB color )
{
    set_render_draw_color ( renderer, color );
    
    int offsetx = 0;
    int offsety = radius;
    int d       = radius -1;
    int status  = 0;

    while (offsety >= offsetx)
    {
        status += SDL_RenderDrawLine ( renderer, coord.x - offsety, coord.y + offsetx, coord.x + offsety, coord.y + offsetx );
        status += SDL_RenderDrawLine ( renderer, coord.x - offsetx, coord.y + offsety, coord.x + offsetx, coord.y + offsety );
        status += SDL_RenderDrawLine ( renderer, coord.x - offsetx, coord.y - offsety, coord.x + offsetx, coord.y - offsety );
        status += SDL_RenderDrawLine ( renderer, coord.x - offsety, coord.y - offsetx, coord.x + offsety, coord.y - offsetx );

        if ( status < 0 )
        {
            status = -1;
            break;
        }

        if ( d >= 2*offsetx )
        {
            d       -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if ( d < 2 * ( radius - offsety ) )
        {
            d       += 2 * offsety - 1;
            offsety -= 1;
        }
        else
        {
            d       += 2 * ( offsety - offsetx - 1 );
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


/// Draw a dotted line
/// @param      renderer            SDL_Renderer
/// @param      origin              Origin coordinate to draw from
/// @param      destination         Destination coordinate to draw to
/// @param      segments            Number of segments in dotted line
int SDL_RenderDottedLine ( SDL_Renderer * renderer, COORDINATE origin, COORDINATE destination, int segments )
{
    int line_space = COORDINATE().get_distance_from ( origin, destination ) / segments;
    int angle      = DEGREE().get_angle ( origin, destination );

    for ( int i = 0; i < segments; i++ )
    {
        origin = DEGREE().rotate ( origin, angle, line_space );
        SDL_RenderDrawPoint ( renderer, origin.x, origin.y );
    }
    
    return 0;
}

int SDL_RenderDottedLine ( SDL_Renderer * renderer, COORDINATE origin, COORDINATE destination, int segments, RGB color )
{
    set_render_draw_color ( renderer, color );
    
    int line_space = COORDINATE().get_distance_from ( origin, destination ) / segments;
    int angle      = DEGREE().get_angle ( origin, destination );

    for ( int i = 0; i < segments; i++ )
    {
        origin = DEGREE().rotate ( origin, angle, line_space );
        SDL_RenderDrawPoint ( renderer, origin.x, origin.y );
    }
    
    return 0;
}


int SDL_RenderDrawLine ( SDL_Renderer * renderer, COORDINATE origin, COORDINATE destination, RGB color )
{
    set_render_draw_color ( renderer, color );
    
    return SDL_RenderDrawLine ( renderer, origin.x, origin.y, destination.x, destination.y );
}


int SDL_RenderDrawPoint ( SDL_Renderer * renderer, COORDINATE origin, RGB color )
{
    set_render_draw_color ( renderer, color );
    
    return SDL_RenderDrawPoint   ( renderer, origin.x, origin.y );
}
