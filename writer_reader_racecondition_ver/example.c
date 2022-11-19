#include <stdio.h>
#include <string.h>
#include <ctype.h>


int main()
{
    char* S = "Happy Merry Christmas~! ";
    for (int i = 0; i< strlen(S); i++) {
        if (S[i] >= 'a' && S[i] <= 'z')
            S[i] -= 32 ;
    }
    printf("%s\n", S);
    return 0;
}