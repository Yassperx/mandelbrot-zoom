// SDL3 template from https://examples.libsdl.org/SDL3/renderer/04-points/
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "header.hpp"

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {

    SDL_SetAppMetadata("Mandelbrot zoom", "1.0", "com.example.brh");
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Mandelbrot zoom", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
float xx = -2.5, xy = 1.0, yx = -1.5, yy = 1.5;
int good(float x, float y) {
    float zx = 0., zy = 0.;
    for(int j = 0 ; j < iters_n ; j++) {
        float zzx = zx;
        zx = zx*zx - zy*zy + x;
        zy = 2*zzx*zy + y;
        if(zx*zx + zy*zy > 4.) return j;
    }

    return -1;
}

SDL_AppResult SDL_AppIterate(void *appstate) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */

    for(int k = 0 ; k < WINDOW_WIDTH ; k++) {
        for(int j = 0 ; j < WINDOW_HEIGHT ; j++) {
            float x = xx + ((k+1)*(xy-xx))/WINDOW_WIDTH;
            float y = yx + ((j+1)*(yy-yx))/WINDOW_HEIGHT;
            // cout << yay << endl;
            int v = good(x, y);
            int r, g, b;
            if(v == -1) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }

            else {
                v = (255*v)/iters_n;
                SDL_SetRenderDrawColor(renderer, v, v, v, 255);
            }

            SDL_RenderPoint(renderer, (float)k, (float)j);
        }
    }

    xx = (xx*999 + xy)/1000;
    xy = (xy*999 + xx)/1000;
    yx = (yx*999 + yy)/1000;
    yy = (yy*999 + yx)/1000;
    
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {

}