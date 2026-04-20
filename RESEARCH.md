# Research - NEAT for CartPole

## CartPole Problem

### State Space (4 inputs)
1. Cart position (x): -2.4 to 2.4 (normalized)
2. Cart velocity (x_dot): unbounded (normalized to -3 to 3)
3. Pole angle (theta): -0.2095 to 0.2095 radians (~12 degrees, normalized)
4. Pole angular velocity (theta_dot): unbounded (normalized to -3 to 3)

### Action Space (2 outputs)
- 0: Push cart left
- 1: Push cart right

### Physics (Verified Working)
- Gravity: 9.8 m/s²
- Mass of cart: 1.0 kg
- Mass of pole: 0.1 kg
- Total mass: 1.1 kg
- Length of pole: 0.5 m (half-length)
- Force magnitude: 10.0 N
- Time step (tau): 0.02s
- Episode terminates when:
  - |theta| > 12 degrees (~0.2095 rad)
  - |x| > 2.4
  - Steps > 1000 (max)

### Fitness
- +1 reward for each timestep the pole stays up
- Maximum episode length: 1000 steps

## NEAT Implementation Details

### Network Architecture
- Inputs: 4 (CartPole state)
- Outputs: 2 (left/right action)
- Initial connections: fully connected from inputs to outputs
- Activation: tanh (bounded output -1 to 1)
- Input neurons use override_value (no activation)
- Output neurons use tanh activation

### Evolution Parameters
- Population size: 150
- Elite percentage: 20%
- Compatibility threshold: 2.0
- Mutation rates:
  - Add node: 10%
  - Add connection: 15%
  - Mutate weight: 90%

### Speciation
- Compatibility function: weighted sum of excess, disjoint, and weight difference genes
- Delta = (c1 * E)/N + (c2 * D)/N + c3 * W
- Coefficients: c1=1.0, c2=1.0, c3=0.4
- Shared fitness: fitness / species_size

## Critical Bug Fixed

### Issue
Network was outputting 0 for all inputs regardless of weights.

### Root Cause
In Network constructor, BOTH input and output neurons were created with:
```cpp
Neuron(i, 0.0)  // This sets override_flag=true!
```

Output neurons with override_flag=true always return their override_value (0.0) instead of computing activation from inputs.

### Fix
Input neurons: `Neuron(i, 0.0)` - correct (override_flag=true)
Output neurons: `Neuron(i)` - default constructor sets override_flag=false

## Build Instructions

```bash
cd AI
make clean && make
./smoothiest
```

## Controls
- ESC: Quit
- UP: Speed up simulation
- DOWN: Slow down simulation
