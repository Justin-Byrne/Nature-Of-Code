//
//  main.cpp
//  Randomizer
//
//  Created by Justin Byrne on 5/8/22.
//

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "include/helpers.hpp"

#define WINDOW_TITLE  "Randomizer"
#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

#pragma mark - GLOBAL VARIABLE DECLARATIONS

SDL_Window   * window   = NULL;
SDL_Renderer * renderer = NULL;

bool run_loop = true;

#pragma mark - GLOBAL FUNCTION DECLARATIONS

int setup_window ( const char * title, int x_pos, int y_pos, int width, int height );
void exit        ( const char * message );
void draw        ( );

#pragma mark - DATA STRUCTURES

enum TYPE
{
    POINTS_Y = 0,
    LINES_Y,
    POINTS_XY,
    LINES_XY
};

#pragma mark - MAIN

int main ( int argc, char * arg[] )
{
    setup_window ( WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT );
    
    srand ( (unsigned) time (0) );  // seed randomizer
    
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
int setup_window ( const char * title, int x_pos, int y_pos, int width, int height )
{
    if ( SDL_Init (SDL_INIT_EVERYTHING) != 0 ) {  SDL_Log ( "ERROR SDL_Init" );  return -1;  }
    
    window = SDL_CreateWindow (
        title,                      // window title
        x_pos,                      // x position, centered
        y_pos,                      // y position, centered
        width,                      // width, in pixels
        height,                     // height, in pixels
        SDL_WINDOW_OPENGL           // flags
    );
    
    if ( !window ) {  SDL_Log ( "Window could not be created! SDL_Error: %s\n", SDL_GetError () );  return -1; }

    renderer = SDL_CreateRenderer (
        window,                     // window when rendering
        -1,                         // index of the rendering driver
        SDL_RENDERER_SOFTWARE       // rendering flags
    );
    
    if ( !renderer ) {  SDL_Log ( "Failed to load renderer! SDL_Error: %s\n", SDL_GetError () );  return -1; }
    
    SDL_SetRenderDrawColor (        // background color
        renderer,                   // rendering context
        255,                        // red value
        255,                        // green value
        255,                        // blue value
        SDL_ALPHA_OPAQUE            // alpha value
    );

    SDL_RenderClear ( renderer );

    SDL_SetRenderDrawColor (        // foreground color
        renderer,                   // rendering context
        0,                          // red value
        0,                          // green value
        0,                          // blue value
        SDL_ALPHA_OPAQUE            // alpha value
    );
    
    return 0;
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
    SDL_Event sdl_event;
    
    int i = 0;
    
    int random_x, random_y, last_x = 0, last_y = WINDOW_HEIGHT / 2;

    while ( run_loop )
    {
        switch ( LINES_Y )
        {
            case 0:                     // POINTS_Y
                
                random_y = generate_random ( 0, WINDOW_HEIGHT );

                SDL_RenderDrawPoint ( renderer, i, random_y );
                
                SDL_RenderPresent ( renderer );
                
                i++;
                
                if ( i >= WINDOW_WIDTH )
                    exit ( "Done !" );
                
                break;
                
            case 1:                     // LINES_Y
                
                random_y = generate_random ( 0, WINDOW_HEIGHT );
                
                SDL_RenderDrawLine ( renderer, last_x, last_y, i, random_y );
                
                last_x = i++;           // Assign 'last_x' and iterate 'i' afterwards
                last_y = random_y;
                
                if ( i >= WINDOW_WIDTH )
                    exit ( "Done !" );
                
                break;
                
            case 2:                     // POINTS_XY
                
                random_x = generate_random ( 0, WINDOW_WIDTH  );
                random_y = generate_random ( 0, WINDOW_HEIGHT );
                
                SDL_RenderDrawPoint( renderer, random_x, random_y );
                
                break;
            
            case 3:                     // LINES_XY
                
                random_x = generate_random ( 0, WINDOW_WIDTH  );
                random_y = generate_random ( 0, WINDOW_HEIGHT );
                
                SDL_RenderDrawLine ( renderer, last_x, last_y, random_x, random_y );
                
                last_x = random_x;
                last_y = random_y;
                
                break;
        }
        
        SDL_RenderPresent ( renderer );
        
        while ( SDL_PollEvent ( &sdl_event )  )
        {
            if ( sdl_event.type == SDL_QUIT )
                run_loop = false;
            else
                break;
        }
    }
}
