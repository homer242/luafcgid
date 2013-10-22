/*
 * luafcgid -- A simple multi-threaded Lua+FastCGI daemon.
 *
 * this code is provided under the "Simplified BSD License"
 * (c) Anthony Viallard 2013
 */

#include "log.h"

int _log_level = LOG_INFO;
int _log_trace_enabled = 0;
int _log_use_syslog = 0;
unsigned short int _log_lshiftspace = 0;
