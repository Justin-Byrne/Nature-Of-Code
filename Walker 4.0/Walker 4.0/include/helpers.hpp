// @File: 		helpers.hpp
// @Project: 	Walker 4.0
// @Author:		Justin Byrne
// @Date:		Created 6/10/22 at 8:36 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#ifndef helpers_hpp
#define helpers_hpp

#include <SDL2/SDL.h>
#include <stdio.h>

#include "structs.hpp"

#pragma mark - GENERICS

int generate_random ( int lower, int upper );

#pragma mark - ARRAY

void array_shift ( int * array, int SIZE_1D, bool shift_left, int positions_to_shift );

void array_shift ( int array[][2], int SIZE_1D, int SIZE_2D, bool shift_left, int positions_to_shift );

#pragma mark - SDL

int SDL_RenderDrawCircle ( SDL_Renderer * renderer, int x, int y, int radius );

int SDL_RenderFillCircle ( SDL_Renderer * renderer, int x, int y, int radius );

int SDL_RenderDottedLine ( SDL_Renderer * renderer, COORDINATE origin, COORDINATE destination, int segments );

#endif /* helpers_hpp */
