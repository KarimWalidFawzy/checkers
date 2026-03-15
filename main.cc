#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>
#include <SDL3_image/SDL_image.h>
#include "include/pawns.h"
#include <vector>
#include <iostream>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE "Checkers"
#define BOARD_SIZE 8
#define SQUARE_SIZE 60

SDL_Texture* checkerboard_tex;
SDL_Texture* pawn_tex;
SDL_Texture* white_pawn_tex;
SDL_Texture* crowned_pawn_tex;
SDL_Texture* crowned_white_pawn_tex;
SDL_Texture* start_screen_tex;

enum GameState { START_SCREEN, PLAYING };
GameState current_state = START_SCREEN;

std::vector<Pawn> pawns;
int selected_pawn = -1;
bool is_white_turn = true;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    /*
    if (!IMG_in) {
        SDL_Log("Couldn't initialize SDL_image: %s", IMG_GetError());
        return SDL_APP_FAILURE;
    }
    */
    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Couldn't create renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    // Load textures
    SDL_Surface* surface;
    surface = SDL_LoadBMP("./checkerboard.bmp");
    if (!surface) {
        SDL_Log("Couldn't load checkerboard.bmp: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    checkerboard_tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    
    surface = SDL_LoadBMP("./pawn.bmp");
    pawn_tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    
    surface = SDL_LoadBMP("./white_pawn.bmp");
    white_pawn_tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    
    surface = SDL_LoadBMP("./crowned_pawn.bmp");
    crowned_pawn_tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    
    surface = SDL_LoadBMP("./crowned_white_pawn.bmp");
    crowned_white_pawn_tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    
    surface = SDL_LoadBMP("./start_screen.bmp");
    start_screen_tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    
    // Initialize pawns
    initialize_pawns();
    
    return SDL_APP_CONTINUE;
}

void initialize_pawns() {
    pawns.clear();
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    
    // Place black pawns (top 3 rows)
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if ((row + col) % 2 == 1) {
                int x = col * SQUARE_SIZE;
                int y = row * SQUARE_SIZE;
                pawns.push_back(Pawn(x, y, SQUARE_SIZE, SQUARE_SIZE, black, "black_pawn"));
            }
        }
    }
    
    // Place white pawns (bottom 3 rows)
    for (int row = 5; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if ((row + col) % 2 == 1) {
                int x = col * SQUARE_SIZE;
                int y = row * SQUARE_SIZE;
                pawns.push_back(Pawn(x, y, SQUARE_SIZE, SQUARE_SIZE, white, "white_pawn"));
            }
        }
    }
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) 
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    
    if (current_state == START_SCREEN && event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        current_state = PLAYING;
        return SDL_APP_CONTINUE;
    }
    
    if (current_state == PLAYING && event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        int mouse_x = event->button.x;
        int mouse_y = event->button.y;
        
        int board_x = mouse_x / SQUARE_SIZE;
        int board_y = mouse_y / SQUARE_SIZE;
        
        if (board_x >= 0 && board_x < BOARD_SIZE && board_y >= 0 && board_y < BOARD_SIZE) {
            // Check if clicking on a pawn
            for (size_t i = 0; i < pawns.size(); i++) {
                int pawn_board_x = pawns[i].getX() / SQUARE_SIZE;
                int pawn_board_y = pawns[i].getY() / SQUARE_SIZE;
                
                if (pawn_board_x == board_x && pawn_board_y == board_y) {
                    if (selected_pawn == -1) {
                        // Select pawn if it's the current player's turn
                        if ((is_white_turn && pawns[i].getName() == "white_pawn") ||
                            (!is_white_turn && pawns[i].getName() == "black_pawn")) {
                            selected_pawn = i;
                        }
                    } else {
                        // Deselect if clicking the same pawn
                        if (selected_pawn == (int)i) {
                            selected_pawn = -1;
                        }
                    }
                    break;
                }
            }
            
            // If a pawn is selected, try to move it
            if (selected_pawn != -1) {
                int selected_board_x = pawns[selected_pawn].getX() / SQUARE_SIZE;
                int selected_board_y = pawns[selected_pawn].getY() / SQUARE_SIZE;
                
                int dx = board_x - selected_board_x;
                int dy = board_y - selected_board_y;
                
                // Basic move validation (diagonal, one square)
                bool valid_move = false;
                if (abs(dx) == 1 && abs(dy) == 1) {
                    // Check direction based on color
                    if ((is_white_turn && dy == -1) || (!is_white_turn && dy == 1)) {
                        valid_move = true;
                    }
                }
                
                if (valid_move) {
                    // Check if destination is empty
                    bool destination_empty = true;
                    for (const auto& pawn : pawns) {
                        int pawn_x = pawn.getX() / SQUARE_SIZE;
                        int pawn_y = pawn.getY() / SQUARE_SIZE;
                        if (pawn_x == board_x && pawn_y == board_y) {
                            destination_empty = false;
                            break;
                        }
                    }
                    
                    if (destination_empty) {
                        pawns[selected_pawn].move(dx * SQUARE_SIZE, dy * SQUARE_SIZE);
                        selected_pawn = -1;
                        is_white_turn = !is_white_turn;
                    }
                }
            }
        }
    }
    
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) 
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    if (current_state == START_SCREEN) {
        SDL_RenderTexture(renderer, start_screen_tex, NULL, NULL);
    } else if (current_state == PLAYING) {
        // Draw checkerboard
        SDL_RenderTexture(renderer, checkerboard_tex, NULL, NULL);
        
        // Draw pawns
        for (const auto& pawn : pawns) {
            SDL_Texture* tex_to_use;
            if (pawn.getName() == "white_pawn") {
                tex_to_use = white_pawn_tex;
            } else {
                tex_to_use = pawn_tex;
            }
            
            SDL_FRect dest = { (float)pawn.getX(), (float)pawn.getY(), (float)pawn.getWidth(), (float)pawn.getHeight() };
            SDL_RenderTexture(renderer, tex_to_use, NULL, &dest);
        }
        
        // Highlight selected pawn
        if (selected_pawn != -1) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 128);
            SDL_FRect highlight = { (float)pawns[selected_pawn].getX(), (float)pawns[selected_pawn].getY(), 
                                   (float)pawns[selected_pawn].getWidth(), (float)pawns[selected_pawn].getHeight() };
            SDL_RenderFillRect(renderer, &highlight);
        }
    }
    
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) 
{
    SDL_DestroyTexture(checkerboard_tex);
    SDL_DestroyTexture(pawn_tex);
    SDL_DestroyTexture(white_pawn_tex);
    SDL_DestroyTexture(crowned_pawn_tex);
    SDL_DestroyTexture(crowned_white_pawn_tex);
    SDL_DestroyTexture(start_screen_tex);
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    
    IMG_Quit();
    SDL_Quit();
}