//
//  main.cpp
//  Entity Rotation
//
//  Created by Justin Byrne on 5/29/22.
//

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "include/colors.hpp"
#include "include/helpers.hpp"
#include "include/structs.hpp"

#define WINDOW_WIDTH  100
#define WINDOW_HEIGHT 100
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

int colors[DEPTH_MAX][3]       = { { 0 } };
int walker_steps[DEPTH_MAX][2] = { { 0 } };

#pragma mark - GLOBAL FUNCTION DECLARATIONS

int setup_window ( const char* title, int x_pos, int y_pos, int width, int height );
void generate_colors ( int start, int end );
void exit ( const char * message );
void draw ( );

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

struct WALKER
{
    POINT origin     = { 0, 0 };
    POINT point      = { 0, 0 };

    ROTATION rotation;
    
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
    
    int isClockwise ( int distance )
    {
//        if (degree == 0) std::cout << "Zero"; else (num > 0) ? std::cout << "Positive": std::cout << "Negative";
        
        return ( distance == 0 ) ? 0 : ( distance > 0 ) ? 1 : -1;
    }
    
    int get_rotation_distance ( int origin, int destination )
    {
        int result = destination % 360 - origin % 360;       // Use a modulo to handle angles that go around the circle multiple times.

        if ( result >   180 ) result -= 360;
        if ( result <= -180 ) result += 360;
        
    //    result = ( result <= -180 ) ? 360 : -360;
        
        return result;
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

/// Sets background and foreground of the renderer
/// @param      background              RGB values for background
/// @param      foreground              RGB values for foreground
void set_render_draw_colors ( RGB background = { 255, 255, 255 }, RGB foreground = { 0, 0, 0 } )
{
    SDL_SetRenderDrawColor (    // background color
        renderer,               // rendering context
        background.red,         // red value
        background.green,       // green value
        background.blue,        // blue value
        SDL_ALPHA_OPAQUE        // alpha value
    );

    SDL_RenderClear ( renderer );
    
    SDL_SetRenderDrawColor (    // background color
        renderer,               // rendering context
        foreground.red,         // red value
        foreground.green,       // green value
        foreground.blue,        // blue value
        SDL_ALPHA_OPAQUE        // alpha value
    );
}

/// Generates a series of gray colors
/// @param      start               Lowest number; preferable black... RGB values
/// @param      end                 Highest number, preferable white... RGB values
void generate_colors ( int start, int end )
{
    if ( DEPTH_MAX > 255 || end > 255 )
        exit ( "Maximum values for 'DEPTH_MAX' and 'end' variables are 255!" );
    
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

    WALKER walker;
    
    walker = { POINT { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 } };

    generate_colors ( 0, 255 );

    /* - - - - - - - - - - - - - - - - - Init - - - - - - - - - - - - - - - - - */
    
    int degree = 0;
    
//    walker.rotation.set ( degree, generate_random ( 0, 360 ) );
    
    walker.rotation.set ( degree, 90 );
    
    while ( run_loop )                                                          // DRAW
    {
        set_render_draw_colors ( );
        
        SDL_RenderDrawPoint ( renderer, walker.origin.x, walker.origin.y );                     // Draw: entity dot

        POINT point = walker.rotate ( degree );                                                 // Create: pivote point & rotate per degree

        SDL_RenderDrawLine ( renderer, walker.origin.x, walker.origin.y, point.x, point.y );    // Draw: sightline with acquired orientation

        SDL_RenderPresent ( renderer );                                                         // Update: renderer... polls for ~500 ms

        SDL_Delay ( 100 );

        degree++;

        degree = ( degree > 359 ) ? 0 : degree;
        
        printf ( "\n] > Degree\n > walker.rotation.origin: %d\n > walker.rotation.destination: %d\n > degree: %d\n", walker.rotation.origin, walker.rotation.destination, degree );

        if ( degree == walker.rotation.destination )
            walker.rotation.set( walker.rotation.destination, generate_random ( 0, 360 ) );
        
        while ( SDL_PollEvent ( &sdl_event )  )
        {
            if ( sdl_event.type == SDL_QUIT )
                run_loop = false;
            else
                break;
        }
    }
}
