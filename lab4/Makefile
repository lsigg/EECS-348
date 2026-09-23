CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -pedantic

TARGETS := task1.exe task2.exe

.PHONY: all clean

all: $(TARGETS)

task1.exe: task1.c
	$(CC) $(CFLAGS) -o $@ $<

task2.exe: task2.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
