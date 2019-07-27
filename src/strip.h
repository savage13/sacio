
#ifndef _STRIP_H_
#define _STRIP_H_

char * rstrip(char *s);
char * lstrip(char *s);

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef HAVE_STRLCPY
size_t strlcpy(char *dst, const char *src, size_t size);
#endif

#ifndef HAVE_STRLCAT
size_t strlcat(char *dst, const char *src, size_t size);
#endif

#endif /* _STRIP_H_ */
