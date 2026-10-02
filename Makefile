# detect if Windows
ifeq ($(OS),Windows_NT)
    EXE = .exe
    RM = del /Q
else
    EXE =
    RM = rm -f
endif

CC = gcc
CFLAGS = -Wall -I.
TARGET = server$(EXE)

CMD_SRC   = $(wildcard commands/*.c)
UTIL_SRC  = $(wildcard utils/*.c)
SRC       = main.c $(CMD_SRC) $(UTIL_SRC)

OBJS = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	$(RM) $(OBJS)

# generic rule for any .c file in our directory.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) main.o commands\*.o utils\*.o $(TARGET)