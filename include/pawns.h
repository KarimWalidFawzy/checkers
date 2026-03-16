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
    Pawn(float x, float y, float w, float h, SDL_Color color, const std::string& name)
        : x(x), y(y), width(w), height(h), color(color), name(name) {}

    void draw(SDL_Renderer* renderer) const {
        SDL_FRect rect = { (float)x, (float)y, (float)width, (float)height };
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }

    void move(float dx, float dy) {
        x += dx;
        y += dy;
    }

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }

    std::string getName() const { return name; }
};
#endif // PAWNS_H