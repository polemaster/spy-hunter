#include "game.h"
#include <time.h>
#include <stdlib.h>

int main() {
    srand( time( NULL ) );
    Game game;

    game.run();

    return 0;
}
