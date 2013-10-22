/*
 * luafcgid -- A simple multi-threaded Lua+FastCGI daemon.
 *
 * this code is provided under the "Simplified BSD License"
 * (c) Anthony Viallard 2013
 */

#ifndef _LFCGID_STR_H_
#define _LFCGID_STR_H_

#include <string.h>

#define str_matches(s, c_str)						\
        ({                                                              \
                const char __dummy[] = c_str;                           \
                (void)(&__dummy);                                       \
                (memcmp (s, c_str, sizeof(c_str)) == 0);   \
        })

#endif
