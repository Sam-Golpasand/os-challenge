CC = gcc
CFLAGS = -Wall -O3 -I.
TARGET = server

UTIL_SRC = $(wildcard utils/*.c)
SRC      = main.c $(UTIL_SRC)

OBJS = $(SRC:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	$(RM) $(OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)