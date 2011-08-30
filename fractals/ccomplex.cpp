#include "ccomplex.h"
#include <math.h>

Ccomplex::Ccomplex(double real, double imaginary )
{
    re = real;
    im = imaginary;
}

Ccomplex::Ccomplex()
{
}

Ccomplex::Ccomplex(const Ccomplex &complex)
{
    re = complex.re;
    im = complex.im;
}

Ccomplex Cexp(const Ccomplex &c)
{
	double e = exp(c.re);
	return Ccomplex(cos(c.im) * e, sin(c.im) * e);
}

Ccomplex Csin(const Ccomplex &c)
{
	Ccomplex imgDiff = Cexp(c) - Cexp(-c);
	return Ccomplex(0, imgDiff.im / 2);
}

Ccomplex Ccos(const Ccomplex &c)
{
	Ccomplex relDiff = Cexp(c) - Cexp(-c);
	return Ccomplex(relDiff.re / 2, 0);
}

Ccomplex Ctan(const Ccomplex &c)
{
	return Csin(c) / Ccos(c);
}

Ccomplex operator -(const Ccomplex num)
{
    return Ccomplex(-num.re, -num.im);
}

/*
Ccomplex::Ccomplex operator =(const Ccomplex num)
{
    return num;
}*/



Ccomplex operator -(const Ccomplex &left, const Ccomplex &right)
{
    return Ccomplex(left.re - right.re, left.im - right.im);
}

Ccomplex operator +(const Ccomplex &left, const Ccomplex &right)
{
    return Ccomplex(left.re + right.re, left.im + right.im);;
}

Ccomplex operator *(const Ccomplex &left, const Ccomplex &right)
{
    return Ccomplex(left.re * right.re - left.im * right.im, left.re * right.im + left.im * right.re);
}

Ccomplex operator /(const Ccomplex &left, const Ccomplex &right)
{
    double numer1 = (left.re * right.re) + (left.im * right.im);
    double numer2 = left.im * right.re - left.re * right.im;
    return Ccomplex((left.re * right.re) + (left.im * right.im) / (right.re * right.re + right.im * right.im), 
                    numer2 / (right.re * right.re + right.im * right.im));
}