#include "game.h"
#include <stdio.h>
#include <stdlib.h>

Game::Game() : logic(START_SCREEN_WIDTH, START_SCREEN_HEIGHT ){
    renderer = NULL;
    for (int i = 0; i < ALL_IMGS; ++i)
        images[i] = NULL;
    index = 0;
}

Game::~Game() {
    for (int i = 0; i < ALL_IMGS; ++i) {
        if( images[i] != NULL ) {
            SDL_DestroyTexture( images[i] );
            images[i] = NULL;
        }
    }
    font.freeMemory();

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    window.freeMemory();

    SDL_Quit();
}

bool Game::initGame() {
    bool success = true;

    // initialize SDL graphics
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL couldn't initialize: %s\n", SDL_GetError() );
        return false;
    }

    // create window
    if( !window.initialize() ) {
        printf( "Error while creating the window: %s\n", SDL_GetError() );
        return false;
    }

    renderer = window.getNewRenderer();
    if( renderer == NULL ) {
        printf( "Error while creating the renderer: %s\n", SDL_GetError() );
        return false;
    }

	if ( SDL_ShowCursor(SDL_DISABLE) < 0 ) {
        printf( "Couldn't hide the cursor: %s", SDL_GetError() );
        return false;
    }

    if ( !paths.isGood() ) {
        printf( "Failed to load/change paths\n");
        return false;
    }

    safeLoad(&images[MY_CAR], paths.PLAYERS_CAR, success);
    safeLoad(&images[ENEMY_CAR], paths.HOSTILE_CAR, success);
    safeLoad(&images[ALLY_CAR], paths.FRIENDLY_CAR, success);
    safeLoad(&images[BUSH0], paths.BUSHES0, success);
    safeLoad(&images[BUSH1], paths.BUSHES1, success);
    safeLoad(&images[TREE0], paths.TREES0, success);
    safeLoad(&images[TREE1], paths.TREES1, success);
    safeLoad(&images[TREE2], paths.TREES2, success);
    safeLoad(&images[TREE3], paths.TREES3, success);
    safeLoad(&images[BOOM], paths.BOOM, success);

    if ( !font.loadFont( paths.FONT_FILENAME, renderer ) ) {
        printf( "Error while loading font: %s, SDL_Error: %s\n", paths.FONT_FILENAME, SDL_GetError() );
        return false;
    }
    font.setFontSize(FONT_SIZE);

    logic.loadTextureSizes(sizes, index);

    return success;
}

void Game::safeLoad(SDL_Texture **texture, const char *path, bool &success) {
	SDL_Texture *newTexture = NULL;

	// load image to surface from given path
	SDL_Surface *loadedSurface = SDL_LoadBMP( path );
	if( loadedSurface == NULL ) {
		printf( "Unable to load image %s! SDL Error: %s\n", path, SDL_GetError() );
        success = false;
        return;
	}

    newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
    if( newTexture == NULL ) {
        printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
        SDL_FreeSurface( loadedSurface );
        success = false;
    }

    // save image sizes
    sizes[index] = loadedSurface->w;
    sizes[index + 1] = loadedSurface->h;
    index += 2;

    SDL_FreeSurface( loadedSurface );

	*texture = newTexture;
}

void Game::renderMenu() {
    // some constants for settings of the menu
    const int cell_height = ( font.getHeight() / 16 ) + MENU_SPACE; // height of text + space between lines
    const int height = ( font.getHeight() / 16 ) * MENU_ITEMS + (MENU_ITEMS-1) * MENU_SPACE + 2 * MENU_TOP_OFFSET;
    const int width = MENU_WIDTH + 2 * MENU_LEFT_OFFSET;
    const int x = window.getWidth() - width; // x position of the menu border
    const int y = window.getHeight() - height; // y position of the menu border

    // draw the border of the menu
    SDL_Rect rect = { x, y, width, height };
    SDL_SetRenderDrawColor( renderer, MENU_BG_COLOR.r, MENU_BG_COLOR.g, MENU_BG_COLOR.b, MENU_BG_COLOR.a );
    SDL_RenderFillRect( renderer, &rect );

    // set current position
    int curr_x = x + MENU_LEFT_OFFSET; // starting text x position
    int curr_y = y + MENU_TOP_OFFSET; // starting text y postion

    // print actual text of the menu
    printMenuText("Available options:", &curr_x, &curr_y, cell_height );
    printMenuText("ESC: Quit the game", &curr_x, &curr_y, cell_height );
    printMenuText("n: Start new game", &curr_x, &curr_y, cell_height );
    printMenuText("p: Pause/unpause", &curr_x, &curr_y, cell_height );
}

void Game::printMenuText(const char *txt, int *x, int *y, const int height) {
    SDL_Color color = MENU_TXT_COLOR;
    font.renderText(txt, renderer, *x, *y, color);
    *y += height;
}

