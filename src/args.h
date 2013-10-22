/*
 * luafcgid -- A simple multi-threaded Lua+FastCGI daemon.
 *
 * this code is provided under the "Simplified BSD License"
 * (c) Anthony Viallard 2013
 */

#ifndef _LFCGID_ARGS_H_
#define _LFCGID_ARGS_H_

struct args {
	const char *log_level;
	int log_trace;
	int log_use_syslog;
	const char *cfg_filename;
};

int args_parse(struct args *args, int argc, char *argv[]);

#endif
