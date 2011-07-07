#include <stdlib.h>
#include <stdio.h>

int main()
{
	int num = 0;
	int i = 0;
	unsigned __int64 arr[100];
	scanf("%d", &num);

	arr[0] = 1; 
	arr[1] = 1;
	for(i = 2; i < num; ++i) 
	{
		arr[i] = arr[i-1] + arr[i-2];
	}
	printf("%I64d", 2 * arr[num - 1]);
	return 0;
}