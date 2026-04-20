#ifndef RENDERER3D_H
#define RENDERER3D_H

#include <raylib.h>
#include <vector>
#include <string>

class Renderer3D {
public:
    Renderer3D(int width, int height);
    ~Renderer3D();
    
    bool init();
    void close();
    
    void render(float cartX, float cartZ, float theta, float phi,
                int generation, int fitness, int bestFitness, int avgFitness);
    
    bool shouldQuit() const;
    void setDelay(int ms) { delay = ms; }

private:
    int width;
    int height;
    bool quit;
    int delay;
};

#endif
