# Change application name here (Executable output name)
TARGET = proyecto
# Compiler
CC = gcc
# Debug
DEBUG = -g
# Optimisation
OPT = -O0
# Warnings
WARN = -Wall

LIBS=-lsqlite3

PTHREAD = -pthread

CCFLAGS = $(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB = `pkg-config --cflags --libs gtk+-3.0`

# Linker
LD = gcc
LDFLAGS = $(PTHREAD) $(GTKLIB) -export-dynamic

OBJS = main.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

main.o: main.c
	$(CC) -c $(CCFLAGS) main.c $(GTKLIB) -o main.o 

clean:
	rm -f *.o $(TARGET)
