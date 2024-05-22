#include "paths.h"
#include <stdio.h>
#include <string.h>

#ifdef WINDOWS
//#include <direct.h>
#include <windows.h>
const bool windows = true;
#else
#include <unistd.h>
const bool windows = false;
#endif

const int MAX_PATH_SIZE = 1000;

Path::Path () :
    FONT_FILENAME("font.bmp"),
    PLAYERS_CAR ("img/car0.bmp"),
    FRIENDLY_CAR ("img/car1.bmp"),
    HOSTILE_CAR ("img/car2.bmp"),
    BUSHES0 ("img/bush0.bmp"),
    BUSHES1 ("img/bush1.bmp"),
    TREES0 ("img/tree0.bmp"),
    TREES1 ("img/tree1.bmp"),
    TREES2 ("img/tree2.bmp"),
    TREES3 ("img/tree3.bmp"),
    BOOM ("img/explosion.bmp")
{
    _good = false;

    if ( _changeDirectory() )
        _good = true;
}

bool Path::isGood() {
    return _good;
}

char *Path::_my_basename(char *path) {
    char *name = NULL;
    if ( windows )
        name = strrchr(path, '\\');
    else
        name = strrchr(path, '/');
    return name ? name + 1 : path;
}

bool Path::_changeDirectory() {
    char buffer[MAX_PATH_SIZE];
#ifdef WINDOWS
    DWORD length = GetCurrentDirectory(sizeof(buffer), buffer);
    if (length == 0) {
        printf("Failed to get the current working directory\n");
        return false;
    }

    char *dir_name = _my_basename(buffer);

    if (strcmp(dir_name, "build") == 0 || strcmp(dir_name, "src") == 0) {
        if (!SetCurrentDirectory("..")) {
            printf("Failed to set current directory\n");
            return false;
        }
    }
#else
    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        printf("Error: getcwd\n");
        return false;
    }

    char *dir_name = _my_basename(buffer);
    if (strcmp(dir_name, "build") == 0 || strcmp(dir_name, "src") == 0) {
        if (chdir("..") != 0) {
            printf("Error: chdir\n");
            return false;
        }
    }
#endif

    return true;
}
