#ifndef CARTPOLE_H
#define CARTPOLE_H

#include <cmath>
#include <vector>
#include "neat_evaluator.h"

class CartPole : public NEATEvaluator {
public:
    CartPole();
    
    void reset() override;
    void step(int action) override;
    
    std::vector<double> getState() const override;
    bool isDone() const override { return done; }
    int getSteps() const override { return steps; }
    
    double getX() const { return x; }
    double getTheta() const { return theta; }
    
    const char* getName() const override { return "CartPole (2D)"; }
    int getNumInputs() const override { return 4; }
    int getNumOutputs() const override { return 2; }
    
    static constexpr double GRAVITY = 9.8;
    static constexpr double MASS_CART = 1.0;
    static constexpr double MASS_POLE = 0.1;
    static constexpr double TOTAL_MASS = MASS_CART + MASS_POLE;
    static constexpr double POLE_LENGTH = 0.5;
    static constexpr double FORCE_MAG = 10.0;
    static constexpr double TAU = 0.02;
    static constexpr double THETA_THRESHOLD_RADS = 12 * 2 * 3.14159 / 360;
    static constexpr double X_THRESHOLD = 2.4;
    static constexpr int MAX_STEPS = 1000;

private:
    double x;
    double xDot;
    double theta;
    double thetaDot;
    
    bool done;
    int steps;
    
    void computePhysics(int action);
};

#endif
