CC = gcc
CPPFLAGS = -pthread
INCLUDES =
TARGET = pthreadApi.out
SRC = 1pthreads.c

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


	

