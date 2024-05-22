#include "window.h"
#include <stdio.h>

Window::Window() {
    _window = NULL;
}

bool Window::initialize() {
	_window = SDL_CreateWindow( WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, START_SCREEN_WIDTH, START_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE );
	if( _window == NULL )
        return false;
    _width = START_SCREEN_WIDTH;
    _height = START_SCREEN_HEIGHT;
    _minimized = false;
    return true;
}

void Window::freeMemory() {
    if ( _window != NULL )
        SDL_DestroyWindow( _window );
}

void Window::handleEvent(const SDL_Event *e, SDL_Renderer *r) {
    if( e->type == SDL_WINDOWEVENT ) {
        switch( e->window.event ) {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                _width = e->window.data1;
                _height = e->window.data2;
                if ( _width < MIN_SCREEN_WIDTH ) {
                    _width = MIN_SCREEN_WIDTH;
                    SDL_SetWindowSize( _window, _width, _height );
                }
                if ( _height < MIN_SCREEN_HEIGHT ) {
                    _height = MIN_SCREEN_HEIGHT;
                    SDL_SetWindowSize( _window, _width, _height );
                }
                SDL_RenderPresent( r );
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                SDL_RenderPresent( r );
                break;
            case SDL_WINDOWEVENT_RESTORED:
                _minimized = false;
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                _minimized = true;
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                _minimized = false;
                break;
        }
    }
}

bool Window::isGood() {
    return _good;
}

bool Window::isMinimized() {
    return _minimized;
}

SDL_Renderer *Window::getNewRenderer() {
    if ( !VSYNC )
        return SDL_CreateRenderer( _window, -1, SDL_RENDERER_ACCELERATED );
    return SDL_CreateRenderer( _window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC  );
}

int Window::getWidth() {
    return _width;
}

int Window::getHeight() {
    return _height;
}
