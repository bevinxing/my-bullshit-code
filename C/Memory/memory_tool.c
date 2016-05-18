/******************************************************************************
* Copyright (C), 2015.
* Filename: os_tool.c
* Description: Implementation of OS specific functions.
* Created by: Bevin Xing
* Revision history:
* Revision  Date          Author           Description
* -----------------------------------------------------------------------------------------------------------------
* v0.1     2015/10/20     Bevin            Create this file. Linux OS
*/

#include <stdlib.h>
#include <string.h>

void *os_malloc(size_t size) {
    return malloc(size);
}

void *os_zmalloc(size_t size) {
    void *b = os_malloc(size);
    if (b)
       os_memset(b, 0, size);
    return b;
}

void os_free(void *p) {
    free(p);
}

void *os_memset(void *s, int c, size_t n) {
    return memset(s, c, n);
}

void *os_memcpy(void *d, const void *s, size_t n) {
    return memcpy(d, s, n);
}

int os_memcmp(const void *s1, const void *s2, size_t n) {
    return memcmp(s1, s2, n);
}

int os_strcmp(const char *s1, const char *s2) {
    return strcmp(s1, s2);
}