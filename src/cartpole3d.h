#ifndef CARTPOLE3D_H
#define CARTPOLE3D_H

#include <cmath>
#include <vector>
#include "neat_evaluator.h"

class CartPole3D : public NEATEvaluator {
public:
    CartPole3D();
    
    void reset() override;
    void step(int action) override;
    
    std::vector<double> getState() const override;
    bool isDone() const override { return done; }
    int getSteps() const override { return steps; }
    
    double getX() const { return x; }
    double getZ() const { return z; }
    double getTheta() const { return theta; }
    double getPhi() const { return phi; }  // 2nd angle for 2D movement
    
    const char* getName() const override { return "CartPole (3D - 2D movement)"; }
    int getNumInputs() const override { return 6; }
    int getNumOutputs() const override { return 4; }  // 4 actions: left/right/forward/back
    
    static constexpr double GRAVITY = 9.8;
    static constexpr double MASS_CART = 1.0;
    static constexpr double MASS_POLE = 0.1;
    static constexpr double TOTAL_MASS = MASS_CART + MASS_POLE;
    static constexpr double POLE_LENGTH = 1.0;
    static constexpr double FORCE_MAG = 15.0;
    static constexpr double TAU = 0.02;
    static constexpr double THETA_THRESHOLD_RADS = 36 * 2 * 3.14159 / 360;
    static constexpr double PHI_THRESHOLD_RADS = 36 * 2 * 3.14159 / 360;
    static constexpr double X_THRESHOLD = 2.0;
    static constexpr double Z_THRESHOLD = 2.0;
    static constexpr int MAX_STEPS = 1000;

private:
    double x, xDot;
    double z, zDot;
    double theta, thetaDot;  // Forward/back tilt
    double phi, phiDot;      // Side-to-side tilt
    
    bool done;
    int steps;
    
    void computePhysics(int action);
};

#endif
