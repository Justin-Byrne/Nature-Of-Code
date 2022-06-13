// @File: 		main.cpp
// @Project: 	Entity Rotation 3.0
// @Author:		Justin Byrne
// @Date:		Created 6/12/22 at 9:20 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <algorithm>

#include <string>       // TODO: DELETE THIS WHEN DONE !!!

#include "include/structs.hpp"
#include "include/helpers.hpp"
#include "include/colors.hpp"

#define DEBUG         0

#define WINDOW_TITLE  "Entity Rotation 3.0"
#define WINDOW_WIDTH  100
#define WINDOW_HEIGHT 100
#define DEPTH_MAX      10
#define SENSE_BUBBLE   50

#pragma mark - GLOBAL VARIABLE DECLARATIONS

SDL_Window   * window   = NULL;
SDL_Renderer * renderer = NULL;

bool run_loop = true;

RGB colors[DEPTH_MAX] = { { 0, 0, 0 } };

#pragma mark - GLOBAL FUNCTION DECLARATIONS

int  setup_window           ( const char * title, int x_pos, int y_pos, int width, int height );
void set_render_draw_color  ( RGB color = { 0, 0, 0 } );
void set_render_draw_colors ( RGB background = { 255, 255, 255 }, RGB foreground = { 0, 0, 0 } );
void generate_colors        ( int start, int end );
void exit                   ( const char * message );
void draw                   ( );

#pragma mark - DATA STRUCTURES

enum STATE
{
    SILENT,     // 0
    ROTATE,     // 1
    MOVING,     // 2
};

struct COUNT
{
    int lower_right = 0;
    int lower_left  = 0;
    int upper_left  = 0;
    int upper_right = 0;
};

struct WINDOW_QUADRANTS
{
    COUNT counts;
    
    int center_horizontal = WINDOW_WIDTH / 2;
    int center_vertical   = WINDOW_HEIGHT / 2;
    
    void check_quandrant ( int x, int y )
    {
        // KEY
        // lower_left  = false, true
        // lower_right = false, false
        // upper_left  = true,  true
        // upper_right = true,  false

        bool top  = false;
        bool left = false;
        
        if ( x < center_horizontal ) left = true;
        if ( y < center_vertical   ) top  = true;
        
        if ( top == false && left == true  ) counts.lower_left  += 1;
        if ( top == false && left == false ) counts.lower_right += 1;
        if ( top == true  && left == true  ) counts.upper_left  += 1;
        if ( top == true  && left == false ) counts.upper_right += 1;
    }
};

struct WALKER
{
    COORDINATE origin = { 0, 0 };
    
    WINDOW_QUADRANTS quadrants;
    
    DEGREE degree;
    
    int steps[DEPTH_MAX];
    int state  = SILENT;
    int radius = 0;
    int walk   = 0;
    
    // Constructors ......................................................... //
    
    WALKER ( COORDINATE origin )
    {
        this->origin = origin;
    }
    
    WALKER ( COORDINATE origin, int radius )
    {
        this->origin = origin;
        this->radius = radius;
    }
    
    // Constructors (Generic) ... //
    
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
    
    // > .. Validators .............. //
    
    void check_boundary ( )
    {
        if ( this->origin.x <= 0 )              this->origin.x++;               // right
        if ( this->origin.y <= 0 )              this->origin.y++;               // top
        if ( this->origin.x >= WINDOW_WIDTH  )  this->origin.x--;               // left
        if ( this->origin.y >= WINDOW_HEIGHT )  this->origin.y--;               // bottom
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
        
        this->steps[0] = this->degree.b;
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

/// Generates a series of gray colors
/// @param      highest_color                 Highest number, preferable white... RGB values
void generate_colors ( int highest_color )
{
    highest_color  = std::clamp ( highest_color, 0, 255 );
    
    int difference = highest_color / DEPTH_MAX;
    
    for ( i = 0; i < DEPTH_MAX; i++ )
            colors[i] = { ( i * difference ), ( i * difference ), ( i * difference ) };
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
    
    walker = { COORDINATE { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 } };

    generate_colors ( 255 );

    /* - - - - - - - - - - - - - - - - - Init - - - - - - - - - - - - - - - - - */
    
    walker.degree = { generate_random(0, 360), generate_random(0, 360) };
//    walker.cache_steps ( );
    
    while ( run_loop )                                                          // DRAW
    {
        #if DEBUG
        std::string OUTPUT = std::string() + "\n[OUTPUT]\n" + "walker.degree.a: \t\t\t%d\n" + "walker.degree.b: \t\t\t%d\n" + "walker.degree.distance: \t%d\n" + "walker.degree.clockwise: \t%s\n" + "walker.degree.step: \t\t%d\n";
        printf ( OUTPUT.c_str ( ), walker.degree.a, walker.degree.b, walker.degree.distance, ( walker.degree.clockwise ) ? "true" : "false", walker.degree.step );
        #endif
        
        set_render_draw_colors ( );
        
        SDL_RenderDrawPoint ( renderer, walker.origin.x, walker.origin.y );     // Draw: entity dot

        int random_number = generate_random ( 1, 30 );
        
        COORDINATE rotate_destination = walker.rotate ( walker.degree.b, random_number );      // Create: pivot point & rotate for ending degree
        
        SDL_RenderDrawLine ( renderer, walker.origin.x, walker.origin.y, rotate_destination.x, rotate_destination.y );    // Draw: destination sightline
        
        #if DEBUG
        walker.quadrants.check_quandrant ( rotate_destination.x, rotate_destination.y );
        std::string QUANDRANTS = std::string() + "\nQuandrant Counts:\n\n" + "lower_right: \t%d\n" + "lower_left: \t%d\n" + "> upper_left: \t%d\n" + "upper_right: \t%d\n";
        printf ( QUANDRANTS.c_str ( ), walker.quadrants.counts.lower_right, walker.quadrants.counts.lower_left, walker.quadrants.counts.upper_left, walker.quadrants.counts.upper_right );
        #endif
        
//        for ( int i = 0; i < DEPTH_MAX; i++ )
//        {
//            set_render_draw_color ( colors[i] );
//
//            COORDINATE step_rotation = walker.rotate ( walker.steps[i], 2 );
//
//            SDL_RenderDrawLine ( renderer, walker.origin.x, walker.origin.y, step_rotation.x, step_rotation.y );
//        }
        
        SDL_RenderPresent ( renderer );                                         // Update: renderer... polls for ~500 ms

//        SDL_Delay ( 100 );
        
        walker.degree = { walker.degree.b, generate_random ( 0, 360 ) };
//        walker.cache_steps ( );
        
        while ( SDL_PollEvent ( &sdl_event )  )
        {
            if ( sdl_event.type == SDL_QUIT )
                run_loop = false;
            else
                break;
        }
    }
}
