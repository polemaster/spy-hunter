#include "logic.h"
#include <stdio.h>
#include <stdlib.h>

Logic::Logic(const int w, const int h) {
    _n = 1 + FRIENDLY_CARS + ENEMY_CARS + PLANTS + OTHER_OBJS;
    _sizes = NULL;
    _objects = NULL;
    _screen_width = w;
    _screen_height = h;
}

Logic::~Logic() {
    _removeObjects();
    if (_sizes != NULL)
        delete[] _sizes;
}

void Logic::loadTextureSizes(const int *arr, const int x) {
    if ( _sizes != NULL )
        delete _sizes;
    _sizes = new SpriteSize[x];

    _sizes[MY_CAR] = {arr[0], arr[1]};
    _sizes[ALLY_CAR] = {arr[2], arr[3]};
    _sizes[ENEMY_CAR] = {arr[4], arr[5]};
    _sizes[BUSH0] = {arr[6], arr[7]};
    _sizes[BUSH1] = {arr[8], arr[9]};
    _sizes[TREE0] = {arr[10], arr[11]};
    _sizes[TREE1] = {arr[12], arr[13]};
    _sizes[TREE2] = {arr[14], arr[15]};
    _sizes[TREE3] = {arr[16], arr[17]};
    _sizes[BOOM] = {arr[18], arr[19]};
}

void Logic::newGame() {
    _removeObjects();

    _road.width = ROAD_WIDTH;
    _road.color = ROAD_COLOR;
    _road.x_left = _screen_width / 2 - _road.width / 2;
    _road.x_right = _screen_width / 2 + _road.width / 2;

    _objects = new Object[_n];
    // set basic parameters of all the objects
    for (int i = 0; i < _n; ++i) {
        _objects[i].x = 0;
        _objects[i].y = 0;
        _objects[i].vel_x = 0;
        _objects[i].vel_y = 0;
        _objects[i].exists = false;
        if ( i == 0 ) {
            _setBasicObjPar(i, MY_CAR);
            _objects[i].x = _screen_width / 2;
            _objects[i].y = 2 * _screen_height / 3;
            _objects[i].exists = true;
        }
        else if ( i < 1 + FRIENDLY_CARS ) {
            _setBasicObjPar(i, ALLY_CAR);
        }
        else if ( i < 1 + FRIENDLY_CARS + ENEMY_CARS ) {
            _setBasicObjPar(i, ENEMY_CAR);
        }
        else if ( i < 1 + FRIENDLY_CARS + ENEMY_CARS + PLANTS ) {
            int type = rand() % (PLANTS_IMGS) + BUSH0;
            _setBasicObjPar(i, type);
        }
        else { // if other (now only explosion)
            _setBasicObjPar(i, BOOM);
        }
    }
    // find the highest and the widest plants
    int highest = 0;
    int widest = 0;
    for (int i = BUSH0; i < BUSH0 + PLANTS_IMGS; ++i) {
        if (_sizes[i].height > highest)
            highest = _sizes[i].height;
        if (_sizes[i].width > widest)
            widest = _sizes[i].width;
    }
    // set starting grid
    _grid.columns = _road.x_left / widest * 2;
    _grid.rows = _screen_height / highest;
    _grid.cell_height = highest;
    _grid.cell_width = widest;

    _sequence.createSequence(_grid.columns);


    // create grid filled with trees every second row and column
    int iter = 1 + FRIENDLY_CARS + ENEMY_CARS;
    while (iter < _n - OTHER_OBJS) {
        for (int j = 0; j < _grid.rows; ++j) {
            if (iter == _n - OTHER_OBJS)
                break;
            for (int k = 0; k < _grid.columns; k+=2) {
                if (j % 2 == 0 && k == 0)
                    ++k;
                _objects[iter].x = _getColumnCoords(k);
                _objects[iter].y = _getRowCoords(j);
                _objects[iter].exists = true;
                if (j == 0)
                    _sequence.add(k);
                ++iter;
                if (iter == _n - OTHER_OBJS)
                    break;
            }
        }
    }
}

void Logic::_removeObjects() {
    if (_objects != NULL)
        delete[] _objects;
}

bool Logic::scoreUpdate() {
    if ( _objects[MY_CAR].x > _road.x_left / 2 && _objects[MY_CAR].x + _objects[MY_CAR].width < _road.x_right && _objects[MY_CAR].vel_y > 0 )
        return true;
    return false;
}

void Logic::_setBasicObjPar(const int a, const int type) {
    _objects[a].type = type;
    _objects[a].width = _sizes[type].width;
    _objects[a].height = _sizes[type].height;
}

