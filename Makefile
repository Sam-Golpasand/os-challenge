CC = gcc
CFLAGS = -Wall -O3 -pthread -I.
TARGET = server

SCHED ?= prio_batch

UTIL_SRC  = $(wildcard utils/*.c)
SCHED_SRC = schedulers/$(SCHED).c
SRC       = main.c $(UTIL_SRC) $(SCHED_SRC)

OBJS = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	$(RM) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) schedulers/*.o $(TARGET)