void Game::renderGameInfo(const int time, const int score) {
    const int height = font.getHeight() / 16 + 2 * INFO_TOP_OFFSET;

    char buffer[MAX_STRING_LENGTH];
    const double d_time = time / 1000.0;
    SDL_Rect game_info_box = { 0, 0, window.getWidth() , height };

    sprintf( buffer, "Score: %d  Time elapsed: %.2lfs", score , d_time );
    SDL_SetRenderDrawColor( renderer, INFO_BG_COLOR.r, INFO_BG_COLOR.g, INFO_BG_COLOR.b, INFO_BG_COLOR.a );
    SDL_RenderFillRect( renderer, &game_info_box );
    font.renderText( buffer, renderer, INFO_LEFT_OFFSET, INFO_TOP_OFFSET, INFO_TXT_COLOR );
}

void Game::renderObjects(const int a, const int b) {
    for (int i = b; i >= a; --i) {
        if ( !logic.getObjectExistence(i) )
            continue;
        SDL_Rect rect = logic.getObjectRect(i);
        SDL_RenderCopy( renderer, images[logic.getObjectType(i)], NULL, &rect);
    }
}

void Game::renderLegend() {
    font.setFontSize(LEGEND_FONT_SIZE);
    // calculate legend rectangle
    int height = 0, width, x, y;
    if ( height < sizes[3] )
        height = sizes[3];
    if ( height < sizes[5] )
        height = sizes[5];
    height += 3*LEGEND_SPACE;
    height *= LEGEND_SCALE;
    height += font.getHeight() / 16;
    width = ( sizes[2] + sizes[4] + LEGEND_LEFT_OFFSET ) * LEGEND_SCALE;
    x = 0;
    y = window.getHeight() - height;
    SDL_Rect rect = { x, y, width, height };
    SDL_SetRenderDrawColor( renderer, LEGEND_BG_COLOR.r, LEGEND_BG_COLOR.g, LEGEND_BG_COLOR.b, LEGEND_BG_COLOR.a );
    SDL_RenderFillRect( renderer, &rect );

    x += LEGEND_LEFT_OFFSET * LEGEND_SCALE;
    y += LEGEND_SPACE * LEGEND_SCALE;

    // write text
    font.renderText("Ally", renderer, x, y, LEGEND_TXT_COLOR);
    font.renderText("Enemy", renderer, x + sizes[2] * LEGEND_SCALE, y, LEGEND_TXT_COLOR);

    y += font.getHeight() / 16 + LEGEND_SCALE * LEGEND_SPACE;

    // draw images
    SDL_Rect ally_car_rect = { x, y, (int)(sizes[2] * LEGEND_SCALE), (int)(sizes[3] * LEGEND_SCALE) };
    SDL_RenderCopy( renderer, images[ALLY_CAR], NULL, &ally_car_rect );
    SDL_Rect enemy_car_rect = { x + (int)(sizes[2] * LEGEND_SCALE), y, (int)(sizes[4] * LEGEND_SCALE), (int)(sizes[5] * LEGEND_SCALE) };
    SDL_RenderCopy( renderer, images[ENEMY_CAR], NULL, &enemy_car_rect );

    font.setFontSize(FONT_SIZE);
}

void Game::renderRoad() {
    Road *road = logic.getRoad();
    SDL_Rect rect = { road->x_left, 0, road->width, window.getHeight() };
    SDL_SetRenderDrawColor( renderer, road->color.r, road->color.g, road->color.b, road->color.a );
    SDL_RenderFillRect( renderer, &rect );
}

void Game::run() {
    if (!initGame()) {
        printf("Initialization of the game has failed\n");
        return;
    }

    bool quit = false;
    SDL_Event event;
    int score = 0;
    Timer timer;

    timer.start();
    logic.newGame();

    while (!quit) {
        // if there was collision last frame, wait 1s and create a new game
        if ( logic.isCollision() ) {
            timer.wait(1000);
            logic.newGame();
        }
        // loop that handles events
        while( SDL_PollEvent( &event ) != 0 ) {
            if( event.type == SDL_QUIT )
                quit = true;
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_n:
                        logic.newGame();
                        timer.start();
                        score = 0;
                        break;
                    case SDLK_p:
                        timer.pause();
                        break;
                }
            }
            window.handleEvent( &event, renderer);
        }

        timer.tick();

        if( !window.isMinimized() && (!timer.isPaused()) ) {
            // clear screen
            SDL_SetRenderDrawColor( renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a );
            SDL_RenderClear( renderer );

            // Update window size and width in logic
            logic.updateWindow( window.getWidth(), window.getHeight() );

            // get keyboard state
            const Uint8* keyboard_state = SDL_GetKeyboardState( NULL );

            // handle user input
            logic.handleInput( keyboard_state );

            // update game state
            logic.updateState( timer.getDeltaTime(), timer.addEnemy(), timer.addAlly() );

            // update score
            if ( logic.scoreUpdate() )
                ++score;

            // render road
            renderRoad();
            // render objects
            renderObjects( 0, logic.numOfObjects() - OTHER_OBJS - 1 );
            // render effects
            renderObjects( logic.numOfObjects() - OTHER_OBJS, logic.numOfObjects() - 1 );

            // render other elements
            renderMenu();
            renderGameInfo( timer.getElapsedTime(), score );
            renderLegend();

            //Update screen
            SDL_RenderPresent( renderer );
        }

    }
}
