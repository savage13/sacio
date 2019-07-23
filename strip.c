
#include <string.h>
#include <ctype.h>

#include "strip.h"

char *
rstrip(char *s) {
    char *back;
    if (*s == 0) {
        return s;
    }
    back = s + strlen(s) - 1;
    while (back >= s && isspace(*back)) {
        --back;
    }
    *(back + 1) = 0;
    return s;
}

char *
lstrip(char *s) {
    if (*s == 0) {
        return s;
    }
    while (isspace(*s)) {
        s++;
    }
    return s;
}

