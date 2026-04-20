CXX = g++
CXXFLAGS = -Wall -Wextra -std=gnu++14
CFLAGS = -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib
DEBUG_FLAGS = -g -O0
RELEASE_FLAGS = -O2

RAYLIB_LIBS = -lraylib
SDL_LIBS = -lSDL2 -lSDL2_ttf

SRCDIR = src
BUILDDIR = .

OBJS = $(SRCDIR)/network.o $(SRCDIR)/network_utils.o $(SRCDIR)/population.o $(SRCDIR)/activation.o $(SRCDIR)/organism.o $(SRCDIR)/species.o $(SRCDIR)/generation.o $(SRCDIR)/cartpole.o $(SRCDIR)/neat_evaluator.o $(SRCDIR)/renderer.o $(SRCDIR)/innovation_tracker.o
OBJS3D = $(SRCDIR)/network.o $(SRCDIR)/network_utils.o $(SRCDIR)/population.o $(SRCDIR)/activation.o $(SRCDIR)/organism.o $(SRCDIR)/species.o $(SRCDIR)/generation.o $(SRCDIR)/cartpole3d.o $(SRCDIR)/neat_evaluator.o $(SRCDIR)/renderer3d.o $(SRCDIR)/innovation_tracker.o

.PHONY: all clean debug release help run run2d run3d

all: debug

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(BUILDDIR)/smoothiest $(BUILDDIR)/smoothiest3d

release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(BUILDDIR)/smoothiest $(BUILDDIR)/smoothiest3d

run: run2d

run2d: $(BUILDDIR)/smoothiest
	./smoothiest

run3d: $(BUILDDIR)/smoothiest3d
	./smoothiest3d

$(BUILDDIR)/smoothiest: $(OBJS) $(SRCDIR)/main.o
	$(CXX) $(CXXFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(SDL_LIBS)

$(BUILDDIR)/smoothiest3d: $(OBJS3D) $(SRCDIR)/main3d.o
	$(CXX) $(CXXFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(RAYLIB_LIBS)

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o smoothiest smoothiest3d

help:
	@echo "Targets:"
	@echo "  all      - build both versions (default: debug)"
	@echo "  debug    - build with debug symbols (-g -O0)"
	@echo "  release  - build with optimizations (-O2)"
	@echo "  clean    - remove build artifacts"
	@echo "  run      - run 2D version (same as run2d)"
	@echo "  run2d    - run 2D CartPole"
	@echo "  run3d    - run 3D CartPole"
	@echo "  help     - show this message"
