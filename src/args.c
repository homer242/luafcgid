/*
 * luafcgid -- A simple multi-threaded Lua+FastCGI daemon.
 *
 * this code is provided under the "Simplified BSD License"
 * (c) Anthony Viallard 2013
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "args.h"

static void usage(int argc, char **argv)
{
	printf("Usage: %s [-L <log_level>] [-GT] [-c <cfg filename>[-h]\n", argv[0]);
	printf("Options:\n");
	printf(" -L --log-level <lvl> Level of log.\n");
	printf(" -T --log-trace       Print the log_trace in debug level.\n");
	printf(" -G --log-use-syslog  Use syslog to emit message.\n");
	printf(" -c, --cfg-filename=FILE\t\tConfig file to be used\n");
	printf(" -h, --help\t\tDisplay this help\n");
}

int args_parse(struct args *args, int argc, char **argv)
{
        int c, ind;
        char short_options[] = "L:TGf:h";
        struct option long_options [] = {
		{"log-level", 1, NULL, 'L'},
		{"log-trace", 0, NULL, 'T'},
		{"log-use-syslog", 0, NULL, 'G'},
                {"cfg-filename", required_argument, 0, 'c' },
		{"help", 0, NULL, 'h'},
                {0, 0, 0, 0 }
        };

	/* default configuration */
	args->cfg_filename = "/etc/luafcgid.lua";
	args->log_level = "info";
	args->log_trace = 0;
	args->log_use_syslog = 0;

	/* parse */
        while((c = getopt_long(argc, argv,
			       short_options, long_options, &ind)) != EOF)
        {
                switch(c)
                {
                case 0:
                        break;
		case 'L':
			args->log_level = optarg;
			break;
		case 'T':
			args->log_trace = 1;
			break;
		case 'G':
			args->log_use_syslog = 1;
			break;
                case 'c':
                        args->cfg_filename = optarg;
                        break;
                case 'h':
			usage(argc, argv);
                        return 1;
                default:
                        break;
                }
        }

	return 0;
}
