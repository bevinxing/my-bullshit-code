#include <string.h>

char str[] = "china:beijing:haidian::zhongguancun:tengfei:";
char *buf = &str;
char *token = NULL;
while (NULL != (token = strsep(&buf, ":"))) {
    printf("Token: %s\n", token);
}

/*
 * Output: 
Token: china
Token: beijing
Token: haidian
Token: 
Token: zhongguancun
Token: tengfei
Token: 

 * Prototype:
char *strsep(char **stringp, const char *delim);
reentrant and should be faster than strtok.

 * Note:
strsep - This function modifies its first argument.
*/