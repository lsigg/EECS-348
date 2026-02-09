CC = gcc
CFLAGS = -Wall -g

TARGET = evenodd
OBJS = main.o evenodd.o
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c evenodd.h
	$(CC) $(CFLAGS) -c main.c

evenodd.o: evenodd.c evenodd.h
	$(CC) $(CFLAGS) -c evenodd.c

clean:
	rm -f $(TARGET) $(OBJS)