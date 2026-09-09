CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = lab02
OBJS = main.o isEven.o isOdd.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c isEven.h isOdd.h
isEven.o: isEven.c isEven.h
isOdd.o: isOdd.c isOdd.h

clean:
	rm -f $(TARGET) $(OBJS)

#make
#./lab02
