#ifndef PAWNS_H
#define PAWNS_H
#ifndef __cplusplus
    #error "This is a C++ header file"
#endif
#include <SDL3/SDL.h>
#include <vector>
#include <string>
class Pawn {
    private:
    int x, y;               // Position
    int width, height;      // Size
    SDL_Color color;       // Color
    std::string name;      // Name or identifier

    public:
    Pawn(int x, int y, int w, int h, SDL_Color color, const std::string& name)
        : x(x), y(y), width(w), height(h), color(color), name(name) {}

    void draw(SDL_Renderer* renderer) const {
        SDL_Rect rect = { x, y, width, height };
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    std::string getName() const { return name; }
};
#endif // PAWNS_H