int Logic::_getColumnCoords(int x) {
    // if it is on the left side of the road
    if (x < _grid.columns / 2)
        return x * _grid.cell_width;
    // else
    x -= _grid.columns / 2;
    return _road.x_right + x * _grid.cell_width;
}

int Logic::_getColumnFromCoords(int x) {
    if (x < _screen_width / 2) {
        return x / _grid.cell_width;
    }
    x = x - _road.x_right;
    x = x / _grid.cell_width;
    x = x + _grid.columns / 2;
    return x;
}

int Logic::_getRowCoords(const int x) {
    return x * _grid.cell_height;
}

void Logic::_generateTreeOrBush(const int a) {
    int x = _sequence.getRandom();
    x = _getColumnCoords(x);
    int type = rand() % (PLANTS_IMGS) + BUSH0;

    _objects[a].type = type;
    _objects[a].x = x;
    _objects[a].y = -_sizes[type].height;
    _objects[a].width = _sizes[type].width;
    _objects[a].height = _sizes[type].height;
    _objects[a].exists = true;
}

void Logic::_generateAlly(const bool generate, const car_generator where) {
    if ( !generate )
        return;
    // if there are already maximum number of cars, don't generate the car
    int sum = 0;
    for (int i = 1; i < 1 + FRIENDLY_CARS; ++i) {
        if ( _objects[i].exists )
            ++sum;
    }
    if ( sum == FRIENDLY_CARS )
        return;
    // if everything is OK, generate the friendly car
    for (int i = 1; i < 1 + FRIENDLY_CARS; ++i) {
        if ( !_objects[i].exists ) {
            _objects[i].exists = true;
            int rand_x = rand() % ( _road.width - _sizes[ALLY_CAR].width) + _road.x_left;
            _objects[i].x = rand_x;
            if (where == TOP) {
                _objects[i].y = -_sizes[ALLY_CAR].height;
                _objects[i].vel_y = MAX_SPEED - 5;
            }
            else {
                _objects[i].y = _screen_height;
                _objects[i].vel_y = 5;
            }
            return;
        }
    }
}

void Logic::_generateEnemy(const bool generate, const car_generator where) {
    if ( !generate )
        return;
    // if there are already maximum number of cars, don't generate the car
    int sum = 0;
    for (int i = 1 + FRIENDLY_CARS; i < 1 + FRIENDLY_CARS + ENEMY_CARS; ++i) {
        if ( _objects[i].exists )
            ++sum;
    }
    if ( sum == ENEMY_CARS )
        return;
    // if everything is OK, generate the enemy car
    for (int i = 1 + FRIENDLY_CARS; i < 1 + FRIENDLY_CARS + ENEMY_CARS; ++i) {
        if ( !_objects[i].exists ) {
            _objects[i].exists = true;
            int rand_x = rand() % ( _road.width - _sizes[ENEMY_CAR].width) + _road.x_left;
            _objects[i].x = rand_x;
            if (where == TOP) {
                _objects[i].y = -_sizes[ENEMY_CAR].height;
                _objects[i].vel_y = MAX_SPEED - 10;
            }
            else {
                _objects[i].y = _screen_height;
                _objects[i].vel_y = 8;
            }
            return;
        }
    }
}

int Logic::numOfObjects() {
    return _n;
}

int Logic::getObjectType(const int a) {
    return _objects[a].type;
}

SDL_Rect Logic::getObjectRect(const int a) {
    SDL_Rect r;
    r.w = _objects[a].width;
    r.h = _objects[a].height;
    r.x = _objects[a].x;
    r.y = _objects[a].y;
    return r;
}

bool Logic::getObjectExistence(const int a) {
    return _objects[a].exists;
}

Road *Logic::getRoad() {
    return &_road;
}

bool Logic::isCollision() {
    return _objects[_n - OTHER_OBJS].exists;
}

void Logic::updateWindow(const int w, const int h) {
    _screen_width = w;
    _screen_height = h;
    _road.x_left = w / 2 - _road.width / 2;
    _road.x_right = w / 2 + _road.width / 2;
}

void Logic::handleInput(const Uint8 *key_states) {
    if ( key_states[SDL_SCANCODE_UP]) {
        _objects[MY_CAR].vel_y += 2;
    }
    if ( key_states[SDL_SCANCODE_DOWN]) {
        _objects[MY_CAR].vel_y -= 1;
    }
    if ( key_states[SDL_SCANCODE_LEFT] ) {
        _objects[MY_CAR].vel_x -= 8;
    }
    if ( key_states[SDL_SCANCODE_RIGHT] ) {
        _objects[MY_CAR].vel_x += 8;
    }
}

