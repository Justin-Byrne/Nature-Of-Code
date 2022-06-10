// @File: 		colors.hpp
// @Project: 	Walker 4.0
// @Author:		Justin Byrne
// @Date:		Created 6/10/22 at 8:36 AM
// @Copyright:	Copyright (c) 2022 Byrne-Systems

#ifndef colors_hpp
#define colors_hpp

#define FREQUENCY     0.1
#define AMPLITUDE 255 / 2
#define CENTER    255 / 2

#define RANGE_OLD_MIN  -1
#define RANGE_OLD_MAX   1
#define RANGE_NEW_MIN   0
#define RANGE_NEW_MAX 255

#define PHASE_1 0
#define PHASE_2 2
#define PHASE_3 4

extern double range_old;
extern double range_new;

extern int i;
extern int calls;

int get_color_value ( int phase );

int * get_color_values ( );

int * get_gray_values ( );

#endif /* colors_hpp */
