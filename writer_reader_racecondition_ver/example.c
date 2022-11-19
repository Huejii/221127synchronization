#include <stdio.h>
#include <string.h>
#include <ctype.h>

char* S = "Happy Merry Christmas~! ";

int main()
{
    char* new;
    strncpy(new, S, strlen(S));
    for (int i = 0; i< strlen(S); i++) {
        if (S[i] >= 'a' && S[i] <= 'z')
            new[i] = toupper(new[i]);
    }
    printf("%s\n", S);
    return 0;
}