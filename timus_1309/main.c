#include <stdio.h>
#include <stdlib.h>

// A(i) = (j^5 - j + 7) mod M,
// B(i) = (-j^5 + j^3 + 3 * j) mod M.

int A(int i)
{
	int j = (i + 1) % 9973;
	return (((((j * j) % 9973) * ((j * j) % 9973)) % 9973) * j - j  + 7) % 9973;
}

int B(int i)
{
	int j = (i + 1) % 9973;
	// (x * x) mod M = (x mod M * x mod M) mod M
	int res = ((-1) * (((j * j) % 9973) * ((j * j) % 9973)) * j + ((((j * j) % 9973) * j)% 9973) + 3 * j ) % 9973;
	return res > 0 ? res : 9973 + res;
}

int main()
{
	int num = 0;
	int result;
	// f(x + M) = (coeffA * f(x) + coeffB) mod M
	int coeffA = A(0);
	int coeffB = B(0);
	// num = mod * M + integr
	int mod = 0;
	int integr = 0;
	// f(p * M) == coeffB * (coeffA^(p-1) + coeffA^(p-2) + .. + 1) (mod M)

	int i = 0;
	int val = 1;
	int temp = 0;
	int j = 0;

	scanf("%d", &num);
	mod = num / 9973;
	integr = num % 9973;
	
	for(i = 1; i < integr; ++i)
	{
		int prevA = coeffA;
		int prevB = coeffB;
		int a = A(i);
		int b = B(i);
		coeffA = prevA * a;
		coeffB = prevB * a + b;

		coeffA = coeffA % 9973;
		coeffB = coeffB % 9973;
	}

	for (i = 1; i < mod; ++i)
	{
		temp = coeffA;
		for(j = 0; j < i; ++j)
		{
			temp *= coeffA;
			temp = temp % 9973;
		}
		val += temp;
		val = val % 9973;
	}

	val *= coeffB;
	val = val % 9973;

	printf("%d", (coeffA * val + coeffB) % 9973);

	return 0;
}