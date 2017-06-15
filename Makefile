CFLAGS=-g
LDLIBS=-lmicrohttpd -lmagic
GENERAL_C_COMPILE=$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@`

ifeq "${shell [ $$(ls www -1 2>&1 | wc -l) -lt 3 ] && echo true}" "true"
WWW_DEPENDS = make-www www/jsTetris.html www/jstetris.js www/jstetris.js.d
endif

all: fileserver

fileserver: c/fileserver.o
	$(GENERAL_C_COMPILE)

eval:
	$(EVAL)

www: $(WWW_DEPENDS)

make-www:
	mkdir -p www

www/%: make-www
	ln -sf ../jsTetris/$* www

.PHONY: all www make-www
