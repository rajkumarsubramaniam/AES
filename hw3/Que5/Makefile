CC = gcc
CPPFLAGS = -pthread
INCLUDES =
TARGET = multiThreaded.out
SRC = 5fileHandling.c

OBJS = $(SRC:.c=.o) 

%.o : %.c
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $< -o $@
	
.PHONY: build
build: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.out *.o $(TARGET)
	
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CPPFLAGS) -o $@

