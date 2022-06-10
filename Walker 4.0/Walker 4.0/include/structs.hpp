// @File: 		structs.hpp
// @Project: 	Walker 4.0
// @Author:		Justin Byrne
// @Date:		Created 6/10/22 at 8:37 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#ifndef structs_hpp
#define structs_hpp

#include <algorithm>
#include "helpers.hpp"

#define PI 3.141592653589
#define SIN(x) sin(x * PI / 180)
#define COS(x) cos(x * PI / 180)

int get_step_divisor ( int degree_distance )
{
    int result = 0;
    
    switch ( degree_distance )
    {
        case   5: case   6: case   7: case   8: case   9: case  10:                                                                                                      result =  2;  break;
        case  11: case  12: case  13: case  14: case  15: case  16:                                                                                                      result =  3;  break;
        case  17: case  18: case  19: case  20: case  21: case  22: case  23: case  24:                                                                                  result =  4;  break;
        case  25: case  26: case  27: case  28: case  29: case  30: case  31: case  32: case  33: case  34: case  35: case  36: case  37: case  38: case  39: case  40:  result =  5;  break;
        case  41: case  42: case  43: case  44: case  45: case  46: case  47: case  48: case  49: case  50: case  51: case  52: case  53: case  54:                      result =  6;  break;
        case  55: case  56: case  57: case  58: case  59: case  60: case  61: case  62: case  63: case  64: case  65: case  66: case  67: case  68: case  69: case  70:  result =  7;  break;
        case  71: case  72: case  73: case  74: case  75: case  76: case  77: case  78: case  79: case  80: case  81: case  82: case  83: case  84:                      result =  8;  break;
        case  85: case  86: case  87: case  88: case  89: case  90: case  91: case  92: case  93: case  94: case  95: case  96: case  97: case  98: case  99: case 100:  result =  9;  break;
        case 101: case 102: case 103: case 104: case 105: case 106: case 107: case 108: case 109: case 110: case 111: case 112: case 113: case 114:                      result = 10;  break;
        case 115: case 116: case 117: case 118: case 119: case 120: case 121: case 122: case 123: case 124: case 125: case 126: case 127: case 128: case 129: case 130:  result = 11;  break;
        case 131: case 132: case 133: case 134: case 135: case 136: case 137: case 138: case 139: case 140: case 141: case 142: case 143: case 144:                      result = 12;  break;
        case 145: case 146: case 147: case 148: case 149: case 150: case 151: case 152: case 153: case 154: case 155: case 156: case 157: case 158: case 159: case 160:  result = 13;  break;
        case 161: case 162: case 163: case 164: case 165: case 166: case 167: case 168: case 169: case 170: case 171: case 172: case 173: case 174:                      result = 14;  break;
        case 175: case 176: case 177: case 178: case 179: case 180: case 181: case 182: case 183: case 184: case 185: case 186: case 187: case 188: case 189: case 190:  result = 15;  break;
        case 191: case 192: case 193: case 194: case 195: case 196: case 197: case 198: case 199: case 200: case 201: case 202: case 203: case 204:                      result = 16;  break;
        case 205: case 206: case 207: case 208: case 209: case 210: case 211: case 212: case 213: case 214: case 215: case 216: case 217: case 218: case 219: case 220:  result = 17;  break;
        case 221: case 222: case 223: case 224: case 225: case 226: case 227: case 228: case 229: case 230: case 231: case 232: case 233: case 234:                      result = 18;  break;
        case 235: case 236: case 237: case 238: case 239: case 240: case 241: case 242: case 243: case 244: case 245: case 246: case 247: case 248: case 249: case 250:  result = 19;  break;
        case 251: case 252: case 253: case 254: case 255: case 256: case 257: case 258: case 259: case 260: case 261: case 262: case 263: case 264:                      result = 20;  break;
        case 265: case 266: case 267: case 268: case 269: case 270: case 271: case 272: case 273: case 274: case 275: case 276: case 277: case 278: case 279: case 280:  result = 21;  break;
        case 281: case 282: case 283: case 284: case 285: case 286: case 287: case 288: case 289: case 290: case 291: case 292: case 293: case 294:                      result = 22;  break;
        case 295: case 296: case 297: case 298: case 299: case 300: case 301: case 302: case 303: case 304: case 305: case 306: case 307: case 308: case 309: case 310:  result = 23;  break;
        case 311: case 312: case 313: case 314: case 315: case 316: case 317: case 318: case 319: case 320: case 321: case 322: case 323: case 324:                      result = 24;  break;
        case 325: case 326: case 327: case 328: case 329: case 330: case 331: case 332: case 333: case 334: case 335: case 336: case 337: case 338: case 339: case 340:  result = 25;  break;
        case 341: case 342: case 343: case 344: case 345: case 346: case 347: case 348: case 349: case 350: case 351: case 352: case 353: case 354:                      result = 26;  break;
        case 355: case 356: case 357: case 358: case 359: case 360:                                                                                                      result = 27;  break;
        default:                                                                                                                                                         result = 1;  break;
    }
    
    return result;
}

