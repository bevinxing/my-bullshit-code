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
分隔符的前后内容都会被当做token解析出来，即使是'\0'
注意连续出现的分隔符和字符串末尾出现的分隔符。
*/

/* Source Code:
char *strsep(char **stringp, const char *delim)
{
    char *s;
    const char *spanp;
    int c, sc;
    char *tok;
    if ((s = *stringp)== NULL)
        return (NULL);
    for (tok = s;;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc =*spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
}
*/