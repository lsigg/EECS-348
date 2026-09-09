CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = lab02
OBJS = main.o isEven.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c isEven.h
isEven.o: isEven.c isEven.h

clean:
	rm -f $(TARGET) $(OBJS)