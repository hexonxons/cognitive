#include "ccomplex.h"
#include <math.h>

Ccomplex Uminus(const Ccomplex c)
{
	Ccomplex result;
	result.re = -c.re;
	result.im = -c.im;
	return result;
}

Ccomplex Usubstruction(const Ccomplex a, const Ccomplex b)
{
	Ccomplex result;
	result.re = a.re - b.re;
	result.im = a.im - b.im;
	return result;
}

Ccomplex Udevision(const Ccomplex a, const Ccomplex b)
{
	Ccomplex result;
	double numer1 = (a.re * b.re) + (a.im * b.im);
	double rel = numer1 / (b.re * b.re + b.im * b.im);
	double numer2 = a.im * b.re - a.re * b.im;
	double img = numer2 / (b.re * b.re + b.im * b.im); 
	result.re = rel;
	result.im = img;
	return result;
};

Ccomplex Umultiplication(const Ccomplex a, const Ccomplex b)
{
	Ccomplex result;
	double rel = a.re * b.re - a.im * b.im;
	double img = a.re * b.im + a.im * b.re;
	result.re = rel;
	result.im = img;
	return result;
};

Ccomplex Cexp(const Ccomplex c)
{
	Ccomplex result;
	double e = exp(c.re);
	double rel = cos(c.im) * e;
	double img = sin(c.im) * e;
	result.re = rel;
	result.im = img;
	return result;
};

Ccomplex Csin(const Ccomplex c)
{
	Ccomplex result;
	Ccomplex E1 = Cexp(c);
	Ccomplex E2 = Cexp(Uminus(c));
	Ccomplex imgDiff = Usubstruction(E1, E2);
	double img = imgDiff.im / 2;
	result.re = 0;
	result.im = img;
	return result;
};

Ccomplex Ccos(const Ccomplex c)
{
	Ccomplex result;
	Ccomplex E1 = Cexp(c);
	Ccomplex E2 = Cexp(Uminus(c));
	Ccomplex relDiff = Usubstruction(E1, E2);
	double rel = relDiff.re / 2;
	result.re = rel;
	result.im = 0;

	return result;
};

Ccomplex Ctan(const Ccomplex c)
{
	Ccomplex newSin = Csin(c);
	Ccomplex newCos = Ccos(c);
	Ccomplex result = Udevision(newSin, newCos);
	return result;
};
