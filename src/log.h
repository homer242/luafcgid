/*
 * luafcgid -- A simple multi-threaded Lua+FastCGI daemon.
 *
 * this code is provided under the "Simplified BSD License"
 * (c) Anthony Viallard 2013
 */

#ifndef _LFCGID_LOG_H_
#define _LFCGID_LOG_H_

/*
 * The level determines the importance of the message.
 * It is based on syslog's level with some addons.
 *
 * The levels are, in order of decreasing importance:
 *
 *   - fatal      (LOG_CRIT)
 *   - err        (LOG_ERR)
 *   - warn       (LOG_WARNING)
 *   - notice     (LOG_NOTICE)
 *   - info       (LOG_INFO)
 *   - dbg        (LOG_DEBUG)
 *   - dbg2       (LOG_DEBUG)
 *   - dbg3       (LOG_DEBUG)
 *   - dbg4       (LOG_DEBUG)
 *   - dbg5       (LOG_DEBUG)
 *
 * The trace log has a debug level. It can be enable
 * by setting _log_trace_enabled to 1 and setting _log_level >= dbg0.
 *
 * use log_setup() to configure the level and the activation of trace.
 */

#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>
#include <ctype.h>
#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "str.h"

#define LOG_DEBUG1     LOG_DEBUG
#define LOG_DEBUG2     LOG_DEBUG1 + 1
#define LOG_DEBUG3     LOG_DEBUG1 + 2
#define LOG_DEBUG4     LOG_DEBUG1 + 3
#define LOG_DEBUG5     LOG_DEBUG1 + 4

extern int _log_level;
extern int _log_trace_enabled;
extern int _log_use_syslog;
extern unsigned short int _log_lshiftspace;
extern const char *__progname;

static inline void _log_write(int priority, char const *format, ...)
{
	va_list ap;

	va_start(ap, format);

	if(_log_use_syslog)
	{
		vsyslog(priority, format, ap);
	}
	else
	{
		/* Prefix the console message as it look on the syslog. */
		time_t _now = time(NULL);
		struct tm *_tm = localtime(&_now);
		char _date[20];
		strftime(_date, sizeof(_date), "%b %e %H:%M:%S", _tm);
		char _hostname[64];
		gethostname(_hostname, sizeof(_hostname));
		char _longformat[1024];
		snprintf(_longformat, sizeof(_longformat), "%s %s %s[%d]: %s",
			 _date, _hostname, __progname, getpid(), format);
		FILE *_stream = priority <= LOG_ERR ? stderr : stdout;
		vfprintf(_stream, _longformat, ap);
		fflush(_stream);
	}

	va_end(ap);
}

