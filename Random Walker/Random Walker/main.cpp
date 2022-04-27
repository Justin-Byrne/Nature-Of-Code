//
//  main.cpp
//  Random Walker
//
//  Created by Justin Byrne on 4/27/22.
//

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#pragma mark - GLOBAL VARIABLE DECLARATIONS

SDL_Window   *window    = NULL;
SDL_Renderer *renderer  = NULL;

int window_width        = 450;
int window_height       = 450;
bool run_loop           = true;

#pragma mark - GLOBAL FUNCTION DECLARATIONS

int setup_window( const char* title, int x_pos, int y_pos, int width, int height );
int generate_random( int lower, int upper );
void poll_events( );

#pragma mark - DATA STRUCTURES

struct walker
{
    int x = 0;
    int y = 0;
    
    time_t time_seed;
    
    walker( int x, int y )
    {
        this->x = x;
        this->y = y;
    }
    
    void next_step( )
    {
        int rand_number = generate_random( 0, 7 );
        
        switch ( rand_number )
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

int main( int argc, char * arg[] )
{
    setup_window( "Walker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height );
    
    srand( (unsigned) time(0) );        // seed randomizer
    
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
    
    SDL_SetRenderDrawColor(             // background color
        renderer,                       // rendering context
        255,                            // red value
        255,                            // green value
        255,                            // blue value
        SDL_ALPHA_OPAQUE                // alpha value
    );

    SDL_RenderClear( renderer );
    
    SDL_SetRenderDrawColor(             // foreground color
        renderer,                       // rendering context
        0,                              // red value
        0,                              // green value
        0,                              // blue value
        SDL_ALPHA_OPAQUE                // alpha value
    );
    
    return 0;
}

/*!
    @brief                              Generates a random number in the range passed
    @param          lower                                 Lower bounds value
    @param          upper                                 Upper bounds value
 */
int generate_random( int lower, int upper )
{
    return ( rand() % (upper - lower + 1) ) + lower;
}

/*!
    @brief                              Initiate poll events
 */
void poll_events( )
{
    walker walker( window_width / 2, window_height / 2 );
    
    while( run_loop )
    {
        SDL_Event sdl_event;
        
        SDL_RenderDrawPoint( renderer, walker.x, walker.y );

        SDL_RenderPresent( renderer );
        
        walker.next_step();
        
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
