//
//  main.cpp
//  Perlin Noise
//
//  Created by Justin Byrne on 5/10/22.
//

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

#pragma mark - GLOBAL VARIABLE DECLARATIONS

SDL_Window   * window   = NULL;
SDL_Renderer * renderer = NULL;

bool run_loop = true;

int p[512] =
{
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,
    10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,
    56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,
    122,60,211,133,230,220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,
    76,132,187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,
    226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,129,22,39,253,19,
    98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,
    179,162,241, 81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,
    121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

// int permutation[] =

#pragma mark - GLOBAL FUNCTION DECLARATIONS

int setup_window ( const char * title, int x_pos, int y_pos, int width, int height );
void exit ( );
void main_loop ( );

#pragma mark - MAIN

double fade ( double t )
{
    return t * t * t * ( t * ( t * 6 - 15 ) + 10 );
}

double lerp ( double t, double a, double b )
{
    return a + t * ( b - a );
}

double grad ( int hash, double x, double y, double z )
{
    int h = hash & 15;                  // CONVERT LO 4 BITS OF HASH CODE
    
    double u = h < 8 ? x : y,           // INTO 12 GRADIENT DIRECTIONS.
           v = h < 4 ? y : h == 12 || h == 14 ? x : z;
  
    return ( ( h&1 ) == 0 ? u : -u ) + ( ( h&2 ) == 0 ? v : -v );
}

double noise ( double x, double y, double z )
{
    int X = (int) floor(x) & 255,       // FIND UNIT CUBE THAT
        Y = (int) floor(y) & 255,       // CONTAINS POINT.
        Z = (int) floor(z) & 255;

        x -= floor(x);                  // FIND RELATIVE X,Y,Z
        y -= floor(y);                  // OF POINT IN CUBE.
        z -= floor(z);
        
    double u = fade(x),                 // COMPUTE FADE CURVES
           v = fade(y),                 // FOR EACH OF X,Y,Z.
           w = fade(z);

    int A = p[X    ]+Y, AA = p[A] + Z, AB = p[A + 1] + Z,           // HASH COORDINATES OF
        B = p[X + 1]+Y, BA = p[B] + Z, BB = p[B + 1] + Z;           // THE 8 CUBE CORNERS,

    return lerp( w, lerp( v, lerp( u, grad( p[AA    ], x,     y,     z     ),       // AND ADD
                                      grad( p[BA    ], x - 1, y,     z     ) ),     // BLENDED
                             lerp( u, grad( p[AB    ], x,     y - 1, z     ),       // RESULTS
                                      grad( p[BB    ], x - 1, y - 1, z     ) ) ),   // FROM  8
                    lerp( v, lerp( u, grad( p[AA + 1], x,     y,     z - 1 ),       // CORNERS
                                      grad( p[BA + 1], x - 1, y,     z - 1 ) ),     // OF CUBE
                             lerp( u, grad( p[AB + 1], x,     y - 1, z - 1 ),
                                      grad( p[BB + 1], x - 1, y - 1, z - 1 ) ) ) );
}

int main ( int argc, char * arg[] )
{
    setup_window ( "Perlin Noise", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT );
    
    srand ( (unsigned) time (0) );      // seed randomizer
    
    main_loop ( );
    
    /* - - - - - - clean up - - - - - - */
    SDL_DestroyRenderer ( renderer );
    SDL_DestroyWindow ( window );
    SDL_Quit ( );
    /* - - - - - - clean up - - - - - - */

    return 0;
}

#pragma mark - GLOBAL FUNCTIONS

/*!
    @brief                                  Setup window
    @param          title                   Window title
    @param          x_pos                   Window's x position
    @param          y_pos                   Window's y position
    @param          width                   Window's width
    @param          height                  Window's height
 */
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
    
    SDL_SetRenderDrawColor (            // background color
        renderer,                       // rendering context
        255,                            // red value
        255,                            // green value
        255,                            // blue value
        SDL_ALPHA_OPAQUE                // alpha value
    );

    SDL_RenderClear ( renderer );

    SDL_SetRenderDrawColor (            // foreground color
        renderer,                       // rendering context
        0,                              // red value
        0,                              // green value
        0,                              // blue value
        SDL_ALPHA_OPAQUE                // alpha value
    );
    
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
    @brief                                  Initiate poll events
 */
void main_loop ( )
{
    SDL_Event sdl_event;
    
    double x, y;
    
//    int colors[3];
    
    // code ...

    while ( run_loop )
    {
        for ( x = 0; x < WINDOW_WIDTH; x++ )
        {
            for ( y = 0; y < WINDOW_HEIGHT; y++ )
            {
                double value = noise ( x, y, 0 );
                
//                int * colors = { floor ( value ) };
                
//                SDL_SetRenderDrawColor(         // foreground color
//                    renderer,                   // rendering context
//                    colors[0],                  // red value
//                    colors[1],                  // green value
//                    colors[2],                  // blue value
//                    SDL_ALPHA_OPAQUE            // alpha value
//                );
            }
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