struct COORDINATE
{
    int x, y;
    
    COORDINATE ( int x, int y )
    {
        this->x = x;
        this->y = y;
    }
    
    // Constructors (Generic) ... //
    
    COORDINATE ( ) { }
    
    ~COORDINATE ( ) { }
};

struct DEGREE
{
    int MIN = 0;
    int MAX = 360;
    
    int a, b, distance, step, step_devisor;
    
    bool clockwise;
    
    DEGREE ( int degree )
    {
        this->a = std::clamp ( degree, MIN, MAX );
    }
    
    DEGREE ( int A, int B )
    {
        this->a = std::clamp ( A, MIN, MAX );
        this->b = std::clamp ( B, MIN, MAX );
        
        set_distance_n_direction ( );
    }
    
    // Constructors (Generic) ... //
    
    DEGREE ( ) { }
    
    ~DEGREE ( ) { }
    
    // Functions ............................................................ //
    
    void advance ( int step = 1 )
    {
        this->a = this->a + step;
        
        this->a = ( this->a == 360 ) ? 0 : this->a;
    }
    
    void regress ( int step = 1 )
    {
        this->a = this->a - step;
        
        this->a = ( this->a == 0 ) ? 360 : this->a;
    }
    
    double convert_to_radian ( int degree )
    {
        return ( degree * PI / 180 );
    }

    int convert_to_degree ( float radian )
    {
        return ( radian * 180 ) / PI;
    }
  
    COORDINATE rotate ( COORDINATE &origin, int &degree, int step = 0 )
    {
        COORDINATE coordinate = { origin.x + step, origin.y };

        double radians        = convert_to_radian ( degree );

        double sine           = sin ( radians );
        double cosine         = cos ( radians );

        coordinate.x         -= origin.x;                                       // translate point back to origin
        coordinate.y         -= origin.y;

        double x_new          = coordinate.x * cosine - coordinate.y * sine;    // rotate point
        double y_new          = coordinate.x * sine   - coordinate.y * cosine;

        coordinate.x          = x_new + origin.x;                               // translate point back
        coordinate.y          = y_new + origin.y;

        return coordinate;
    }
    
    void set_distance_n_direction ( )
    {
        int distance  = this->b % MAX - this->a % MAX;
        
            distance += ( distance > 180 )
                            ? ( - MAX )
                            : ( distance <= -180 )
                                ? MAX
                                : distance;
        
        this->distance  = distance;
        
        this->clockwise = ( distance == MIN )
                            ? generate_random ( MIN, MAX )
                            : ( distance > MIN )
                                ? true
                                : false;
    }
};

struct RGB
{
    int red, green, blue;
    
    RGB ( int red, int green, int blue )
    {
        this->red   = std::clamp ( red,   0, 255 );
        this->green = std::clamp ( green, 0, 255 );
        this->blue  = std::clamp ( blue,  0, 255 );
    }
    
    // Constructors (Generic) ... //
    
    RGB ( ) { }
    
    ~RGB ( ) { }
};

#endif /* structs_hpp */
