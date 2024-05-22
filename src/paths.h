#pragma once
#include "constants.h"
// The purpose of this class is to make sure that
// current working directory is the project root directory.
class Path {
public:
    Path();

    const char * FONT_FILENAME;
    const char * PLAYERS_CAR;
    const char * FRIENDLY_CAR;
    const char * HOSTILE_CAR;
    const char * BUSHES0;
    const char * BUSHES1;
    const char * TREES0;
    const char * TREES1;
    const char * TREES2;
    const char * TREES3;
    const char * BOOM;

    bool isGood();
private:
    char *_my_basename(char *path);
    bool _changeDirectory();

    bool _good;
};
