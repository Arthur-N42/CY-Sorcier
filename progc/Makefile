CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRC_FILES = main.c basic.c traitement_s.c traitement_t.c

OBJ_FILES = $(SRC_FILES:.c=.o)

EXECUTABLE = traitements.exe

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o $(EXECUTABLE)
