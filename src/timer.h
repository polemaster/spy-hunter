#pragma once
#include "constants.h"

class Timer {
public:
    void start();
    void pause();
    bool isPaused();
    void tick();
    void wait(const int ms);
    Uint32 getElapsedTime();
    Uint32 getDeltaTime();
    bool addEnemy();
    bool addAlly();
private:
    Uint32 _start_time;
    Uint32 _prev_time;
    Uint32 _curr_time;
    Uint32 _delta_time;
    Uint32 _temp_time;
    bool _paused;
    Uint32 _ally_random_time, _enemy_random_time;
    Uint32 _ally_timestamp, _enemy_timestamp;
    bool _add_ally, _add_enemy;

    Uint32 _getRandomTime();
};
