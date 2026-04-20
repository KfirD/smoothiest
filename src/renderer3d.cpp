#include "renderer3d.h"

Renderer3D::Renderer3D(int w, int h) : width(w), height(h), quit(false), delay(16) {}

Renderer3D::~Renderer3D() {
    close();
}

bool Renderer3D::init() {
    InitWindow(width, height, "NEAT CartPole - 3D");
    SetTargetFPS(60);
    return true;
}

void Renderer3D::close() {
    CloseWindow();
}

bool Renderer3D::shouldQuit() const {
    return WindowShouldClose() || quit;
}

void Renderer3D::render(float cartX, float cartZ, float theta, float phi,
                        int generation, int fitness, int bestFitness, int avgFitness) {
    if(WindowShouldClose()) {
        quit = true;
        return;
    }
    
    // Handle keyboard
    if(IsKeyDown(KEY_ESCAPE)) quit = true;
    if(IsKeyDown(KEY_UP)) delay = std::max(0, delay - 2);
    if(IsKeyDown(KEY_DOWN)) delay = std::min(50, delay + 2);
    
    BeginDrawing();
    ClearBackground({30, 30, 40, 255});
    
    // Draw 3D scene
    Camera3D camera = { {15.0f, 10.0f, 15.0f}, {0, 0, 0}, {0, 1, 0}, 45.0f, 0 };
    
    BeginMode3D(camera);
    
    // Draw ground plane
    DrawPlane({0, -1.5f, 0}, {20, 20}, {50, 50, 60, 255});
    
    // Draw X-axis track
    DrawLine3D({-10, -1.0f, 0}, {10, -1.0f, 0}, {100, 100, 100, 255});
    // Draw Z-axis track
    DrawLine3D({0, -1.0f, -10}, {0, -1.0f, 10}, {100, 100, 100, 255});
    
    // Draw cart at (cartX, cartZ)
    Vector3 cartPos = {cartX, -1.0f, cartZ};
    DrawCube(cartPos, 2.0f, 0.5f, 1.0f, {200, 60, 60, 255});
    DrawCubeWires(cartPos, 2.0f, 0.5f, 1.0f, {255, 100, 100, 255});
    
    // Draw single pole with 3D tilt (theta = X-tilt, phi = Z-tilt)
    float poleLength = 3.0f;
    
    // Calculate pole end position in 3D
    // theta tilts in X direction, phi tilts in Z direction
    Vector3 poleEnd = {
        cartX + sinf(theta) * poleLength,
        -1.0f + cosf(theta) * poleLength * cosf(phi),
        cartZ + sinf(phi) * poleLength
    };
    
    // Draw pole as thick line
    DrawLine3D(cartPos, poleEnd, {100, 200, 100, 255});
    DrawSphere(poleEnd, 0.25f, {80, 180, 80, 255});
    
    // Draw pivot point
    DrawSphere(cartPos, 0.15f, {150, 150, 150, 255});
    
    EndMode3D();
    
    // Draw UI
    DrawText(TextFormat("Generation: %d", generation), 20, 20, 24, WHITE);
    DrawText(TextFormat("Best Fitness: %d", bestFitness), 20, 50, 20, GREEN);
    DrawText(TextFormat("Avg Fitness: %d", avgFitness), 20, 75, 20, GRAY);
    DrawText(TextFormat("Steps: %d", fitness), 20, 100, 20, YELLOW);
    DrawText(TextFormat("Cart X: %.2f, Z: %.2f", cartX, cartZ), 20, 125, 16, LIGHTGRAY);
    DrawText(TextFormat("Theta: %.2f, Phi: %.2f", theta, phi), 20, 145, 16, LIGHTGRAY);
    
    DrawText("Controls: ESC=quit, UP/DOWN=speed", 20, height - 40, 16, GRAY);
    
    EndDrawing();
    
    if(delay > 0) {
        WaitTime(delay / 1000.0);
    }
}
