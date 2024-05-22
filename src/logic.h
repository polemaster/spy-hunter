#pragma once
#include "sequence.h"
#include "constants.h"

enum car_generator { TOP, BOTTOM }; // for deciding whether generate new cars from the bottom or the top

typedef struct {
    int width, height;
    int x, y;
    int vel_x, vel_y;
    int type;
    bool exists;
} Object;

typedef struct {
    int width;
    int height;
} SpriteSize;

// grid for starting positions of trees and bushes
typedef struct {
    int rows, columns;
    int cell_height, cell_width;
} Grid;

typedef struct {
    int width;
    int x_left, x_right;
    SDL_Color color;
} Road;

class Logic {
public:
    Logic(const int w, const int h);
    ~Logic();

    void loadTextureSizes(const int *arr, const int x);
    void newGame();
    int numOfObjects();
    int getObjectType(const int a);
    SDL_Rect getObjectRect(const int a);
    bool getObjectExistence(const int a);
    Road *getRoad();
    void updateWindow(const int w, const int h);
    void handleInput(const Uint8 *key_states);
    void updateState(const Uint32 delta, const bool add_enemy, const bool add_ally);
    bool scoreUpdate();
    bool isCollision();
private:
    Road _road;
    Object *_objects;
    SpriteSize *_sizes;
    Grid _grid;
    Sequence _sequence;
    int _n; // number of total possible objects in game
    int _screen_width, _screen_height;

    void _setBasicObjPar(const int a, const int type);
    void _generateTreeOrBush(int a);
    void _generateEnemy(const bool generate, const car_generator where );
    void _generateAlly(const bool generate, const car_generator where );
    void _removeObjects();
    void _initObjects();
    int _getRowCoords(const int x);
    int _getColumnCoords(int x);
    int _getColumnFromCoords(int x);
};

bool _checkCollision(const Object *obj1, const Object *obj2);
