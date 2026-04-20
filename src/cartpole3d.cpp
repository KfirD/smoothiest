#include "cartpole3d.h"
#include <random>
#include <cmath>

static std::random_device rd3d;
static std::mt19937 gen3d(rd3d());

CartPole3D::CartPole3D() : x(0), xDot(0), z(0), zDot(0), theta(0), thetaDot(0), phi(0), phiDot(0), done(false), steps(0) {}

void CartPole3D::reset() {
    std::uniform_real_distribution<> dis(-0.05, 0.05);
    x = dis(gen3d);
    xDot = dis(gen3d);
    z = dis(gen3d);
    zDot = dis(gen3d);
    theta = dis(gen3d);
    thetaDot = dis(gen3d);
    phi = dis(gen3d);
    phiDot = dis(gen3d);
    
    done = false;
    steps = 0;
}

void CartPole3D::step(int action) {
    if(done) return;
    
    computePhysics(action);
    
    steps++;
    
    if(std::abs(x) > X_THRESHOLD || std::abs(z) > Z_THRESHOLD || 
       std::abs(theta) > THETA_THRESHOLD_RADS || std::abs(phi) > PHI_THRESHOLD_RADS) {
        done = true;
    }
    if(steps >= MAX_STEPS) {
        done = true;
    }
}

void CartPole3D::computePhysics(int action) {
    double forceX = 0, forceZ = 0;
    
    // Action: 0=left, 1=right, 2=forward, 3=back
    if(action == 0) forceX = -FORCE_MAG;
    else if(action == 1) forceX = FORCE_MAG;
    else if(action == 2) forceZ = -FORCE_MAG;
    else if(action == 3) forceZ = FORCE_MAG;
    
    // Simplified physics - decoupled X and Z motion
    double cosTheta = std::cos(theta);
    double sinTheta = std::sin(theta);
    double cosPhi = std::cos(phi);
    double sinPhi = std::sin(phi);
    
    // Theta dynamics (forward/back tilt)
    double tempTheta = (forceX + MASS_POLE * POLE_LENGTH * thetaDot * thetaDot * sinTheta) / TOTAL_MASS;
    double thetaAcc = (GRAVITY * sinTheta - cosTheta * tempTheta) / 
        (POLE_LENGTH * (4.0/3.0 - MASS_POLE * cosTheta * cosTheta / TOTAL_MASS));
    
    // Phi dynamics (side-to-side tilt)  
    double tempPhi = (forceZ + MASS_POLE * POLE_LENGTH * phiDot * phiDot * sinPhi) / TOTAL_MASS;
    double phiAcc = (GRAVITY * sinPhi - cosPhi * tempPhi) / 
        (POLE_LENGTH * (4.0/3.0 - MASS_POLE * cosPhi * cosPhi / TOTAL_MASS));
    
    double xAcc = forceX / TOTAL_MASS;
    double zAcc = forceZ / TOTAL_MASS;
    
    x += TAU * xDot;
    xDot += TAU * xAcc;
    z += TAU * zDot;
    zDot += TAU * zAcc;
    theta += TAU * thetaDot;
    thetaDot += TAU * thetaAcc;
    phi += TAU * phiDot;
    phiDot += TAU * phiAcc;
    
    xDot = std::max(-10.0, std::min(10.0, xDot));
    zDot = std::max(-10.0, std::min(10.0, zDot));
    thetaDot = std::max(-10.0, std::min(10.0, thetaDot));
    phiDot = std::max(-10.0, std::min(10.0, phiDot));
}

std::vector<double> CartPole3D::getState() const {
    std::vector<double> state(6);
    state[0] = x / X_THRESHOLD;
    state[1] = xDot / 3.0;
    state[2] = z / Z_THRESHOLD;
    state[3] = zDot / 3.0;
    state[4] = theta / THETA_THRESHOLD_RADS;
    state[5] = phi / PHI_THRESHOLD_RADS;
    return state;
}
