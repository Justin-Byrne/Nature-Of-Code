// @File: 		main.cpp
// @Project: 	Walker 5.0
// @Author:		Justin Byrne
// @Date:		Created 6/16/22 at 8:03 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <algorithm>

#include "include/structs.hpp"
#include "include/helpers.hpp"

#define DEBUG_ROTATE             0
#define DEBUG_BODY               1
#define DEBUG_SENSE              0
#define DEBUG_SIGHTLINE          1
#define DEBUG_TRAILS             0

#define WINDOW_TITLE  "Walker 5.0"
#define WINDOW_WIDTH           800
#define WINDOW_HEIGHT          800
#define WALKER_MAX             100
#define DEPTH_MAX              100
#define REGION_BODY             10
#define REGION_SENSE            70
#define LINE_SEGMENTS            6

#pragma mark - GLOBAL VARIABLE DECLARATIONS

SDL_Window   * window   = NULL;
SDL_Renderer * renderer = NULL;

bool run_loop = true;

RGB colors[DEPTH_MAX] = { { 0, 0, 0 } };

#pragma mark - GLOBAL FUNCTION DECLARATIONS

int  setup_window    ( const char * title, int x_pos, int y_pos, int width, int height );
void exit            ( const char * message );
void generate_colors ( int start, int end );
void draw            ( );

#pragma mark - DATA STRUCTURES

enum STATE
{
    SILENT,     // 0
    ROTATE,     // 1
    MOVING,     // 2
};

struct WALKER
{
    int id;
    static int ID;
    
    int state         = SILENT;
    int radius        = 0;
    int walk_distance = 0;
    
    COORDINATE origin = { 0, 0 };
    COORDINATE steps[DEPTH_MAX];
    
    DEGREE degree;
    
    // Constructors ......................................................... //
    
    WALKER ( COORDINATE origin, int radius )
    {
        this->origin = origin;
        this->radius = radius;
        this->id     = ID++;
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

    double get_distance ( COORDINATE point_a, COORDINATE point_b )
    {
        return std::sqrt ( point_a.x - point_b.x * point_a.x - point_b.x + point_a.y - point_b.y * point_a.y - point_b.y );
    }
    
    bool is_inside_region ( COORDINATE region )
    {
        return ( ( region.x - this->origin.x ) * ( region.x - this->origin.x ) +
                 ( region.y - this->origin.y ) * ( region.y - this->origin.y ) <= ( this->radius * this->radius ) ) ? true : false;
    }
    
    // > .. Misc .................... //
    
    void cache_steps ( )
    {
        for ( int i = DEPTH_MAX - 1; i > 0; i-- )
            this->steps[i] = this->steps[i - 1];
        
        this->steps[0] = this->origin;
    }
};

int WALKER::ID;

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
        walker[i]        = { COORDINATE { (double) generate_random ( padding, WINDOW_WIDTH - padding ), (double) generate_random ( padding, WINDOW_HEIGHT - padding ) }, REGION_SENSE };
        walker[i].degree = { generate_random ( 0, 360 ), generate_random ( 0, 360 ) };
    }

    COORDINATE rotate_origin, rotate_destination;
    
    /* - - - - - - - - - - - - - - - - - Init - - - - - - - - - - - - - - - - - */
    
    while ( run_loop )                                                          // DRAW
    {
        set_render_draw_colors ( renderer );     // TODO: checke whether can go outside 'while' loop
        
        for ( int i = 0; i < WALKER_MAX; i++ )
        {
            SDL_RenderDrawPoint (renderer, walker[i].origin, RGB ( 0, 0, 0 ) );
            
            #if DEBUG_BODY
            SDL_RenderDrawCircle ( renderer, walker[i].origin, REGION_BODY, RGB ( 200, 200, 200 ) );
            #endif
            
            #if DEBUG_SENSE
            SDL_RenderDrawCircle ( renderer, walker[i].origin, REGION_SENSE, RGB ( 240, 240, 240 ) );
            #endif
            
            switch ( walker[i].state )
            {
                case SILENT:
                case ROTATE:
                    
                    #if DEBUG_ROTATE
                    rotate_origin      = walker[i].rotate ( walker[i].degree.a, REGION_BODY );
                    SDL_RenderDrawLine ( renderer, walker[i].origin, rotate_origin, RGB ( 100, 100, 100 ) );

                    rotate_destination = walker[i].rotate ( walker[i].degree.b, REGION_BODY );
                    SDL_RenderDrawLine ( renderer, walker[i].origin, rotate_destination, RGB (  50,  50,  50 ) );
                    #endif
                    
                    ( walker[i].degree.clockwise ) ? walker[i].degree.advance ( ) : walker[i].degree.regress ( );
                    
                    if ( walker[i].degree.distance == 0 )
                    {
                        walker[i].state         = MOVING;
                        walker[i].walk_distance = generate_random ( 1, 30 );
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
            
            #if DEBUG_SIGHTLINE
            for ( int j = i + 1; j < WALKER_MAX; j++ )                          // Draw connecting sightline to nearby entity
                if ( walker[i].is_inside_region( COORDINATE { walker[j].origin.x, walker[j].origin.y } ) )
                    SDL_RenderDottedLine ( renderer, walker[i].origin, walker[j].origin, LINE_SEGMENTS, RGB ( 225, 0, 1 ) );
            #endif
            
            #if DEBUG_TRAILS
            for ( int j = 0; j < DEPTH_MAX; j++ )                               // Draw trailing steps
                SDL_RenderDrawPoint ( renderer, walker[i].steps[j], colors[j] );
            #endif
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
