CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -pedantic

TARGETS := football.exe tempature.exe

.PHONY: all clean

all: $(TARGETS)

football.exe: football.c
	$(CC) $(CFLAGS) -o $@ $<

tempature.exe: tempature.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
