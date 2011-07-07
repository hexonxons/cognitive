#include "fractal.h"

void mandelbrot(int x, int y, HDC hdc)
{
	// max
	double M = 100;
	// num of iterations
	int iter = 60;
	// constant
	Ccomplex C;
	Ccomplex Z;
	Ccomplex temp;
	int n = 0;

	Z.re = 0;
	Z.im = 0;
	C.re = x * 0.005;
	C.im = y * 0.005;
	n = 0;

	while (Z.im * Z.im + Z.re * Z.re < M * M && n < iter)
	{
		temp = Z;
		Z.re = temp.re * temp.re - temp.im * temp.im + C.re;
		Z.im = 2 * temp.re * temp.im + C.im;
		++n;
	}
	if (n < iter)
		// color of outside region
		SetPixel(hdc, 512 + x, 384 + y, 0xDBEFFF);
	else
		// color of set
		SetPixel(hdc, 512 + x, 384 + y, 0x000000);

}

void julia(int x, int y, HDC hdc)
{
	// max
	double M = 100;
	// num of iterations
	int iter = 20;
	// constant
	Ccomplex C;
	Ccomplex Z;
	Ccomplex temp;
	int n = 0;

	Z.re = x * 0.005;
	Z.im = y * 0.005;
	C.re = 0.1103100;
	C.im = -0.6703700; 
	n = 0;

	while (Z.im * Z.im + Z.re * Z.re < M * M && n < iter)
	{
		temp = Z;
		Z.re = temp.re * temp.re - temp.im * temp.im + C.re;
		Z.im = 2 * temp.re * temp.im + C.im;
		++n;
	}
	if (n < iter)
		// color of outside region
		SetPixel(hdc, 512 + x, 384 + y, 0xDBEFFF);
	else
		// color of set
		SetPixel(hdc, 512 + x, 384 + y, 0x000000);

}

void fractal1(int x, int y, HDC hdc)
{
	// max
	double M = 100;
	// num of iterations
	int iter = 60;
	// constant
	Ccomplex C;
	Ccomplex Z;
	int n = 0;

	Z.re = x * 0.002;
	Z.im = y * 0.002;
	C.re = x * 0.0015;
	C.im = y * 0.0015; 
	n = 0;

	while (Z.im * Z.im + Z.re * Z.re < M * M && n < iter)
	{
		Z = Umultiplication(Udevision(C, Ccos(Z)), Udevision(C, Ccos(Z)));
		++n;
	}
	if (n < iter)
		// color of outside region
		SetPixel(hdc, 512 + x, 384 + y, RGB(n * 6, n * 20, 0));
	else
		// color of set
		SetPixel(hdc, 512 + x, 384 + y, 0x444444);
}