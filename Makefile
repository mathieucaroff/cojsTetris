# @author Mathieu CAROFF

CC = gcc
CFLAGS = -g
LDLIBS = -lmicrohttpd


BINARIES = fileserver
DIRECTORIES = tmp www

EXISTING_BINARIES != ls $(BINARIES) 2>/dev/null

all: $(foreach binary, $(BINARIES), bin/$(binary)) www test

test: bin/fileserver
	bin/fileserver 8888

clean: clean-tmp

clean-tmp:
ifeq "$(shell ls -d tmp 2> /dev/null)" "tmp"
	rm -r tmp
endif

clean-bin:
ifeq "$(shell ls -d bin 2> /dev/null)" "bin"
	rm -r bin
endif

# Recipe to make .o files from .c files:
#tmp/fileserver.o: c/fileserver.c; $(O_FROM_C_COMPILE)
$(foreach binary, $(BINARIES), tmp/$(binary).o: tmp c/$(binary).c; $(CC) $(CFLAGS) c/$(binary).c -c -o $$@)


# Recipe to make bin files from .o files:
#bin/fileserver: tmp/fileserver.o; $(BIN_FROM_O_COMPILE)
$(foreach binary, $(BINARIES), bin/$(binary): bin tmp/$(binary).o; $(CC) $(CFLAGS) tmp/$(binary).o $(LDLIBS) -o $$@)


# Recipe to make direcories
tmp:
ifeq "$(shell ls -d tmp 2> /dev/null)" ""
	mkdir -p tmp
endif

bin:
ifeq "$(shell ls -d bin 2> /dev/null)" ""
	mkdir -p bin
endif


ifeq "${shell [ $$(ls www -1 2>/dev/null | wc -l) -lt 3 ] && echo true}" "true"
WWW_DEPENDS = make-www $(foreach filename, jsTetris.html jstetris.js jstetris.js.d, www/$(filename))
endif

www: $(WWW_DEPENDS)

make-www:
	mkdir -p www

www/%: make-www
	ln -sf ../jsTetris/$* www

.PHONY: all www make-www
