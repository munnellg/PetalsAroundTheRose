OBJS = $(wildcard src/*.c)

CC = clang

COMPILER_FLAGS = -Wall

LINKER_FLAGS = 

BIN = petals

all : $(OBJS)
	$(CC) $(COMPILER_FLAGS) $(OBJS) -o $(BIN) $(LINKER_FLAGS)