void Logic::updateState(const Uint32 delta, const bool add_enemy, const bool add_ally) {
    double delta_time = delta / 10.0;

    // adjust player's car's speed
    _objects[MY_CAR].vel_y -= 1;
    if ( _objects[MY_CAR].vel_y < 0 )
        _objects[MY_CAR].vel_y = 0;
    else if ( _objects[MY_CAR].vel_y > MAX_SPEED )
        _objects[MY_CAR].vel_y = MAX_SPEED;

    // generate enemy and ally cars, if any, and if possible
    car_generator edge;
    if ( _objects[MY_CAR].vel_y < 5 )
        edge = BOTTOM;
    else
        edge = TOP;
    _generateEnemy( add_enemy, edge );
    _generateAlly( add_ally, edge );

    // the speed of trees and bushes is equal to the player's car speed
    for (int i = 1 + ENEMY_CARS + FRIENDLY_CARS; i < _n - OTHER_OBJS; ++i) {
        _objects[i].y += _objects[MY_CAR].vel_y * delta_time;
        // if the tree/bush vanishes, create a new one
        if ( _objects[i].y >= _screen_height ) {
            _generateTreeOrBush(i);
        }
        // if the tree/bush goes out of first cell height, free it so that other trees can be spawned there
        if ( _objects[i].y >= 0 && _objects[i].y - _objects[MY_CAR].vel_y * delta_time < 0)
            _sequence.remove(_getColumnFromCoords(_objects[i].x));
    }

    // update position of enemy and friendly cars
    for (int i = 1; i < 1 + FRIENDLY_CARS + ENEMY_CARS; ++i) {
        if ( !_objects[i].exists )
            continue;
        _objects[i].y += ( _objects[MY_CAR].vel_y - _objects[i].vel_y ) * delta_time;
        if ( _objects[i].y >= _screen_height || _objects[i].y + _objects[i].height <= 0) {
            _objects[i].exists = false;
        }
    }

    // update player's car position
    _objects[MY_CAR].x += _objects[MY_CAR].vel_x * delta_time;
    if ( _objects[MY_CAR].x < _road.x_left - _objects[MY_CAR].width ) {
        _objects[_n - OTHER_OBJS].exists = true;
        _objects[_n - OTHER_OBJS].x = _objects[MY_CAR].x - _sizes[BOOM].width / 2;
        _objects[_n - OTHER_OBJS].y = _objects[MY_CAR].y + _objects[MY_CAR].height / 2 - _sizes[BOOM].height / 2;
    }
    else if ( _objects[MY_CAR].x > _road.x_right ) {
        _objects[_n - OTHER_OBJS].exists = true;
        _objects[_n - OTHER_OBJS].x = _objects[MY_CAR].x + _objects[MY_CAR].width - _sizes[BOOM].width / 2;
        _objects[_n - OTHER_OBJS].y = _objects[MY_CAR].y + _objects[MY_CAR].height / 2 - _sizes[BOOM].height / 2;
    }
    // if ( _objects[MY_CAR].x < 0 )
    //     _objects[MY_CAR].x = 0;
    // if ( _objects[MY_CAR].x + _objects[MY_CAR].width > _screen_width)
    //     _objects[MY_CAR].x = _screen_width - _objects[MY_CAR].width;
    _objects[MY_CAR].vel_x = 0;

    // check for collision
    for ( int i = 1; i < 1 + FRIENDLY_CARS + ENEMY_CARS; ++i ) {
        if ( _checkCollision( &_objects[MY_CAR], &_objects[i] ) ) {
            _objects[_n - OTHER_OBJS].exists = true;
            _objects[_n - OTHER_OBJS].x = ( _objects[MY_CAR].x + _objects[i].x ) / 2;
            _objects[_n - OTHER_OBJS].y = ( _objects[MY_CAR].y + _objects[i].y ) / 2;
        }
    }

}

bool _checkCollision(const Object *obj1, const Object *obj2) {
    int left1, top1, right1, bottom1;
    int left2, top2, right2, bottom2;

    // calculate edges of object 1
    left1 = obj1->x;
    top1 = obj1->y;
    right1 = obj1->x + obj1->width;
    bottom1 = obj1->y + obj1->height;

    // calculate edges of object 2
    left2 = obj2->x;
    top2 = obj2->y;
    right2 = obj2->x + obj2->width;
    bottom2 = obj2->y + obj2->height;

    // if objects do not collide(i.e. they are not hovering over each other), return false
    if ( left1 >= right2 || right1 <= left2 || top1 >= bottom2 || bottom1 <= top2 )
        return false;
    // else, if they collide return true
    return true;
}
