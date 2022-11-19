#include <stdio.h>
#include <string.h>
#include <ctype.h>

char* S = "Happy Merry Christmas~! ";

int main()
{
    char* S = "Happy Merry Christmas~! ";
    char* N = "Happy new year~! ";
    S = N;
    printf("%s\n", S);
    return 0;
}