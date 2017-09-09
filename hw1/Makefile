CSTD = c99
CC = gcc
INCLUDES = -I ./
CFLAGS = -O0 -Wall -g -std=$(CSTD)
CPPFLAGS =
LDFLAGS = 
TARGET = hw1

SOURCES:= circbuff.c\
	doublyLinked.c\
		main.c


OBJS = $(SOURCES:.c=.o)
%.o : %.c
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< $(CFLAGS) -o $@


.PHONY: build
build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET) $(BASENAME).map *.o *.asm *.i *.dep
