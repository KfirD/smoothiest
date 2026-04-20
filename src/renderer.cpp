#include "renderer.h"
#include <iostream>
#include <cmath>
#include <algorithm>

Renderer::Renderer(int w, int h) 
    : width(w), height(h), window(nullptr), renderer(nullptr), font(nullptr), quit(false), delay(16) {}

Renderer::~Renderer() {
    close();
}

bool Renderer::init() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    if(TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }
    
    window = SDL_CreateWindow("NEAT CartPole", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                               width, height, SDL_WINDOW_SHOWN);
    if(!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    font = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", 18);
    if(!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
    }
    
    return true;
}

void Renderer::close() {
    if(font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    if(renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if(window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    TTF_Quit();
    SDL_Quit();
}

void Renderer::handleEvents() {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) {
            quit = true;
        }
        else if(e.type == SDL_KEYDOWN) {
            if(e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
            else if(e.key.keysym.sym == SDLK_UP) {
                delay = std::max(0, delay - 5);
            }
            else if(e.key.keysym.sym == SDLK_DOWN) {
                delay = std::min(100, delay + 5);
            }
        }
    }
}

void Renderer::render(CartPole* cartpole, int generationNumber, int fitness, int bestFitness, int avgFitness) {
    handleEvents();
    
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);
    
    drawBackground();
    
    if(cartpole) {
        drawCartPole(cartpole);
    }
    
    if(font) {
        int y = 20;
        int lineHeight = 25;
        
        SDL_Color white = {255, 255, 255, 255};
        
        std::string genText = "Generation: " + std::to_string(generationNumber);
        renderText(20, y, genText, 24);
        y += lineHeight;
        
        if(bestFitness > 0) {
            std::string bestText = "Best Fitness: " + std::to_string(bestFitness);
            renderText(20, y, bestText, 20);
            y += lineHeight;
            
            std::string avgText = "Avg Fitness: " + std::to_string(avgFitness);
            renderText(20, y, avgText, 20);
            y += lineHeight;
        }
        
        std::string stepsText = "Steps: " + std::to_string(cartpole ? cartpole->getSteps() : 0);
        renderText(20, y, stepsText, 20);
        y += lineHeight * 2;
        
        renderText(20, y, "Controls:", 16);
        y += lineHeight;
        renderText(20, y, "UP/DOWN: Speed", 14);
        y += lineHeight;
        renderText(20, y, "ESC: Quit", 14);
    }
    
    SDL_RenderPresent(renderer);
    
    if(delay > 0) {
        SDL_Delay(delay);
    }
}

void Renderer::drawBackground() {
    SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
    SDL_Rect ground = {0, height - 100, width, 100};
    SDL_RenderFillRect(renderer, &ground);
    
    SDL_SetRenderDrawColor(renderer, 50, 50, 60, 255);
    SDL_Rect track = {100, height - 120, width - 200, 20};
    SDL_RenderFillRect(renderer, &track);
}

void Renderer::drawCartPole(CartPole* cp) {
    double scale = 80.0;
    double cartX = width / 2 + cp->getX() * scale;
    double cartY = height - 120;
    
    double poleLen = 150.0;  // Longer pole for harder difficulty
    double poleAngle = cp->getTheta();
    
    double poleEndX = cartX + std::sin(poleAngle) * poleLen;
    double poleEndY = cartY - std::cos(poleAngle) * poleLen;
    
    SDL_SetRenderDrawColor(renderer, 200, 60, 60, 255);
    SDL_Rect cart = {(int)cartX - 30, (int)cartY - 15, 60, 30};
    SDL_RenderFillRect(renderer, &cart);
    
    SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
    SDL_RenderDrawLine(renderer, (int)cartX, (int)cartY, (int)poleEndX, (int)poleEndY);
    
    SDL_SetRenderDrawColor(renderer, 80, 180, 80, 255);
    SDL_Rect poleTip = {(int)poleEndX - 5, (int)poleEndY - 5, 10, 10};
    SDL_RenderFillRect(renderer, &poleTip);
}

void Renderer::renderText(int x, int y, const std::string& text, int size) {
    if(!font) return;
    
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), white);
    if(!surface) return;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(!texture) {
        SDL_FreeSurface(surface);
        return;
    }
    
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
