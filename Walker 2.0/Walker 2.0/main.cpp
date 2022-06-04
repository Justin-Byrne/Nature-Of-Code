//
//  main.cpp
//  Walker 2.0
//
//  Created by Justin Byrne on 5/16/22.
//

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <algorithm>

#include "include/colors.hpp"
#include "include/helpers.hpp"

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500
#define WALKER_MAX     10
#define DEPTH_MAX      10
#define SENSE_BUBBLE   50

#define PI 3.141592653589
#define SIN(x) sin(x * PI / 180)
#define COS(x) cos(x * PI / 180)

#pragma mark - GLOBAL VARIABLE DECLARATIONS

SDL_Window   * window   = NULL;
SDL_Renderer * renderer = NULL;

bool run_loop = true;
bool debug    = false;

int colors[DEPTH_MAX][3]                   = { { 0 } };
int walker_steps[WALKER_MAX][DEPTH_MAX][2] = { { 0 } };

#pragma mark - GLOBAL FUNCTION DECLARATIONS

int  setup_window    ( const char* title, int x_pos, int y_pos, int width, int height );
void generate_colors ( int start, int end );
void exit            ( const char * message );
void draw            ( );

#pragma mark - DATA STRUCTURES

enum DIRECTION
{
    UP,         // 0
    UP_RIGHT,   // 1
    RIGHT,      // 2
    DOWN_RIGHT, // 3
    DOWN,       // 4
    DOWN_LEFT,  // 5
    LEFT,       // 6
    UP_LEFT     // 7
};

struct POINT
{
    int x, y;
    
    POINT ( int x, int y )
    {
        this->x = x;
        this->y = y;
    }
    
    ~POINT ( ) { };
};

struct WALKER
{
    POINT origin     = { 0, 0 };
    POINT point      = { 0, 0 };

    int point_length = 35;
    int radius       = 0;
    
    time_t time_seed;
    
    // Constructors ......................................................... //
    
    WALKER ( POINT origin )
    {
        this->origin = origin;
    }
    
    WALKER ( POINT origin, int radius )
    {
        this->origin = origin;
        this->radius = radius;
    }
    
    WALKER ( POINT origin, POINT point )
    {
        this->origin = origin;
        this->point  = point;
    }
    
    WALKER ( POINT origin, POINT point, int radius )
    {
        this->origin = origin;
        this->point  = point;
        this->radius = radius;
    }
    
    // Constructors (Generic) ... //
    
    WALKER ( )  { };

    ~WALKER ( ) { };

    // Functions ............................................................ //
    
    void next_step ( int direction )
    {
        switch ( direction )
        {
            case 0:  this->origin.y++;                    break;                // up
            case 1:  this->origin.y++; this->origin.x++;  break;                // up right
            case 2:  this->origin.x++;                    break;                // right
            case 3:  this->origin.y--; this->origin.x++;  break;                // down right
            case 4:  this->origin.y--;                    break;                // down
            case 5:  this->origin.y--; this->origin.x--;  break;                // down left
            case 6:  this->origin.x--;                    break;                // left
            case 7:  this->origin.y++; this->origin.x--;  break;                // up left
        }
        
        check_boundary ( );
    }
    
    void check_boundary ( )
    {
        if ( this->origin.x <= 0 )              this->origin.x++;               // right
        if ( this->origin.y <= 0 )              this->origin.y++;               // top
        if ( this->origin.x >= WINDOW_WIDTH  )  this->origin.x--;               // left
        if ( this->origin.y >= WINDOW_HEIGHT )  this->origin.y--;               // bottom
    }

    bool isInsideCircle ( POINT circle )
    {
        return ( ( circle.x - this->origin.x ) * ( circle.x - this->origin.x ) +
                 ( circle.y - this->origin.y ) * ( circle.y - this->origin.y ) <= ( this->radius * this->radius ) ) ? true : false;
    }

    double convertToRadian ( int degree )
    {
        return ( degree * PI / 180 );
    }

    int convertToDegree ( float radian )
    {
        return ( radian * 180 ) / PI;
    }
    
    POINT rotate ( int degree )
    {
        POINT point    = { this->origin.x + this->point_length, this->origin.y };
        
        double radians = convertToRadian ( degree );
        
        double sine    = sin ( radians );
        double cosine  = cos ( radians );

        point.x       -= this->origin.x;                                        // translate point back to origin
        point.y       -= this->origin.y;

        double x_new  = point.x * cosine - point.y * sine;                      // rotate point
        double y_new  = point.x * sine   - point.y * cosine;
        
        point.x       = x_new + this->origin.x;                                 // translate point back
        point.y       = y_new + this->origin.y;
        
        return point;
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
int setup_window ( const char * title, int x_pos, int y_pos, int width, int height )
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
    end   = std::clamp ( end,   0, 255 );
    start = std::clamp ( start, 0, 255 );
    
    int difference = end / DEPTH_MAX;
    
    for ( i = 0; i < DEPTH_MAX; i++ )
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
        walker[i] = { POINT { generate_random ( 0, WINDOW_WIDTH ), generate_random ( 0, WINDOW_HEIGHT ) }, SENSE_BUBBLE };

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
            walker_steps[i][0][0] = walker[i].origin.x;
            walker_steps[i][0][1] = walker[i].origin.y;
        }
        
        for ( i = 0; i < WALKER_MAX; i++ )                                      // Draw: walkers
        {
            for ( int j = 0; j < DEPTH_MAX; j++ )                               // Draw: shadows
            {
                if ( debug )
                    SDL_RenderDrawCircle ( renderer, walker[i].origin.x, walker[i].origin.y, SENSE_BUBBLE );

                SDL_RenderDrawPoint  ( renderer, walker_steps[i][j][0], walker_steps[i][j][1] );

                SDL_SetRenderDrawColor (    // foreground color
                    renderer,               // rendering context
                    colors[j][0],           // red value
                    colors[j][1],           // green value
                    colors[j][2],           // blue value
                    SDL_ALPHA_OPAQUE        // alpha value
                );
            }
            
            for ( int j = i + 1; j < WALKER_MAX; j++ )                          // Draw: sense bubble if triggered
                if ( walker[i].isInsideCircle ( POINT { walker[j].origin.x, walker[j].origin.y } ) )
                {
                    if ( debug )
                    {
                        SDL_RenderFillCircle ( renderer, walker[i].origin.x, walker[i].origin.y, SENSE_BUBBLE );
                        SDL_RenderFillCircle ( renderer, walker[j].origin.x, walker[j].origin.y, SENSE_BUBBLE );
                    }
                    
                    SDL_RenderDrawLine ( renderer, walker[i].origin.x, walker[i].origin.y, walker[j].origin.x, walker[j].origin.y );
                }
        }
        
        SDL_RenderPresent ( renderer );                                         // Update: renderer... polls for ~500 ms
        
        SDL_Delay ( 50 );
        
        for ( i = 0; i < WALKER_MAX; i++ )                                      // Array shift all 'walker_steps'
            array_shift ( walker_steps[i], DEPTH_MAX, 2, false, 1 );
        
        for ( i = 0; i < WALKER_MAX; i++ )                                      // Init: next 'walker' step
            walker[i].next_step ( generate_random ( 0, 7 ) );

        while ( SDL_PollEvent ( &sdl_event )  )
        {
            if ( sdl_event.type == SDL_QUIT )
                run_loop = false;
            else
                break;
        }
    }
}
