CC=gcc
AR=ar
CFLAGS=-g -std=gnu99 -fPIC
INCS=-I$(CURDIR)
LIBS=-L$(CURDIR)

QUIET_CC = @echo '  ' CC '  ' $@;
QUIET_AR = @echo '  ' AR '  ' $@;

SOURCES=$(wildcard nn/*.c) $(wildcard rand/*.c) $(wildcard fft/*.c)
HEADERS=$(wildcard *.h)

OBJS=$(patsubst %.c, build/%.o, $(SOURCES))

LIBRARY=libccas.so

.PHONY: all clean

all: $(LIBRARY)

clean:
	rm -f $(OBJS) $(LIBRARY)
	rm -rf build

build:
	mkdir -p build
	mkdir -p build/nn
	mkdir -p build/rand
	mkdir -p build/fft

libccas.a: build $(OBJS) $(HEADERS)
	$(QUIET_AR)$(AR) rcs $@ $(OBJS)

libccas.so: build $(OBJS) $(HEADERS)
	$(QUIET_CC)$(CC) -shared $(OBJS) -lm -o $@

libccas.dylib: build $(OBJS) $(HEADERS)
	$(QUIET_CC)$(CC) -shared $(OBJS) -lm -o $@

build/%.o: %.c
	$(QUIET_CC)$(CC) $(CFLAGS) $(INCS) -c $< -o $@

