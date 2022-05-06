//
//  main.cpp
//  Random Walker
//
//  Created by Justin Byrne on 4/27/22.
//

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/colors.hpp"
#include "include/helpers.hpp"

#define WALKER_MAX 500
#define MAX_DEPTH  10

#pragma mark - GLOBAL VARIABLE DECLARATIONS

SDL_Window   *window   = NULL;
SDL_Renderer *renderer = NULL;

int window_width  = 450;
int window_height = 450;
bool run_loop     = true;

int colors[MAX_DEPTH][3] = { { 0 } };

#pragma mark - GLOBAL FUNCTION DECLARATIONS

int setup_window ( const char* title, int x_pos, int y_pos, int width, int height );
void generate_colors ( int start, int end );
void main_loop ( );

int walker_steps[WALKER_MAX][MAX_DEPTH][2] = { { 0 } };

#pragma mark - DATA STRUCTURES

struct WALKER
{
    int x     = 0;
    int y     = 0;
    
    time_t time_seed;
    
    WALKER ( int x, int y )
    {
        this->x = x;
        this->y = y;
    }
    
    WALKER() { };
    
    ~WALKER() { };
    
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
    }
};

#pragma mark - MAIN

int main ( int argc, char * arg[] )
{
    setup_window ( "Walker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height );
    
    srand ( (unsigned) time (0) );  // seed randomizer
    
    main_loop ( );
    
    /* - - - - - - clean up - - - - - - */
    SDL_DestroyRenderer ( renderer );
    SDL_DestroyWindow ( window );
    SDL_Quit ( );
    /* - - - - - - clean up - - - - - - */

    return 0;
}

#pragma mark - GLOBAL FUNCTIONS

/*!
    @brief                                  Setup window
    @param          title                   Window title
    @param          x_pos                   Window's x position
    @param          y_pos                   Window's y position
    @param          width                   Window's width
    @param          height                  Window's height
 */
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
    
    SDL_SetRenderDrawColor (             // background color
        renderer,                       // rendering context
        255,                            // red value
        255,                            // green value
        255,                            // blue value
        SDL_ALPHA_OPAQUE                // alpha value
    );

    SDL_RenderClear ( renderer );
    
    SDL_SetRenderDrawColor (             // foreground color
        renderer,                       // rendering context
        0,                              // red value
        0,                              // green value
        0,                              // blue value
        SDL_ALPHA_OPAQUE                // alpha value
    );
    
    return 0;
}

/*!
    @brief                                  Generates a series of gray colors
    @param          start                   Lowest number; preferable black... RGB values
    @param          end                     Highest number, preferable white... RGB values
 */
void generate_colors ( int start, int end )
{
    int i, j;
    int difference = end / MAX_DEPTH;
    
    for ( i = 0; i < 3; i++ )
        colors[0][i] = start;
    
    for ( i = 1; i < ( MAX_DEPTH - 1 ); i++ )
        for ( j = 0; j < 3; j++ )
            colors[i][j] = { i * difference };
    
    for ( i = 0; i < 3; i++ )
        colors[MAX_DEPTH - 2][i] = end;
}

/*!
    @brief                                  Initiate poll events
 */
void main_loop ( )
{
    WALKER walker[WALKER_MAX];

    int iter;
    
    for ( iter = 0; iter < WALKER_MAX; iter++ )
    {
        walker[iter] = { generate_random ( 0, window_width ), generate_random ( 0, window_height ) };
    }
    
    generate_colors ( 0, 255 );
    
    SDL_Event sdl_event;
    
    while ( run_loop )
    {
        int i, j;
        
        for ( i = 0; i < WALKER_MAX; i++ ) {  walker_steps[i][0][0] = walker[i].x;  walker_steps[i][0][1] = walker[i].y;  }
        
        i = 0;
        
        while ( i < MAX_DEPTH )
        {
            if ( walker_steps[0][i][0] > 0 && walker_steps[0][i][1] > 0 )
            {
                for ( j = 0; j < WALKER_MAX; j++ )
                    SDL_RenderDrawPoint ( renderer, walker_steps[j][i][0], walker_steps[j][i][1] );
                
                SDL_SetRenderDrawColor (    // foreground color
                    renderer,               // rendering context
                    colors[i][0],           // red value
                    colors[i][1],           // green value
                    colors[i][2],           // blue value
                    SDL_ALPHA_OPAQUE        // alpha value
                );
                
                SDL_RenderPresent ( renderer );
            }
            else
            {
                break;
            }
            
            i++;
        }
        
        for ( i = 0; i < WALKER_MAX; i++ )
            array_shift ( walker_steps[i], MAX_DEPTH, 2, false, 1 );
        
        for ( i = 0; i < WALKER_MAX; i++ )
            walker[i].next_step ();
        
        while ( SDL_PollEvent ( &sdl_event )  )
        {
            if ( sdl_event.type == SDL_QUIT )
                run_loop = false;
            else
                break;
        }
    }
}
