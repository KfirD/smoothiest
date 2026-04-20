#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <memory>
#include "cartpole.h"
#include "generation.h"

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();
    
    bool init();
    void close();
    
    void render(CartPole* cartpole, int generationNumber, int fitness = 0, int bestFitness = 0, int avgFitness = 0);
    void renderText(int x, int y, const std::string& text, int size);
    
    bool shouldQuit() const { return quit; }
    void handleEvents();
    
    void setDelay(int ms) { delay = ms; }

private:
    void drawCartPole(CartPole* cp);
    void drawBackground();
    
    int width;
    int height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool quit;
    int delay;
};

#endif
