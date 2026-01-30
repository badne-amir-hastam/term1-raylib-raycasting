CC      := gcc
CFLAGS  := -std=c99 -Wall -Wextra -O2 -Idirectories/include
LDFLAGS := -Ldirectories/lib -lraylib -lopengl32 -lgdi32 -lwinmm

SRC     := src/main.c 
OUT     := game.exe

all: $(OUT)

$(OUT): $(SRC)
# 	@if not exist build mkdir build
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)
	.\game.exe
com:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)
run:
	.\game.exe
clean:
	del $(OUT)