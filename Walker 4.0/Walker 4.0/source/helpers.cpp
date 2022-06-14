// @File: 		helpers.cpp
// @Project: 	Walker 4.0
// @Author:		Justin Byrne
// @Date:		Created 6/10/22 at 8:36 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#include "../include/helpers.hpp"

#include <SDL2/SDL.h>
#include <stdlib.h>

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

#pragma mark - GEOMETRY

/// Checks whether the point's coordinates passed are within the circle's radius passed
/// @param      point_x             X coordinate of the point
/// @param      point_y             Y coordinate of the point
/// @param      circle_x            X coordinate of the circle
/// @param      circle_y            Y coordinate of the circle
/// @param      radius              Radius of the circle
bool isInsideCircle ( int point_x, int point_y, int circle_x, int circle_y, int radius )
{
    int distance = ( point_x - circle_x ) * ( point_x - circle_x ) + ( point_y - circle_y ) * ( point_y - circle_y );
    
    return ( distance <= ( radius * radius ) ) ? true : false;
}

#pragma mark - SDL

/// Draw circle outline
/// @param      renderer            SDL_Renderer
/// @param      x                   X coordinate of circle
/// @param      y                   Y coordinate of circle
/// @param      radius              Radius of circle
int SDL_RenderDrawCircle ( SDL_Renderer * renderer, int x, int y, int radius )
{
    int offsetx, offsety, d, status;

    offsetx = 0;
    offsety = radius;
    d       = radius -1;
    status  = 0;

    while (offsety >= offsetx)
    {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0)
        {
            status = -1;
            break;
        }

        if (d >= 2*offsetx)
        {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety))
        {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else
        {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

/// Draw circle fill
/// @param      renderer            SDL_Renderer
/// @param      x                   X coordinate of circle
/// @param      y                   Y coordinate of circle
/// @param      radius              Radius of circle
int SDL_RenderFillCircle ( SDL_Renderer * renderer, int x, int y, int radius )
{
    int offsetx, offsety, d, status;

    offsetx = 0;
    offsety = radius;
    d       = radius -1;
    status  = 0;

    while (offsety >= offsetx)
    {
        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx, x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety, x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety, x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx, x + offsety, y - offsetx);

        if (status < 0)
        {
            status = -1;
            break;
        }

        if (d >= 2*offsetx)
        {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety))
        {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else
        {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}
