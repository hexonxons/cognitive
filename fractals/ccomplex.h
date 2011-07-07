#ifndef __CCOMPLEX_H__
#define __CCOMPLEX_H__

typedef struct 
{
	double re;
	double im;
} Ccomplex;

Ccomplex Cexp(const Ccomplex c);
Ccomplex Csin(const Ccomplex c);
Ccomplex Ccos(const Ccomplex c);
Ccomplex Ctan(const Ccomplex c);
Ccomplex Uminus(const Ccomplex c);
Ccomplex Usubstruction(const Ccomplex a, const Ccomplex b);
Ccomplex Udevision(const Ccomplex a, const Ccomplex b);
Ccomplex Umultiplication(const Ccomplex a, const Ccomplex b);

#endif
