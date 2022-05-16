//
//  main.cpp
//  Walker 2.0
//
//  Created by Justin Byrne on 5/16/22.
//

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/colors.hpp"
#include "include/helpers.hpp"

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500
#define WALKER_MAX     10
#define MAX_DEPTH      25

#pragma mark - GLOBAL VARIABLE DECLARATIONS

SDL_Window   * window   = NULL;
SDL_Renderer * renderer = NULL;

bool run_loop = true;

int colors[MAX_DEPTH][3]                   = { { 0 } };
int walker_steps[WALKER_MAX][MAX_DEPTH][2] = { { 0 } };

#pragma mark - GLOBAL FUNCTION DECLARATIONS

int setup_window ( const char* title, int x_pos, int y_pos, int width, int height );
void generate_colors ( int start, int end );
void exit ( const char * message );
void draw ( );

#pragma mark - DATA STRUCTURES

struct WALKER
{
    int x = 0;
    int y = 0;
    
    time_t time_seed;
    
    WALKER ( int x, int y )
    {
        this->x = x;
        this->y = y;
    }
    
    WALKER ( ) { };
    
    ~WALKER ( ) { };
    
    void next_step ( )
    {
        switch ( generate_random ( 0, 7 ) )
        {
            case 0:  this->y++;             break;  // up
            case 1:  this->y++; this->x++;  break;  // up right
            case 2:  this->x++;             break;  // right
            case 3:  this->y--; this->x++;  break;  // down right
            case 4:  this->y--;             break;  // down
            case 5:  this->y--; this->x--;  break;  // down left
            case 6:  this->x--;             break;  // left
            case 7:  this->y++; this->x--;  break;  // up left
        }
        
        if ( x <= 0 ) x++;              // Basic collision code for bounds of window
        if ( y <= 0 ) y++;
        if ( x >= WINDOW_WIDTH  ) x--;
        if ( y >= WINDOW_HEIGHT ) y--;
    }
};

#pragma mark - MAIN

int main ( int argc, char * arg[] )
{
    setup_window ( "Walker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT );
    
    srand ( (unsigned) time (0) );      // seed randomizer
    
    draw ( );
    
    /* - - - - - - clean up - - - - - - */
    SDL_DestroyRenderer ( renderer );
    SDL_DestroyWindow ( window );
    SDL_Quit ( );
    /* - - - - - - clean up - - - - - - */

    return 0;
}

#pragma mark - GLOBAL FUNCTIONS

/// Setup window
/// @param      title               Window title
/// @param      x_pos               Window's x position
/// @param      y_pos               Window's y position
/// @param      width               Window's width
/// @param      height              Window's height
int setup_window ( const char* title, int x_pos, int y_pos, int width, int height )
{
    if ( SDL_Init (SDL_INIT_EVERYTHING) != 0 ) {  SDL_Log ( "ERROR SDL_Init" );  return -1;  }
    
    window = SDL_CreateWindow (
        title,                          // window title
        x_pos,                          // x position, centered
        y_pos,                          // y position, centered
        width,                          // width, in pixels
        height,                         // height, in pixels
        SDL_WINDOW_OPENGL               // flags
    );
    
    if ( !window ) {  SDL_Log ( "Window could not be created! SDL_Error: %s\n", SDL_GetError () );  return -1; }

    renderer = SDL_CreateRenderer (
        window,                         // window when rendering
        -1,                             // index of the rendering driver
        SDL_RENDERER_SOFTWARE           // rendering flags
    );
    
    if ( !renderer ) {  SDL_Log ( "Failed to load renderer! SDL_Error: %s\n", SDL_GetError () );  return -1; }
    
    return 0;
}

/// Generates a series of gray colors
/// @param      start               Lowest number; preferable black... RGB values
/// @param      end                 Highest number, preferable white... RGB values
void generate_colors ( int start, int end )
{
    if ( MAX_DEPTH > 255 || end > 255 )
        exit ( "Maximum values for 'MAX_DEPTH' and 'end' variables are 255!" );
    
    int difference = end / MAX_DEPTH;
    
    for ( i = 0; i < MAX_DEPTH; i++ )
        for ( int j = 0; j < 3; j++ )
            colors[i][j] = { i * difference };
}

/// Exits program
/// @param      message             Message to be left after deactivating program
void exit ( const char * message )
{
    run_loop = false;
    
    printf ( "[EXIT] By program !\n%s\n", message );
}

/// Initiate poll events
void draw ( )
{
    /* - - - - - - - - - - - - - - - - - Init - - - - - - - - - - - - - - - - - */
    
    SDL_Event sdl_event;

    WALKER walker[WALKER_MAX];

    for ( i = 0; i < WALKER_MAX; i++ )
        walker[i] = { generate_random ( 0, WINDOW_WIDTH ), generate_random ( 0, WINDOW_HEIGHT ) };

    generate_colors ( 0, 255 );

    /* - - - - - - - - - - - - - - - - - Init - - - - - - - - - - - - - - - - - */
    
    while ( run_loop )                                                          // DRAW
    {
        SDL_SetRenderDrawColor (    // background color
            renderer,               // rendering context
            255,                    // red value
            255,                    // green value
            255,                    // blue value
            SDL_ALPHA_OPAQUE        // alpha value
        );

        SDL_RenderClear ( renderer );
        
        for ( i = 0; i < WALKER_MAX; i++ )                                      // Store: key 'walker' values within walker_steps
        {
            walker_steps[i][0][0] = walker[i].x;
            walker_steps[i][0][1] = walker[i].y;
        }
        
        for ( i = 0; i < WALKER_MAX; i++ )                                      // Draw: walkers
        {
            for ( int j = 0; j < MAX_DEPTH; j++ )
            {
                if ( walker_steps[i][j][0] <= 0 || walker_steps[i][j][1] <= 0 ) break;

                SDL_RenderDrawCircle ( renderer, walker_steps[i][0][0], walker_steps[i][0][1], 50 );
                SDL_RenderDrawPoint  ( renderer, walker_steps[i][j][0], walker_steps[i][j][1]     );

                SDL_SetRenderDrawColor (    // foreground color
                    renderer,               // rendering context
                    colors[j][0],           // red value
                    colors[j][1],           // green value
                    colors[j][2],           // blue value
                    SDL_ALPHA_OPAQUE        // alpha value
                );
            }
        }
        
        SDL_RenderPresent ( renderer );                                         // Update: renderer... polls for ~500
        
        SDL_Delay ( 50 );
        
        for ( i = 0; i < WALKER_MAX; i++ )                                      // Array shift all 'walker_steps'
            array_shift ( walker_steps[i], MAX_DEPTH, 2, false, 1 );
        
        for ( i = 0; i < WALKER_MAX; i++ )                                      // Init: next 'walker' step
            walker[i].next_step ( );

        while ( SDL_PollEvent ( &sdl_event )  )
        {
            if ( sdl_event.type == SDL_QUIT )
                run_loop = false;
            else
                break;
        }
    }
}
