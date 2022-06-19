// @File: 		main.cpp
// @Project: 	Walker 5.0
// @Author:		Justin Byrne
// @Date:		Created 6/16/22 at 8:03 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string>       // TODO: Omit this !
#include <algorithm>
#include <random>

#include "include/structs.hpp"
#include "include/helpers.hpp"

// DEBUG
#define DEBUG_ROTATE             0
#define DEBUG_BODY               1
#define DEBUG_SENSE              0
#define DEBUG_SIGHTLINE          1
#define DEBUG_TRAILS             0

#define DEBUG_ATTRIBUTES         0

// PROGRAM
#define WINDOW_TITLE  "Walker 5.0"
#define WINDOW_WIDTH           800
#define WINDOW_HEIGHT          800
#define WALKER_MAX             100
#define DEPTH_MAX               25
#define REGION_BODY             10
#define REGION_SENSE            70
#define LINE_SEGMENTS            6

// ATTRIBUTES
#define MAX_LEVEL              100
#define MIN_LEVEL               10

#pragma mark - GLOBAL VARIABLE DECLARATIONS

SDL_Window   * window   = NULL;
SDL_Renderer * renderer = NULL;

bool run_loop = true;

RGB step_colors[DEPTH_MAX] = { { 0, 0, 0 } };

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

struct COLORS
{
    RGB white          = { 255, 255, 255 };
    RGB yellow_sun     = { 237, 235,   0 };
    RGB green_apple    = { 146, 195,   0 };
    RGB blue_ice       = {  28,  50, 251 };
    RGB blue_bright    = {   0, 248, 250 };
    RGB gray_brilliant = {  84,  84,  84 };
    RGB black          = {   0,   0,   0 };
};

COLORS colors;

struct ATTRIBUTES
{
    int vitality;
    
    int health;
    int walk_speed;
    int walk_distance;
    int stamina;
    int stamina_refactor;
    
    // Constructors ......................................................... //
    
    ATTRIBUTES ( int vitality )
    {
        // SET MASTER ATTRIBUTE
        vitality = std::clamp ( vitality, MIN_LEVEL, MAX_LEVEL );
        
        this->vitality = generate_random ( MIN_LEVEL, vitality );
        
        // DISTRIBUTE TO SLAVE ATTRIBUTES
        this->health           = generate_random ( MIN_LEVEL, this->vitality );
        this->walk_speed       = generate_random ( MIN_LEVEL, this->vitality );
        this->walk_distance    = generate_random ( MIN_LEVEL, this->vitality );
        this->stamina          = generate_random ( MIN_LEVEL, this->vitality );
        this->stamina_refactor = generate_random ( MIN_LEVEL, this->vitality );
        
        normal_distribution ( 0.5, 0.2, 1000 );
    }
    
    // Constructors (Generic) ....... //
    
    ATTRIBUTES  ( ) { };
    
    ~ATTRIBUTES ( ) { };
    
    // Functions ............................................................ //
    
    void normal_distribution ( double mean, double deviation, int samples )
    {
        std::random_device rd { };
        std::mt19937 gen { rd ( ) };
         
        // values near the mean are the most likely
        // standard deviation affects the dispersion of generated values from the mean
        std::normal_distribution<> d { mean, deviation };
        std::map<int, int> hist { };
            
        for ( int n = 0; n < samples; n++ )
            hist [ std::round ( d ( gen ) ) ]++;

//        for ( auto p : hist )
//            printf ( "p.first: %d\n %d\n", p.first, ( p.second / 200 ) );
    }
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
    
    ANGLE angle;
    
    ATTRIBUTES attributes = { generate_random ( 100 ) };
    
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
        
