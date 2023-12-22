#pragma once
#include "bitmap.h"
#include "matrix.h"

void NoResize();
void DeleteCur();
void SetSize(int x, int y, int w, int h);

void rotate_pyramid(bitmap&, char, char);
void move_pyramid(bitmap&, char, char);
void zoom_pyramid(bitmap&, char, char o = '\0');

void rotate_cube(bitmap&, char, char);
void move_cube(bitmap&, char, char);
void zoom_cube(bitmap&, char, char o = '\0');

void init();
