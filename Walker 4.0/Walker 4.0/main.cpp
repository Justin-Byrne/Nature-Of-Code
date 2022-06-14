// @File: 		main.cpp
// @Project: 	Walker 4.0
// @Author:		Justin Byrne
// @Date:		Created 6/10/22 at 8:34 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <algorithm>

#include "include/structs.hpp"
#include "include/helpers.hpp"

#define DEBUG                    1

#define WINDOW_TITLE  "Walker 4.0"
#define WINDOW_WIDTH           500
#define WINDOW_HEIGHT          500
#define WALKER_MAX              50
#define DEPTH_MAX              100
#define BODY                    10
#define SENSE                   30

#pragma mark - GLOBAL VARIABLE DECLARATIONS

SDL_Window   * window   = NULL;
SDL_Renderer * renderer = NULL;

bool run_loop = true;

RGB colors[DEPTH_MAX] = { { 0, 0, 0 } };

#pragma mark - GLOBAL FUNCTION DECLARATIONS

int setup_window            ( const char * title, int x_pos, int y_pos, int width, int height );
void set_render_draw_color  ( RGB color = { 0, 0, 0 } );
void set_render_draw_colors ( RGB background = { 255, 255, 255 }, RGB foreground = { 0, 0, 0 } );
void exit                   ( const char * message );
void generate_colors        ( int start, int end );
void draw                   ( );

#pragma mark - DATA STRUCTURES

enum STATE
{
    SILENT,     // 0
    ROTATE,     // 1
    MOVING,     // 2
};

struct WALKER
{
    COORDINATE origin = { 0, 0 };
    COORDINATE steps[DEPTH_MAX];
    
    DEGREE degree;
    
    int state         = SILENT;
    int radius        = 0;
    int walk_distance = 0;
    
    // Constructors ......................................................... //
    
    WALKER ( COORDINATE origin, int radius )
    {
        this->origin = origin;
        this->radius = radius;
    }
    
    // Constructors (Generic) ....... //
    
    WALKER  ( ) { };

    ~WALKER ( ) { };

    // Functions ............................................................ //

    // > .. Getters ................. //
    
    COORDINATE rotate ( int degree )
    {
        this->state = ROTATE;
        
        return DEGREE().rotate ( this->origin, degree );
    }
    
    COORDINATE rotate ( int degree, int step_length )
    {
        this->state = ROTATE;
        
        return DEGREE().rotate ( this->origin, degree, step_length );
    }
    
    // > .. Iterators ............... //
    
    void next_step ( int step_size = 3 )
    {
        cache_steps ( );
        
        this->origin = DEGREE().rotate ( this->origin, this->degree.a, step_size );
        
        this->walk_distance--;
        
        check_boundary ( );
    }
    
    // > .. Validators .............. //
    
    void check_boundary ( )
    {
        this->origin.x = ( this->origin.x <= 0 ) ? 1 : this->origin.x;                              // left
        this->origin.x = ( this->origin.x >= WINDOW_WIDTH  ) ? WINDOW_WIDTH  - 1 : this->origin.x;  // right
        
        this->origin.y = ( this->origin.y <= 0 ) ? 1                             : this->origin.y;  // top
        this->origin.y = ( this->origin.y >= WINDOW_HEIGHT ) ? WINDOW_HEIGHT - 1 : this->origin.y;  // bottom
    }

    bool is_inside_bubble ( COORDINATE bubble )
    {
        return ( ( bubble.x - this->origin.x ) * ( bubble.x - this->origin.x ) +
                 ( bubble.y - this->origin.y ) * ( bubble.y - this->origin.y ) <= ( this->radius * this->radius ) ) ? true : false;
    }
    
    // > .. Misc .................... //
    
    void cache_steps ( )
    {
        for ( int i = DEPTH_MAX - 1; i > 0; i-- )
            this->steps[i] = this->steps[i - 1];
        
        this->steps[0] = this->origin;
    }
};

#pragma mark - MAIN

int main ( int argc, char * arg[] )
{
    setup_window ( WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT );
    
    srand ( (unsigned) time (0) );                          // seed randomizer
    
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
    
    return 0;
}

/// Sets  foreground of the renderer
/// @param      color               RGB values for foreground
void set_render_draw_color ( RGB color )
{
    SDL_SetRenderDrawColor (        // background color
        renderer,                   // rendering context
        color.red,                  // red value
        color.green,                // green value
        color.blue,                 // blue value
        SDL_ALPHA_OPAQUE            // alpha value
    );
}

