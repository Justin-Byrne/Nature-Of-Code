//
//  helpers.hpp
//  Randomizer
//
//  Created by Justin Byrne on 5/8/22.
//

#ifndef helpers_hpp
#define helpers_hpp

#include <stdio.h>

int generate_random ( int lower, int upper );

void array_shift ( int * array, int SIZE_1D, bool shift_left, int positions_to_shift );

void array_shift ( int array[][2], int SIZE_1D, int SIZE_2D, bool shift_left, int positions_to_shift );

#endif /* helpers_hpp */
