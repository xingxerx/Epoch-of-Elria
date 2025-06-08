# Makefile for Enhanced Game Engine with SVG Graphics

CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall -Wextra -O2
INCLUDES = -I.
SRCDIR = .
OBJDIR = obj
BINDIR = bin

# Source files
SOURCES = Vector2D.cpp graphics.cpp GameEnvironment.cpp GameObject.cpp GameObject3D.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)

# 3D Game sources
SOURCES_3D = GameObject3D.cpp
OBJECTS_3D = $(SOURCES_3D:%.cpp=$(OBJDIR)/%.o)

# Executables
TARGETS = $(BINDIR)/game_engine_parallel_demo $(BINDIR)/game_engine_simple_graphics $(BINDIR)/game_walkable_simple $(BINDIR)/game_3d_openworld $(BINDIR)/rpg_characters $(BINDIR)/dream_weaver_complete

# Default target
all: directories $(TARGETS)

# Create directories
directories:
	@mkdir -p $(OBJDIR) $(BINDIR) assets

# Original game engine
$(BINDIR)/game_engine_parallel_demo: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $<

# Simple game engine with SVG graphics
$(BINDIR)/game_engine_simple_graphics: main_simple_graphics.cpp $(OBJDIR)/Vector2D.o $(OBJDIR)/graphics.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(OBJDIR)/Vector2D.o $(OBJDIR)/graphics.o

# Walkable game with player controls
$(BINDIR)/game_walkable_simple: main_walkable_simple.cpp $(OBJDIR)/Vector2D.o $(OBJDIR)/graphics.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(OBJDIR)/Vector2D.o $(OBJDIR)/graphics.o

# 3D Open World game
$(BINDIR)/game_3d_openworld: main_3d_openworld.cpp $(OBJDIR)/GameObject3D.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(OBJDIR)/GameObject3D.o

# Dream Weaver's Heart RPG Characters
$(BINDIR)/rpg_characters: main_rpg_characters.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $<

# Complete Dream Weaver's Heart Game
$(BINDIR)/dream_weaver_complete: main_dream_weaver_complete.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $<

# Object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

# Clean build files
clean:
	rm -rf $(OBJDIR) $(BINDIR) *.html

# Clean everything including generated assets
distclean: clean
	rm -rf assets

# Run the original game
run-original: $(BINDIR)/game_engine_parallel_demo
	./$(BINDIR)/game_engine_parallel_demo

# Run the simple graphics game
run-simple: $(BINDIR)/game_engine_simple_graphics
	./$(BINDIR)/game_engine_simple_graphics

# Run the 3D open world game
run-3d: $(BINDIR)/game_3d_openworld
	./$(BINDIR)/game_3d_openworld

# Run the RPG characters demo
run-rpg: $(BINDIR)/rpg_characters
	./$(BINDIR)/rpg_characters

# Run the complete Dream Weaver's Heart game
run-dream-weaver: $(BINDIR)/dream_weaver_complete
	./$(BINDIR)/dream_weaver_complete

# Quick build and run the complete game
game: $(BINDIR)/dream_weaver_complete
	@echo "The Dream Weaver's Heart game compiled successfully!"
	@echo "Starting the game..."
	./$(BINDIR)/dream_weaver_complete

# Create sample assets
assets: directories
	@echo "Sample SVG assets already created in assets/ directory"

# Help
help:
	@echo "Available targets:"
	@echo "  all           - Build all executables"
	@echo "  clean         - Remove build files"
	@echo "  distclean     - Remove all generated files"
	@echo "  run-original  - Run the original parallel game engine"
	@echo "  run-enhanced  - Run the enhanced game engine with graphics"
	@echo "  assets        - Create sample SVG assets"
	@echo "  help          - Show this help message"

# Dependencies
$(OBJDIR)/graphics.o: graphics.cpp graphics.h Vector2D.h
$(OBJDIR)/GameEnvironment.o: GameEnvironment.cpp GameEnvironment.h Vector2D.h graphics.h GameObject.h
$(OBJDIR)/GameObject.o: GameObject.cpp GameObject.h Vector2D.h graphics.h GameEnvironment.h
$(OBJDIR)/Vector2D.o: Vector2D.cpp Vector2D.h

.PHONY: all clean distclean run-original run-enhanced assets help directories
