# Spy Hunter

_Project made for Basics of Computer Programming on GUT._

## Description

In this game you move around with your car using arrow keys (top, left, down & bottom). If you drive off to the side of the road or hit any car, you lose. The game continues indefinitely.

#### Technology used

This project was written in C++ with the use of SDL2 library for GUI.

## Example screen from the game

## Installation (on Linux)

Requirements:

- g++

#### Method 1 - Using installed SDL (requires root privileges)

- fedora: sudo dnf install SDL2-devel
- ubuntu: sudo apt install libsdl2-dev
- arch: sudo pacman -S sdl2

Running:

```
g++ src/*.cpp -lSDL2
./a.out
```

or (requires cmake & make):

```
mkdir build && cd build
cmake ..
make
./spy_hunter
```

#### Method 2 - Using SDL installed locally (requires make)

1. Download SDL source code from: https://wiki.libsdl.org/SDL2/SourceCode
1. unzip it and cd into it
1. Run the following:

```
mkdir build && cd build
mkdir ~/sdl
../configure --prefix=/home/<user-name>/sdl
make
make install
```

4. cd into spy hunter project directory

```
cp -r ~/sdl/include/SDL2 .
cp ~/sdl/lib/libSDL2.a .
```

5. Now run either this:

```
g++ -I. src/\*.cpp libSDL2.a
```

_note: you may want to change #include <SDL2/SDL.h> to #include "SDL.h" if it doesn't work_  
or this:

```
g++ src/\*.cpp $(~/sdl/bin/sdl2-config --cflags --libs)
```

#### Method 3 - Using template given to us during the project

1. Copy SDL2-2.0.10 directory and libSDL2.a file to spy hunter project directory
1. Run the following commands:

```
ln -sT SDL2-2.0.10/include SDL2
g++ -I. src/\*.cpp libSDL2.a
```
