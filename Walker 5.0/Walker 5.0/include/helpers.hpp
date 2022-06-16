// @File: 		helpers.hpp
// @Project: 	Walker 5.0
// @Author:		Justin Byrne
// @Date:		Created 6/16/22 at 8:05 AM
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

// COLOR

void set_render_draw_color  ( SDL_Renderer * renderer, RGB color = { 0, 0, 0 } );

void set_render_draw_colors ( SDL_Renderer * renderer, RGB background = { 255, 255, 255 }, RGB foreground = { 0, 0, 0 } );

// DRAW

int SDL_RenderDrawCircle    ( SDL_Renderer * renderer, COORDINATE coord, int radius );

int SDL_RenderDrawCircle    ( SDL_Renderer * renderer, COORDINATE coord, int radius, RGB color );


int SDL_RenderFillCircle    ( SDL_Renderer * renderer, COORDINATE coord, int radius );

int SDL_RenderFillCircle    ( SDL_Renderer * renderer, COORDINATE coord, int radius, RGB color );


int SDL_RenderDottedLine    ( SDL_Renderer * renderer, COORDINATE origin, COORDINATE destination, int segments );

int SDL_RenderDottedLine    ( SDL_Renderer * renderer, COORDINATE origin, COORDINATE destination, int segments, RGB color );


int SDL_RenderDrawLine      ( SDL_Renderer * renderer, COORDINATE origin, COORDINATE destination, RGB color );


int SDL_RenderDrawPoint     ( SDL_Renderer * renderer, COORDINATE origin, RGB color );

#endif /* helpers_hpp */
