#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>
#include <SDL3_image/SDL_image.h>
/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE "SDL3 Example"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    //SDL_SetAppMetadata("Example Renderer Rectangles", "1.0", "com.example.renderer-rectangles");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    window = SDL_CreateWindow(WINDOW_TITLE,WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer= SDL_CreateRenderer(window, NULL);
    SDL_Surface* bmp = SDL_LoadBMP("./checkerboard.bmp");
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, bmp);
    
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}
SDL_AppResult SDL_AppEvent(void* appstate,SDL_Event* event) 
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* exit the program! */
    }
    return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppIterate(void* appstate) 
{

    return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void* appstate, SDL_AppResult result) 
{
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window) 
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
}
/**
 * Missing SDL 3 Callback Functions
When you use the callback-based application model (#define SDL_MAIN_USE_CALLBACKS 1), your program must define the following functions, in addition to the SDL_AppInit function you've shown:

SDL_AppIterate
This function is the main loop of your application, where you should handle rendering and game logic for each frame. This is the missing function for SDL_AppIterate reference.

SDL_AppEvent
This function handles input events (keyboard, mouse, etc.) from the operating system. This is the missing function for SDL_AppEvent reference.

SDL_AppQuit
This function is called when the application is shutting down. It's used for cleanup (destroying windows, renderers, etc.). This is the missing function for SDL_AppQuit reference.
 */