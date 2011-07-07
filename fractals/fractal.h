#ifndef __FRACTAL_H__
#define __FRACTAL_H__

#include "ccomplex.h"
#include <windows.h>

void mandelbrot(int x, int y, HDC hdc);
void julia(int x, int y, HDC hdc);
void fractal1(int x, int y, HDC hdc);

#endif