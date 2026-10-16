CC = gcc
CFLAGS = -Wall -I.
TARGET = server

CMD_SRC  = $(wildcard commands/*.c)
UTIL_SRC = $(wildcard utils/*.c)
SRC      = main.c $(CMD_SRC) $(UTIL_SRC)

OBJS = $(SRC:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	$(RM) $(OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)