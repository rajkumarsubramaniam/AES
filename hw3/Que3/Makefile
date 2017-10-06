CC = gcc
CPPFLAGS = -pthread -lrt
INCLUDES =
TARGET = profiler.out
SRC = profiler.c

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

