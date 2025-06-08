# Makefile for Enhanced Game Engine with SVG Graphics

CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall -Wextra -O2
INCLUDES = -I.
SRCDIR = .
OBJDIR = obj
BINDIR = bin

# Source files
SOURCES = Vector2D.cpp graphics.cpp GameEnvironment.cpp GameObject.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)

# Executables
TARGETS = $(BINDIR)/game_engine_parallel_demo $(BINDIR)/game_engine_with_graphics

# Default target
all: directories $(TARGETS)

# Create directories
directories:
	@mkdir -p $(OBJDIR) $(BINDIR) assets

# Original game engine
$(BINDIR)/game_engine_parallel_demo: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $<

# Enhanced game engine with graphics
$(BINDIR)/game_engine_with_graphics: main_with_graphics.cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(OBJECTS)

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

# Run the enhanced game
run-enhanced: $(BINDIR)/game_engine_with_graphics
	./$(BINDIR)/game_engine_with_graphics

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
