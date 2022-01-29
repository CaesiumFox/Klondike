cc := g++
cflags := -I ./include/ -c
ld := g++
lflags := -l SDL2main -l SDL2 -l SDL2_image

srcs := $(wildcard src/*.cpp)
objs := $(patsubst src/%.cpp, build/%.o, $(srcs))

exe := klondike.elf

.PHONY: all
all: $(objs)
	$(cc) $(lflags) $^ -o $(exe)

build/:
	mkdir -p build

build/%.o: src/%.cpp build/
	$(cc) $(cflags) $< -o $@
