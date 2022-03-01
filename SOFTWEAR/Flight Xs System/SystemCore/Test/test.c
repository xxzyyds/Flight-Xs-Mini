#include <stdio.h>
#include <stdlib.h>
void main(void)
{
	char a;
	a = getchar();
	printf("%c\n" , & a);
	system("pause");
}