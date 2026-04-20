#include "cartpole.h"
#include <random>
#include <cmath>

static std::random_device rd;
static std::mt19937 gen(rd());

CartPole::CartPole() : x(0), xDot(0), theta(0), thetaDot(0), done(false), steps(0) {}

void CartPole::reset() {
    std::uniform_real_distribution<> dis(-0.05, 0.05);
    x = dis(gen);
    xDot = dis(gen);
    theta = dis(gen);
    thetaDot = dis(gen);
    
    done = false;
    steps = 0;
}

void CartPole::step(int action) {
    if(done) return;
    
    computePhysics(action);
    
    steps++;
    
    if(std::abs(x) > X_THRESHOLD || std::abs(theta) > THETA_THRESHOLD_RADS) {
        done = true;
    }
    if(steps >= MAX_STEPS) {
        done = true;
    }
}

void CartPole::computePhysics(int action) {
    double force = (action == 1) ? FORCE_MAG : -FORCE_MAG;
    
    double costheta = std::cos(theta);
    double sintheta = std::sin(theta);
    
    double temp = (force + MASS_POLE * POLE_LENGTH * thetaDot * thetaDot * sintheta) / TOTAL_MASS;
    double thetaAcc = (GRAVITY * sintheta - costheta * temp) / 
        (POLE_LENGTH * (4.0/3.0 - MASS_POLE * costheta * costheta / TOTAL_MASS));
    double xAcc = temp - MASS_POLE * POLE_LENGTH * thetaAcc * costheta / TOTAL_MASS;
    
    x += TAU * xDot;
    xDot += TAU * xAcc;
    theta += TAU * thetaDot;
    thetaDot += TAU * thetaAcc;
    
    xDot = std::max(-10.0, std::min(10.0, xDot));
    thetaDot = std::max(-10.0, std::min(10.0, thetaDot));
}

std::vector<double> CartPole::getState() const {
    std::vector<double> state(4);
    state[0] = x / X_THRESHOLD;
    state[1] = xDot / 3.0;
    state[2] = theta / THETA_THRESHOLD_RADS;
    state[3] = thetaDot / 3.0;
    return state;
}
