//
//  helpers.cpp
//  Random Walker
//
//  Created by Justin Byrne on 5/2/22.
//

#include "../include/helpers.hpp"

#include <stdlib.h>

/*!
    @brief                                  Generates a random number in the range passed
    @param          lower                   Lower bounds value
    @param          upper                   Upper bounds value
 */
int generate_random ( int lower, int upper )
{
    return ( rand () % (upper - lower + 1) ) + lower;
}

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