/// Sets background and foreground of the renderer
/// @param      background          RGB values for background
/// @param      foreground          RGB values for foreground
void set_render_draw_colors ( RGB background, RGB foreground )
{
    SDL_SetRenderDrawColor (        // background color
        renderer,                   // rendering context
        background.red,             // red value
        background.green,           // green value
        background.blue,            // blue value
        SDL_ALPHA_OPAQUE            // alpha value
    );

    SDL_RenderClear ( renderer );
    
    SDL_SetRenderDrawColor (        // background color
        renderer,                   // rendering context
        foreground.red,             // red value
        foreground.green,           // green value
        foreground.blue,            // blue value
        SDL_ALPHA_OPAQUE            // alpha value
    );
}

/// Exits program
/// @param      message             Message to be left after deactivating program
void exit ( const char * message )
{
    run_loop = false;
    
    printf ( "[EXIT] By program !\n%s\n", message );
}

/// Generates a series of gray colors
/// @param      highest_color                 Highest number, preferable white... RGB values
void generate_colors ( int highest_color )
{
    highest_color  = std::clamp ( highest_color, 0, 255 );
    
    int difference = highest_color / DEPTH_MAX;
    
    for ( int i = 0; i < DEPTH_MAX; i++ )
            colors[i] = { ( i * difference ), ( i * difference ), ( i * difference ) };
}

#pragma mark - DRAW

/// Initiate poll events
void draw ( )
{
    /* - - - - - - - - - - - - - - - - - Init - - - - - - - - - - - - - - - - - */
    
    SDL_Event sdl_event;

    WALKER walker[WALKER_MAX];

    generate_colors ( 255 );
    
    int padding = 50;
    
    for ( int i = 0; i < WALKER_MAX; i++ )
    {
        walker[i]        = { COORDINATE { (double) generate_random ( padding, WINDOW_WIDTH - padding ), (double) generate_random ( padding, WINDOW_HEIGHT - padding ) }, BODY };
        walker[i].degree = { generate_random ( 0, 360 ), generate_random ( 0, 360 ) };
    }

    /* - - - - - - - - - - - - - - - - - Init - - - - - - - - - - - - - - - - - */
    
    while ( run_loop )                                                          // DRAW
    {
        COORDINATE rotate_coordinate, rotate_destination;
        
        set_render_draw_colors ( );
        
        for ( int i = 0; i < WALKER_MAX; i++ )
        {
            set_render_draw_color ( RGB ( 0, 0, 0 ) );
            SDL_RenderDrawPoint   ( renderer, walker[i].origin.x, walker[i].origin.y );
            
            #if DEBUG
            set_render_draw_color ( RGB ( 225, 225, 225 ) );
            SDL_RenderDrawCircle  ( renderer, walker[i].origin.x, walker[i].origin.y, BODY );
            #endif
            
            switch ( walker[i].state )
            {
                case SILENT:
                case ROTATE:
                    
                    #if DEBUG
                    rotate_coordinate     = walker[i].rotate ( walker[i].degree.a, BODY );
                    set_render_draw_color ( RGB ( 100, 100, 100 ) );
                    SDL_RenderDrawLine    ( renderer, walker[i].origin.x, walker[i].origin.y, rotate_coordinate.x, rotate_coordinate.y );      // Draw: current sightline

                    rotate_destination    = walker[i].rotate ( walker[i].degree.b, 20 );
                    set_render_draw_color ( RGB (  50,  50,  50 ) );
                    SDL_RenderDrawLine    ( renderer, walker[i].origin.x, walker[i].origin.y, rotate_destination.x, rotate_destination.y );    // Draw: destination sightline
                    #endif
                    
                    ( walker[i].degree.clockwise ) ? walker[i].degree.advance ( ) : walker[i].degree.regress ( );
                    
                    if ( walker[i].degree.distance == 0 )
                    {
                        walker[i].state         = MOVING;
                        walker[i].walk_distance = generate_random ( 10, 30 );
                    }
                    
                    break;
                    
                case MOVING:
                    
                    walker[i].next_step ( 2 );

                    if ( walker[i].walk_distance == 0 )
                    {
                        walker[i].state  = SILENT;
                        walker[i].degree = { walker[i].degree.b, generate_random ( 0, 360 ) };
                    }
                    
                    break;
            }
            
            for ( int j = 0; j < DEPTH_MAX; j++ )                               // Draw trailing steps
            {
                set_render_draw_color ( colors[j] );
                SDL_RenderDrawPoint   ( renderer, walker[i].steps[j].x, walker[i].steps[j].y );
            }
        }
        
        SDL_RenderPresent ( renderer );                                         // Update: renderer... polls for ~500 ms
        
        SDL_Delay ( 50 );

        while ( SDL_PollEvent ( &sdl_event )  )
        {
            if ( sdl_event.type == SDL_QUIT )
                run_loop = false;
            else
                break;
        }
    }
}
