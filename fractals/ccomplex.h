#ifndef __CCOMPLEX_H__
#define __CCOMPLEX_H__

struct Ccomplex
{
    Ccomplex(double, double);
    Ccomplex();
    Ccomplex(const Ccomplex &);

    //Ccomplex operator =(const Ccomplex);
	double re;
	double im;
};

Ccomplex Cexp(const Ccomplex &);
Ccomplex Csin(const Ccomplex &);
Ccomplex Ccos(const Ccomplex &);
Ccomplex Ctan(const Ccomplex &);

Ccomplex operator -(const Ccomplex);
Ccomplex operator -(const Ccomplex &, const Ccomplex &);
Ccomplex operator +(const Ccomplex &, const Ccomplex &);
Ccomplex operator *(const Ccomplex &, const Ccomplex &);
Ccomplex operator /(const Ccomplex &, const Ccomplex &);

#endif
