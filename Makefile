CC ?= clang
CFLAGS := -g -Wall -Wextra -std=c99 -I. $(CFLAGS)

TARGET = marasm
OBJECTS = map.o bytecode.o marasm.o main.o

all: dvm.map $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

$(OBJECTS):
	$(CC) -c $(CFLAGS) -o "$@" "$(shell basename "$@" .o).c"

dvm.map: opcode.h
	ruby make_opcodes.rb opcode.h > dvm.map

clean: distclean

distclean:
	-rm -f $(OBJECTS) $(TARGET) dvm.map
	-rm -rf *.dSYM
