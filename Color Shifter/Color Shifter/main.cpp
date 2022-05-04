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

#define PI 3.141592653589793
#define FREQUENCY 0.15
#define AMPLITUDE 255 / 2
#define CENTER    255 / 2

#define RANGE_OLD_MIN  -1
#define RANGE_OLD_MAX   1
#define RANGE_NEW_MIN   0
#define RANGE_NEW_MAX 255

#define PHASE_1 0
#define PHASE_2 2
#define PHASE_3 4

double range_old = RANGE_OLD_MAX - RANGE_OLD_MIN;
double range_new = RANGE_NEW_MAX - RANGE_NEW_MIN;

#pragma mark - GLOBAL VARIABLE DECLARATIONS

SDL_Window   *window    = NULL;
SDL_Renderer *renderer  = NULL;

int window_width        = 450;
int window_height       = 450;
bool run_loop           = true;

#pragma mark - GLOBAL FUNCTION DECLARATIONS

int setup_window( const char* title, int x_pos, int y_pos, int width, int height );
int get_color_value( );
void poll_events( );

#pragma mark - MAIN

int main( int argc, char * arg[] )
{
    setup_window( "Color", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height );
    
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

int i     = 0;
int calls = 0;      // tracks amount of times 'get_color_value()' is called to identify when to shift 'i'

int get_color_value( int phase )
{
    double sin_value = sin( ( FREQUENCY * i + phase ) * AMPLITUDE + CENTER );
    double new_value = ( ( ( sin_value - RANGE_OLD_MIN ) * range_new ) / range_old ) + RANGE_NEW_MIN;
    
    switch ( calls )
    {
        case 2:  calls = 0; i++;  break;
        default: calls++;         break;
    }
    
    return int ( (int) round( new_value ) );
}

int * get_color_values ( )
{
    return new int[3] { get_color_value( PHASE_1 ), get_color_value( PHASE_2 ), get_color_value( PHASE_3 ) };
}

int * get_gray_values ( )
{
    return new int[3] { get_color_value( PHASE_1 ), get_color_value( PHASE_1 ), get_color_value( PHASE_1 ) };
}

/*!
    @brief                              Initiate poll events
 */
void poll_events( )
{
    while( run_loop )
    {
        SDL_Event sdl_event;
        
//        int RED   = get_color_value( PHASE_1 );
//        int GREEN = get_color_value( PHASE_2 );
//        int BLUE  = get_color_value( PHASE_3 );
        
        int * colors = { get_gray_values( ) };
        
        SDL_SetRenderDrawColor(         // background color
            renderer,                   // rendering context
            colors[0],                  // red value
            colors[1],                  // green value
            colors[2],                  // blue value
            SDL_ALPHA_OPAQUE            // alpha value
        );
        
        delete colors;
        
        SDL_RenderClear( renderer );
        
        SDL_RenderPresent( renderer );

        SDL_Delay( 100 );
        
        while( SDL_PollEvent( &sdl_event )  )
        {
            switch( sdl_event.type )
            {
                case SDL_QUIT:
                    run_loop = false;
                    break;
            }
        }
    }
}

