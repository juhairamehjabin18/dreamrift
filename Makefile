# Dreamrift - cross-platform Makefile
# Works on Windows (MinGW/w64devkit), macOS (Homebrew raylib), and Linux.

CC = gcc
SRC = $(wildcard src/*.c)
CFLAGS = -Wall -std=c99 -O1

ifeq ($(OS),Windows_NT)
    EXE = dreamrift.exe
    LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
else
    UNAME_S := $(shell uname -s)
    EXE = dreamrift
    ifeq ($(UNAME_S),Darwin)
        # Homebrew on Apple Silicon installs to /opt/homebrew
        CFLAGS += -I/opt/homebrew/include
        LIBS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa \
               -framework IOKit -framework CoreVideo -framework CoreAudio
    else
        LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    endif
endif

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(LIBS)

run: all
	./$(EXE)

clean:
	rm -f dreamrift dreamrift.exe