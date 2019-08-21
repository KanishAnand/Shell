#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void main()
{
    char *a = "kanish";
    char b[] = "anand";
    strcat(a, b);
    printf("%s\n", a);
}
