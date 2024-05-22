#pragma once
#include <SDL2/SDL.h>
// note: paths of images are stored in path.cpp

// number of tree and bush images included in the game, number of other images (explosion)
const int TREE_IMGS = 4;
const int BUSH_IMGS = 2;
const int PLANTS_IMGS = TREE_IMGS + BUSH_IMGS;
const int OTHER_IMGS = 1;
const int ALL_IMGS = 3 + PLANTS_IMGS + OTHER_IMGS;

const int MY_CAR = 0;
const int ALLY_CAR = 1;
const int ENEMY_CAR = 2;
const int BUSH0 = 3;
const int BUSH1 = 4;
const int TREE0 = 5;
const int TREE1 = 6;
const int TREE2 = 7;
const int TREE3 = 8;
const int BOOM = 9;

// maximum number of friendly/enemy cars, trees and bushes
// number of objects
const int FRIENDLY_CARS = 3; // maximum at the same time
const int ENEMY_CARS = 3; // the same
const int PLANTS = 20;
const int OTHER_OBJS = 1; // other objects: 1 - explosion

// menu options
const int MENU_ITEMS = 4;
const int MENU_WIDTH = 300;
const int MENU_SPACE = 5; // space between lines (vertically)
const int MENU_TOP_OFFSET = 10;
const int MENU_LEFT_OFFSET = 10;
const SDL_Color MENU_BG_COLOR = { 255, 0, 0, 255 }; // red
const SDL_Color MENU_TXT_COLOR = { 0, 0, 0, 255 }; // black

// top bar options
const int INFO_TOP_OFFSET = 10;
const int INFO_LEFT_OFFSET = 10;
const SDL_Color INFO_BG_COLOR = { 0, 0, 80, 255 }; // navy blue
const SDL_Color INFO_TXT_COLOR = { 255, 255, 255, 255 }; // white

// windows options
const int START_SCREEN_WIDTH = 1500;
const int START_SCREEN_HEIGHT = 950;
const char * const WINDOW_TITLE = "Spy Hunter";
const int MIN_SCREEN_WIDTH = 1500;
const int MIN_SCREEN_HEIGHT = 950;

// road options
const int ROAD_WIDTH = 600;
const SDL_Color ROAD_COLOR = { 85, 85, 85, 255 }; // grey

// miscellaneous
const int MAX_SPEED = 25;
const int MAX_STRING_LENGTH = 100;
const int MAX_GENERATOR_TIME = 7; // max time in seconds before generating a new car
const int MIN_GENERATOR_TIME = 3; // analogous to above but min time
const bool VSYNC = true; // turn on/off vertical synchronization
const double FONT_SIZE = 1.5;
const SDL_Color BG_COLOR = { 40, 126, 22, 255 }; // green

// legend options
const int LEGEND_SPACE = 5; // space vertically between text/images
const double LEGEND_SCALE = 0.5;
const double LEGEND_FONT_SIZE = 1;
const int LEGEND_LEFT_OFFSET = 5;
const SDL_Color LEGEND_TXT_COLOR = { 0, 0, 0, 255 }; // black
const SDL_Color LEGEND_BG_COLOR = BG_COLOR;
