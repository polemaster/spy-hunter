#include "timer.h"
#include <stdlib.h>

void Timer::start() {
    _start_time = SDL_GetTicks();
    _prev_time = SDL_GetTicks();
    _paused = false;
    _ally_random_time = _getRandomTime();
    _enemy_random_time = _getRandomTime();
    _ally_timestamp = _start_time;
    _enemy_timestamp = _start_time;
}

void Timer::pause() {
    if ( !_paused ) {
        _paused = true;
        _temp_time = SDL_GetTicks();
    }
    else {
        _paused = false;
        _start_time += SDL_GetTicks() - _temp_time;
    }
}

void Timer::tick() {
    _curr_time = SDL_GetTicks();
    _delta_time = _curr_time - _prev_time;
    _prev_time = _curr_time;
    if ( _ally_random_time < _curr_time - _ally_timestamp ) {
        _add_ally = true;
        _ally_timestamp = _curr_time;
        _ally_random_time = _getRandomTime();
    }
    else
        _add_ally = false;
    if ( _enemy_random_time < _curr_time - _enemy_timestamp ) {
        _add_enemy = true;
        _enemy_timestamp = _curr_time;
        _enemy_random_time = _getRandomTime();
    }
    else
        _add_enemy = false;
}

Uint32 Timer::_getRandomTime() {
    return ( ( rand() % MAX_GENERATOR_TIME ) + MIN_GENERATOR_TIME ) * 1000;
}

void Timer::wait(int ms) {
    SDL_Delay(ms);
    _start_time += ms;
    _prev_time += ms;
}

bool Timer::addAlly() {
    return _add_ally;
}

bool Timer::addEnemy() {
    return _add_enemy;
}

bool Timer::isPaused() {
    return _paused;
}

Uint32 Timer::getElapsedTime() {
    return SDL_GetTicks() - _start_time;
}

Uint32 Timer::getDeltaTime() {
    return _delta_time;
}
