//
//  main.cpp
//  Color Shifter
//
//  Created by Justin Byrne on 4/29/22.
//

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "include/colors.hpp"

#define WINDOW_WIDTH   464
#define WINDOW_HEIGHT  464
#define RECTANGLE_SIZE  16

#pragma mark - GLOBAL VARIABLE DECLARATIONS

SDL_Window   * window   = NULL;
SDL_Renderer * renderer = NULL;

bool run_loop           = true;

#pragma mark - GLOBAL FUNCTION DECLARATIONS

int setup_window ( const char* title, int x_pos, int y_pos, int width, int height );
void exit ( );
void poll_events ( );

#pragma mark - MAIN

int main( int argc, char * arg[] )
{
    setup_window( "Color Shifter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT );
    
    poll_events( );
    
    /* - - - - - - clean up - - - - - - */
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit( );
    /* - - - - - - clean up - - - - - - */

    return 0;
}

#pragma mark - GLOBAL FUNCTIONS

/*!
    @brief                              Setup window
    @param          title                                 Window title
    @param          x_pos                                 Window's x position
    @param          y_pos                                 Window's y position
    @param          width                                 Window's width
    @param          height                               Window's height
 */
int setup_window( const char* title, int x_pos, int y_pos, int width, int height )
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 ) {  SDL_Log( "ERROR SDL_Init" );  return -1;  }
    
    window = SDL_CreateWindow(
        title,                          // window title
        x_pos,                          // x position, centered
        y_pos,                          // y position, centered
        width,                          // width, in pixels
        height,                         // height, in pixels
        SDL_WINDOW_OPENGL               // flags
    );
    
    if ( !window ) {  SDL_Log( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );  return -1; }

    renderer = SDL_CreateRenderer(
        window,                         // window when rendering
        -1,                             // index of the rendering driver
        SDL_RENDERER_SOFTWARE           // rendering flags
    );
    
    if ( !renderer ) {  SDL_Log( "Failed to load renderer! SDL_Error: %s\n", SDL_GetError() );  return -1; }

    return 0;
}

/*!
    @brief                                  Initiates exit
 */
void exit ( )
{
    run_loop = false;
    
    printf ( "Done !" );
    
    SDL_Delay ( 5000 );
}

/*!
    @brief                              Initiate poll events
 */
void poll_events( )
{
    int i = 0;
    
    int horz_align = 0;
    int vert_align = 0;
    
    int screen_dimensions = WINDOW_WIDTH + WINDOW_HEIGHT;
    
    SDL_Rect rectangles[screen_dimensions];
    
    while( run_loop )
    {
        SDL_Event sdl_event;

        int x_coord = RECTANGLE_SIZE * horz_align;
        int y_coord = RECTANGLE_SIZE * vert_align;
        
        if ( x_coord + y_coord >= screen_dimensions )
        {
            exit ( );
            
            continue;
        }
        
        if ( x_coord >= ( WINDOW_WIDTH ) )
        {
            horz_align = 0;
            vert_align++;

            continue;
        }
        
        rectangles[i] = { x_coord, y_coord, RECTANGLE_SIZE, RECTANGLE_SIZE };
        
        int * colors = { get_gray_values ( ) };
        
        SDL_SetRenderDrawColor(         // background color
            renderer,                   // rendering context
            255,                        // red value
            255,                        // green value
            255,                        // blue value
            SDL_ALPHA_OPAQUE            // alpha value
        );
        
        SDL_SetRenderDrawColor(         // foreground color
            renderer,                   // rendering context
            colors[0],                  // red value
            colors[1],                  // green value
            colors[2],                  // blue value
            SDL_ALPHA_OPAQUE            // alpha value
        );

        SDL_RenderFillRect ( renderer, &rectangles[i] );

        SDL_RenderPresent( renderer );

        delete colors;

        i++;
        horz_align++;
        
        while ( SDL_PollEvent ( &sdl_event )  )
        {
            if ( sdl_event.type == SDL_QUIT )
                run_loop = false;
            else
                break;
        }
    }
}

