CC = gcc
CFLAGS = -std=c99 -Wall

LD = gcc -o
LFLAGS = -Wall

SOURCES = src/main.c
OBJECTS = $(SOURCES:src/%.c=obj/%.o)

all: bin/exp

bin/exp: $(OBJECTS)
	$(LD) $@ $(LFLAGS) $(OBJECTS)

$(OBJECTS): obj/%.o : src/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