        return ANGLE().rotate ( this->origin, degree );
    }
    
    COORDINATE rotate ( int degree, int step_length )
    {
        this->state = ROTATE;
        
        return ANGLE().rotate ( this->origin, degree, step_length );
    }
    
    // > .. Iterators ............... //
    
    void next_step ( int step_size = 3 )
    {
        cache_steps ( );
        
        this->origin = ANGLE().rotate ( this->origin, this->angle.a, step_size );
        
        this->walk_distance--;
        
        check_boundary ( );
    }
    
    // > .. Validators .............. //
    
    void check_boundary ( )
    {
        this->origin.x = ( this->origin.x <= 0 ) ? 1 : this->origin.x;                              // left
        this->origin.y = ( this->origin.y <= 0 ) ? 1 : this->origin.y;                              // top
        
        this->origin.x = ( this->origin.x >= WINDOW_WIDTH  ) ? WINDOW_WIDTH  - 1 : this->origin.x;  // right
        this->origin.y = ( this->origin.y >= WINDOW_HEIGHT ) ? WINDOW_HEIGHT - 1 : this->origin.y;  // bottom
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
    
    srand ( (unsigned) time (0) );                                              // seed randomizer
    
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
/// @param      top_color                 Highest number, preferable white... RGB values
void generate_colors ( int top_color, bool invert = false )
{
    const int COLOR_MIN = 84;
    const int COLOR_MAX = 255;
    
    top_color = std::clamp ( top_color, COLOR_MIN, COLOR_MAX );
    
    int difference = ( invert ) ? ( top_color - COLOR_MIN ) / DEPTH_MAX : top_color / DEPTH_MAX;
    
    if ( invert )
        for ( int i = DEPTH_MAX - 1; i > -1; i-- )
            step_colors[i] = { top_color - ( i * difference ), top_color - ( i * difference ), top_color - ( i * difference ) };
    else
        for ( int i = 0; i < DEPTH_MAX; i++ )
            step_colors[i] = { ( ( i + 1 ) * difference ), ( ( i + 1 ) * difference ), ( ( i + 1 ) * difference ) };
}

#pragma mark - DRAW

/// Initiate poll events
void draw ( )
{
    /* - - - - - - - - - - - - - - - - - Init - - - - - - - - - - - - - - - - - */
    
    SDL_Event sdl_event;

    WALKER walker[WALKER_MAX];

    generate_colors ( 255, true );
    
    int padding = 50;
    
    for ( int i = 0; i < WALKER_MAX; i++ )
    {
        walker[i]       = { COORDINATE { (double) generate_random ( padding, WINDOW_WIDTH - padding ), (double) generate_random ( padding, WINDOW_HEIGHT - padding ) }, REGION_SENSE };
        walker[i].angle = { generate_random ( 0, 360 ), generate_random ( 0, 360 ) };
    }

    COORDINATE rotate_origin, rotate_destination;
    
    /* - - - - - - - - - - - - - - - - - Init - - - - - - - - - - - - - - - - - */
    
    while ( run_loop )                                                          // DRAW
    {
        set_render_draw_colors ( renderer, colors.gray_brilliant, colors.white );
        
        for ( int i = 0; i < WALKER_MAX; i++ )
        {
            SDL_RenderDrawPoint (renderer, walker[i].origin, colors.white );
            
            #if DEBUG_BODY
            SDL_RenderDrawCircle ( renderer, walker[i].origin, REGION_BODY, colors.blue_ice );
            #endif
            
            #if DEBUG_SENSE
            SDL_RenderDrawCircle ( renderer, walker[i].origin, REGION_SENSE, colors.blue_bright );
            #endif
            
            switch ( walker[i].state )
            {
                case SILENT:
                case ROTATE:
                    
                    #if DEBUG_ROTATE
                    rotate_origin      = walker[i].rotate ( walker[i].angle.a, REGION_BODY );
                    SDL_RenderDrawLine ( renderer, walker[i].origin, rotate_origin, colors.yellow_sun );

                    rotate_destination = walker[i].rotate ( walker[i].angle.b, REGION_BODY );
                    SDL_RenderDrawLine ( renderer, walker[i].origin, rotate_destination, colors.black );
                    #endif
                    
                    ( walker[i].angle.clockwise ) ? walker[i].angle.advance ( ) : walker[i].angle.regress ( );
                    
                    if ( walker[i].angle.distance == 0 )
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
                        walker[i].angle = { walker[i].angle.b, generate_random ( 0, 360 ) };
                    }
                    
                    break;
            }
            
            #if DEBUG_SIGHTLINE
            for ( int j = i + 1; j < WALKER_MAX; j++ )
                if ( walker[i].is_inside_region ( COORDINATE { walker[j].origin.x, walker[j].origin.y } ) )
                    SDL_RenderDottedLine ( renderer, walker[i].origin, walker[j].origin, LINE_SEGMENTS, colors.yellow_sun );
            #endif
            
            #if DEBUG_TRAILS
            for ( int j = 0; j < DEPTH_MAX; j++ )
                SDL_RenderDrawPoint ( renderer, walker[i].steps[j], step_colors[j] );
            #endif
        }
        
        SDL_RenderPresent ( renderer );                                         // Update: renderer... polls for ~500 ms
        
        SDL_Delay ( 50 );

        #if DEBUG_ATTRIBUTES
        for ( int i = 0; i < WALKER_MAX; i++ )
        {
            std::string OUTPUT = std::string() + "[ OUTPUT ]\n\n" + "\twalker[%d].id: \t\t\t\t >> %d <<\n" + "\tattributes.vitality: \t\t\t%d\n" + "\tattributes.health: \t\t\t\t%d\n" + "\tattributes.walk_speed: \t\t\t%d\n" + "\tattributes.walk_distance: \t\t%d\n" + "\tattributes.stamina: \t\t\t%d\n" + "\tattributes.stamina_refactor: \t%d\n\n";
            printf ( OUTPUT.c_str ( ), i, walker[i].id, walker[i].attributes.vitality, walker[i].attributes.health, walker[i].attributes.walk_speed, walker[i].attributes.walk_distance, walker[i].attributes.stamina, walker[i].attributes.stamina_refactor );
        }
        #endif
        
        while ( SDL_PollEvent ( &sdl_event )  )
        {
            if ( sdl_event.type == SDL_QUIT )
                run_loop = false;
            else
                break;
        }
    }
}
