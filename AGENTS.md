# AGENTS.md - Smoothiest AI Codebase

## Project Overview

This is a neuroevolution project implementing NEAT (Neuroevolution of Augmenting Topologies). The codebase is in the `AI/` directory and contains:
- Neural network implementation (`network.cpp/h`)
- Population/species management (`population.cpp/h`, `species.cpp/h`)
- Generation evolution (`generation.cpp/h`)
- Innovation tracking for historical marking
- Activation functions (`activation.cpp/h`)

## Build Commands

```bash
# Build the project
cd AI && make

# Run the executable
cd AI && ./smoothiest

# Clean build artifacts
cd AI && make clean
```

## Code Style

### Formatting
- Use **2 spaces** for indentation (not tabs)
- Opening brace on same line: `void foo() {`
- Max line length: ~100 characters
- One space after keywords: `if (`, `for (`, `while (`

### Naming Conventions
- **Classes**: PascalCase (`Network`, `Connection`, `Generation`)
- **Methods/Functions**: camelCase (`getConnections()`, `addNewNeuron()`)
- **Variables**: camelCase (`numInputs`, `fitnessValue`)
- **Constants**: UPPER_SNAKE_CASE or k-prefixed (`const double elitePercentage = 0.40;`)
- **Files**: lowercase with underscores (`network_utils.cpp`, `generation.h`)

### Types
- Use `std::vector` for dynamic arrays
- Use `std::unordered_map` for hash maps
- Use `double` for floating-point numbers
- Use `int` for integers
- Avoid raw pointers where possible; prefer references

### Imports
- Standard library first, then project headers
- Use `#include "filename.h"` for local headers
- Example:
  ```cpp
  #include <iostream>
  #include <vector>
  #include <unordered_map>
  
  #include "network.h"
  #include "generation.h"
  ```

### Class Structure
```cpp
class MyClass {
public:
    // Public methods and data
private:
    // Private methods and data
};
```

### Error Handling
- Use `std::cerr` for error messages
- Throw `std::exception` or subclasses for exceptional cases
- Check preconditions in debug builds with assertions

### Comments
- Use comments sparingly to explain *why*, not *what*
- Document complex algorithms with brief explanation
- TODO comments should include brief description: `// TODO: implement X`

## Testing

No formal test framework currently configured. To add tests:
1. Consider using [Catch2](https://github.com/catchorg/Catch2) header-only testing
2. Create `test/` directory for test files
3. Update makefile to build tests

## Key Classes

| Class | Purpose |
|-------|---------|
| `Network` | Neural network with nodes and connections |
| `Connection` | Weighted connection between neurons |
| `Neuron` | Individual neuron with activation |
| `Population` | Collection of organisms |
| `Species` | Group of similar organisms |
| `Generation` | One iteration of evolution |
| `InnovationTracker` | Tracks historical markings for crossover |

## Development Workflow

1. Make changes in feature branches
2. Test locally with `make && ./smoothiest`
3. Commit using Conventional Commits:
   - `feat:` new feature
   - `fix:` bug fix
   - `refactor:` code restructuring
   - `test:` adding tests
   - `docs:` documentation

## Performance Considerations

- Parallel evaluation is used in `generation.cpp` with `std::thread`
- Use references (`&`) to avoid copying large objects
- Consider using `const` for methods that don't modify state

## Common Issues

- **Segfaults**: Check neuron/connection indices are valid
- **Evolution stagnation**: Adjust species compatibility threshold
- **Slow training**: Increase parallelism, reduce population size for testing
