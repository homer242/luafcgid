bin_PROGRAMS = lfcgid

lfcgid_SOURCES = main.c main.h \
	buffer.c buffer.h \
	config.c config.h \
	pool.c pool.h \
	request.c request.h \
	args.c args.h \
	log.c log.h \
	str.h

lfcgid_CFLAGS = $(LUA_CFLAGS) $(LFCGI_CFLAGS)
lfcgid_LDADD = $(LUA_LIBS) -lfcgi -lpthread