#define _log(priority, fmt, ...)					\
	do {								\
		if(_log_level >= priority) {				\
			_log_write(priority, fmt "\n", ##__VA_ARGS__);	\
		}							\
	} while(0)

#define _log_dbgX(localpriority, fmt, ...)				\
	do {								\
		if(_log_level >= localpriority) {			\
			_log_write(LOG_DEBUG, fmt "\n", ##__VA_ARGS__);	\
		}							\
	} while(0)

/*
 * main API
 */
#define log_open(ident, opt, facility)		\
        openlog(basename(ident), opt, facility)

#define log_close()				\
        closelog()

#define log_fatal(fmt, ...)						\
        _log(LOG_CRIT,							\
	     "-- FATAL -- %s:%d %s() %.*s" fmt,				\
	     __FILE__, __LINE__, __func__, _log_lshiftspace, " ", ##__VA_ARGS__)

#define log_err(fmt, ...)						\
        _log(LOG_ERR,							\
	     "-- error -- %s:%d %s() %.*s" fmt,				\
	     __FILE__, __LINE__, __func__, _log_lshiftspace, " ", ##__VA_ARGS__)

#define log_warn(fmt, ...)						\
        _log(LOG_WARNING,						\
	     "-- warning -- %s:%d %s() %.*s" fmt,			\
	     __FILE__, __LINE__, __func__, _log_lshiftspace, " ", ##__VA_ARGS__)

#define log_notice(fmt, ...)						\
        _log(LOG_NOTICE,						\
	     "notice %s:%d %s() %.*s" fmt,				\
	     __FILE__, __LINE__, __func__, _log_lshiftspace, " ", ##__VA_ARGS__)

#define log_info(fmt, ...)						\
	_log(LOG_INFO,							\
	     "info %s:%d %s() %.*s" fmt,				\
	     __FILE__, __LINE__, __func__, _log_lshiftspace, " ", ##__VA_ARGS__)

#define log_dbg(fmt, ...)						\
        _log(LOG_DEBUG,							\
	     "dbg %s:%d %s() %.*s" fmt,					\
	     __FILE__, __LINE__, __func__, _log_lshiftspace, " ", ##__VA_ARGS__)

#define log_dbg1(fmt, ...)						\
        _log_dbgX(LOG_DEBUG1,						\
		  "dbg1 %s:%d %s() %.*s" fmt,				\
		  __FILE__, __LINE__, __func__, _log_lshiftspace, " ", ##__VA_ARGS__)

#define log_dbg2(fmt, ...)						\
        _log_dbgX(LOG_DEBUG2,						\
		  "dbg2 %s:%d %s() %.*s" fmt,				\
		  __FILE__, __LINE__, __func__, _log_lshiftspace, " ", ##__VA_ARGS__)

#define log_dbg3(fmt, ...)						\
	_log_dbgX(LOG_DEBUG3,						\
		  "dbg3 %s:%d %s() %.*s" fmt,				\
		  __FILE__, __LINE__, __func__, _log_lshiftspace, " ", ##__VA_ARGS__)

#define log_dbg4(fmt, ...)						\
	_log_dbgX(LOG_DEBUG4,						\
		  "dbg4 %s:%d %s() %.*s" fmt,				\
		  __FILE__, __LINE__, __func__, _log_lshiftspace, " ", ##__VA_ARGS__)

#define log_dbg5(fmt, ...)						\
	_log_dbgX(LOG_DEBUG5,						\
		  "dbg5 %s:%d %s() %.*s" fmt,				\
		  __FILE__, __LINE__, __func__, _log_lshiftspace, " ", ##__VA_ARGS__)

#define log_trace(fmt, ...)						\
	do {								\
		if(_log_trace_enabled) {				\
			_log(LOG_DEBUG,					\
			     "trace %s:%d %s() %.*s" fmt,		\
			     __FILE__, __LINE__, __func__, _log_lshiftspace, " ", ##__VA_ARGS__); \
		}							\
	} while(0)

static inline void log_setup(const char *level, int trace_enabled, int use_syslog)
{
	if(str_matches(level, "fatal")){
		_log_level = LOG_CRIT;
	} else if(str_matches(level, "err")) {
		_log_level = LOG_ERR;
	} else if(str_matches(level, "warn")) {
		_log_level = LOG_WARNING;
	} else if(str_matches(level, "notice")) {
		_log_level = LOG_NOTICE;
	} else if(str_matches(level, "info")) {
		_log_level = LOG_INFO;
	} else if(str_matches(level, "dbg")) {
		_log_level = LOG_DEBUG;
	} else if(str_matches(level, "dbg2")) {
		_log_level = LOG_DEBUG2;
	} else if(str_matches(level, "dbg3")) {
		_log_level = LOG_DEBUG3;
	} else if(str_matches(level, "dbg4")) {
		_log_level = LOG_DEBUG4;
	} else if(str_matches(level, "dbg5")) {
		_log_level = LOG_DEBUG5;
	} else {
		_log_level = LOG_INFO;
	}

	_log_trace_enabled = trace_enabled;
	_log_use_syslog = use_syslog;

	log_dbg5("level=%s (%d) - trace_enabled=%d - use_syslog=%d",
		 level, _log_level, _log_trace_enabled, _log_use_syslog);
}

static inline void log_info_hexdump(const void *buf, size_t n)
{
	const unsigned char *data = buf;
	char hex_line[49 + 1];
	char asc_line[16 + 1];
	char *hex_ptr;
	char *asc_ptr;
	char ch;
	size_t i;

	log_info("Dumping %zu bytes at %p", n, buf);

	hex_line[sizeof hex_line - 1] = '\0';
	asc_line[sizeof asc_line - 1] = '\0';
	hex_ptr = hex_line;
	asc_ptr = asc_line;
	for(i = 0; i < n; i++)
	{
		if(i % 16 == 0)
		{
			if(i != 0)
			{
				log_info("%08x  %s |%s|",
					 i - 16, hex_line, asc_line);
			}
			hex_ptr = hex_line;
			asc_ptr = asc_line;
		}
		else if(i % 8 == 0)
		{
			sprintf(hex_ptr, " ");
			hex_ptr += 1;
		}

		sprintf(hex_ptr, "%02x ", data[i]);
		hex_ptr += 3;

		if(isprint(data[i]))
			ch = data[i];
		else
			ch = '.';

		*asc_ptr++ = ch;
	}
}

#endif
