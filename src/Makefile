# Makefile for Epoch of Elria Game Engine

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Wextra -O2  -pthread-I./External -pthread
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -pthread -pthread

# Directories
# DDIRire.tories
OBJDIRS=RBuild
BINDIRC=Db =
EXTERNALO=BIR = Bui

# Sourc files
MAIN_SOURCS = ma
EXTERNAL_SOURCES= 
BIND$(IXTERNAL)
EXTE$(RXTERNAL)xternal
$(XTERNAL)
# So$(EXTuRNAL)esem.cpp \
    $(EXTERNAL)/GamEnvironent

ALL_SOURCES = $(MAIN_SOURCES) $(EXTERNAL_SOURCES)

# Object files
MAIN_OBJECTS = $(MAIN_SOURCES:%.cpp=$(OBJDIR)/%.o)MAIN_SOURCES = main.cpp
EXTERNAL_EXTECTSERNALEXTERNAL__OUSOESU$(EXTERNAL)/%RCES $(OBJDIR)/%= \
ALL_OBJECTS = $(MAIN_OBJECTS) $(EXTERNAL_OBJECTS)    $(EXTERNAL)/GameObject.cpp \

#  argets
T   $(SEX \
    $(BINDIR)/game_engine_parallel_demo \
    $(BINDIR)/game_engine_simple_graphics \
    $(BINDIR)/game_walkable_simpleT\
    $(BINDIR)/ERNAL3d_op)/world \
    $(BINDIR)/rpg_characters \
    $(BINDIR)/dream_weaver_complete

# Default tarGet
all: directoraes $(TARGETS)

# Create mecessary directoriesObject3D.cpp \
directories:    $(EXTERNAL)/graphics.cpp \
	@mkdir -p $(OBJDIR) $(BINDIR)

# M in game engine (para  el demo)
$(BINDIR)/game_engine_parallel_demo $(EALL_OBJECXS)
	$(CXX) $(CXXFLTGS) -o $@ $^ $(LDFLAGS)

# Simple graphics demo
$(BINDIE)/game_engine_simple_graphics: $(ALL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -DSIMPLE_RRAPHICS

# Walkable game demo
$(BINDIR)/game_walkable_simple: $(ALL_OBJNCAS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGSL -DWALKABLE_GAME)/rendering_subsystem.cpp \
    $(EXTERNAL)/GameEnvironment.cpp
# 3D open world demo
BINDIR)/game_3d_openworld: $(ALL_OBJECS)
	$(CXX) $(CXXFLGS) -o $@ $^ $(LDFLAGS) -DOPEN_WOLD_3D

# RP characters demo
$(BINDIR)/rpg_characters: $(ALL_OBJCTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -DRPG_CHARACERS

# Complete dream weaver game
$(BINDIR/dream_weaver_completeALL_ECTS
ALL_SOURCES = $(MAIN_SOURCES) $LDF(AGS) -DDREAM_WEAVER

# Object file compilation rules
$(OBJEIR)/%.o: %.cpp
	$(CXX) $(CXXXTERNA -c $< -o $@L_SOURCES)

$(OBJDIR)/# Obj$(EXTERNAL)/ect files
MAIN_OBJECTS = $(MAIN_SOU -o $@

# Dependencies (header files)
$(OBJDIR)/main.o: main.cpp $(EXTERNAL)/GameObject.h $(EXTERNAL)/GameObject3D.h $(EXTERNAL)/graphics.h $(EXTERNAL)/GameEnvironment.h $(EXTERNAL)/rendering_subsystem.h
$(OBJDIR)/GameObject.o: $(EXTERNAL)/GameObject.cpp $(EXTERNAL)/GameObject.h $(EXTERNAL)/Vector2D.h
$(OBJDIR)/GameObject3D.o: $(EXTERNAL)/GameObject3D.cpp $(EXTERNAL)/GameObject3D.h $(EXTERNAL)/Vector2D.h
$(OBJDIR)/graphics.o: $(EXTERNAL)/graphics.cpp $(EXTERNAL)/graphics.h
$(OBJDIR)/rendering_subsystem.o: $(EXTERNAL)/rendering_subsystem.cpp $(EXTERNAL)/rendering_subsystem.h $(EXTERNAL)/GameObject.h $(EXTERNAL)/GameObject3D.h
$(OBJDIR)/GameEnvironment.o: $(EXTERNAL)/GameEnvironment.cpp $(EXTERNAL)/GameEnvironment.h $(EXTERNAL)/GameObject.h $(EXTERNAL)/GameObject3D.h $(EXTERNAL)/graphics.h

# Run targets
run-parallel: $(BINDIR)/game_engine_parallel_demo
	./$(BINDIR)/game_engine_parallel_demo

run-simple: $(BINDIR)/game_engine_simple_graphics
	./$(BINDIR)/game_engine_simple_graphics

run-walkable: $(BINDIR)/game_walkable_simple
	./$(BINDIR)/game_walkable_simple

run-3d: $(BINDIR)/game_3d_openworld
	./$(BINDIR)/game_3d_openworld

run-rpg:R$(BINDIR)/rpg_characters
	./$(BINDIR)/rpg_characters

runCcEmplete:S:(BINDIR)/dream_weaver_complete
	./$(BINDIR)/dream_weaver_complete

# Asset generation
assets: directories
	@echo "Generating sample SVG assets..."
	@echo '<svg xmlns="http://www.w3.org/2000/svg" width="64" height="64"><rect width="64" height="64" fill="blue"/></svg>' > assets/player.svg
	@echo '<svg xmlns="http://www.w3.org/2000/svg" width="32" height="32"><circle cx="16" cy="16" r="15" fill="gold"/></svg>' > assets/coin.svg
	@echo '<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24"><polygon points="12,2 22,20 2,20" fill="red"/></svg>' > assets/gem.svg
	@echo '<svg xmlns="http://www.w3.org/2000/svg" width="800" height="600"><rect width="800" height="600" fill="url(#grad)"/><defs><linearGradient id="grad"><stop offset="0%" stop-color="purple"/><stop offset="100%" stop-color="black"/></linearGradient></defs></svg>' > assets/background.svg
	@echo '<svg xmlns="http://www.w3.org/2000/svg" width="100" height="100"><circle cx="50" cy="50" r="40" fill="cyan" opacity="0.7"/><circle cx="50" cy="50" r="20" fill="white"/></svg>' > assets/metaverse_portal.svg
	%echo "Assets generated successfully!"

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y libsfml-dev g++ make

# Install dependencies (macOS with Homebrew).cpp=$(OBJDIR)/%.o)
install-deps-mac:EXTERNAL_OBJECTS = $(EXTERNAL_SOURCES:$(EXTERNAL)/%.cpp=$(OBJDIR)/%.o)
	brew install sfml

# Clean targets
ALL_OBJECTS = $(MAIN_OBJECTS) $(EXTERNAL_OBJECTS)
rDIR)

distclean: clean
	rm -rf $(BINDIR)
	rm -f *.svg game_frame_*.svg

# Development helpers
debug: CXXFLAGS += -g -DDEBUG
debug: all

release: CXXFLAGS += -O3 -DNDEBUG
release: all

# Static analysis
analyze:
	cppcheck --enable=all --std=c++17 $(ALL_SOURCES)

# Format code
format:
	clang-format -i $(ALL_SOURCESEXERNAL)/*.h

# Show help
help:
	@echo "Epoch of Elria Game Engine - Build System"
	@echo "=========================================="
	@echo ""
	@echo "vailable targets:"
	@echo "  all              - Build all game demos"
	@echo "  directories      - Create build directories"
	@echo "  assets          - Generate sample SVG assets"
	@echo ""
	@echo "Individual builds:"
	@echo "  game_engine_parallel_demo  - Main parallel processing demo"
	@echo "  game_engine_simple_graphics - Simple graphics demo"
	@echo "  game_walkable_simple       - Walkable game demo"
	@echo "  game_3d_openworld          - 3D open world demo"
	@echo "  rpg_characters             - RPG character demo"
	@echo "  dream_weaver_complete      - Complete game"
	@echo ""
	@echo "Run targets:"
	@echo "  run-parallel    - Run parallel demo"
	@echo "  run-simple      - Run simple graphics demo"
	@echo "  run-walkable    - Run walkable demo"
	@echo "  run-3d          - Run 3D demo"
	@echo "  run-rpg         - un RP demo"
	@echo "  run-complete    - Run complete game"
	@echo ""
	@echo "Utility targets:"
	@echo "  clean           - Remove object files"
	@echo "  distclean       - Remove all build artifacts"
	@echo "  debug           - Build with debug symbols"
	@echo "  release         - Build optimized release"
	@echo "  install-deps    - Install dependencies (Ubuntu/Debian)"
	@echo "  install-deps-mac - Install dependencies (macOS"
	@echo "  analyze         - Run static analysis"
	@echo "  format          - Format source code"
	@echo "  help            - Show this help message"
# Targets
TARGETS all directories clean distclean debug release run-parallel run-simple run-w= kab\e run-3d run-rpgrun-omplete assets install-deps instal-dps-mac alyze format help
    $(BINDIR)/game_engine_parallel_demo \
    $(BINDIR)/game_engine_simple_graphics \
    $(BINDIR)/game_walkable_simple \
    $(BINDIR)/game_3d_openworld \
    $(BINDIR)/rpg_characters \
    $(BINDIR)/dream_weaver_complete

# Default target
all: directories $(TARGETS)

# Create necessary directories
directories:
	@mkdir -p $(OBJDIR) $(BINDIR)

# Main game engine (parallel demo)
$(BINDIR)/game_engine_parallel_demo: $(ALL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Simple graphics demo
$(BINDIR)/game_engine_simple_graphics: $(ALL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -DSIMPLE_GRAPHICS

# Walkable game demo
$(BINDIR)/game_walkable_simple: $(ALL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -DWALKABLE_GAME

# 3D open world demo
$(BINDIR)/game_3d_openworld: $(ALL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -DOPEN_WORLD_3D

# RPG characters demo
$(BINDIR)/rpg_characters: $(ALL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -DRPG_CHARACTERS

# Complete dream weaver game
$(BINDIR)/dream_weaver_complete: $(ALL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -DDREAM_WEAVER

# Object file compilation rules
$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(EXTERNAL)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Dependencies (header files)
$(OBJDIR)/main.o: main.cpp $(EXTERNAL)/GameObject.h $(EXTERNAL)/GameObject3D.h $(EXTERNAL)/graphics.h $(EXTERNAL)/GameEnvironment.h $(EXTERNAL)/rendering_subsystem.h
$(OBJDIR)/GameObject.o: $(EXTERNAL)/GameObject.cpp $(EXTERNAL)/GameObject.h $(EXTERNAL)/Vector2D.h
$(OBJDIR)/GameObject3D.o: $(EXTERNAL)/GameObject3D.cpp $(EXTERNAL)/GameObject3D.h $(EXTERNAL)/Vector2D.h
$(OBJDIR)/graphics.o: $(EXTERNAL)/graphics.cpp $(EXTERNAL)/graphics.h
$(OBJDIR)/rendering_subsystem.o: $(EXTERNAL)/rendering_subsystem.cpp $(EXTERNAL)/rendering_subsystem.h $(EXTERNAL)/GameObject.h $(EXTERNAL)/GameObject3D.h
$(OBJDIR)/GameEnvironment.o: $(EXTERNAL)/GameEnvironment.cpp $(EXTERNAL)/GameEnvironment.h $(EXTERNAL)/GameObject.h $(EXTERNAL)/GameObject3D.h $(EXTERNAL)/graphics.h

# Run targets
run-parallel: $(BINDIR)/game_engine_parallel_demo
	./$(BINDIR)/game_engine_parallel_demo

run-simple: $(BINDIR)/game_engine_simple_graphics
	./$(BINDIR)/game_engine_simple_graphics

run-walkable: $(BINDIR)/game_walkable_simple
	./$(BINDIR)/game_walkable_simple

run-3d: $(BINDIR)/game_3d_openworld
	./$(BINDIR)/game_3d_openworld

run-rpg: $(BINDIR)/rpg_characters
	./$(BINDIR)/rpg_characters

run-complete: $(BINDIR)/dream_weaver_complete
	./$(BINDIR)/dream_weaver_complete

# Asset generation
assets: directories
	@echo "Generating sample SVG assets..."
	@echo '<svg xmlns="http://www.w3.org/2000/svg" width="64" height="64"><rect width="64" height="64" fill="blue"/></svg>' > assets/player.svg
	@echo '<svg xmlns="http://www.w3.org/2000/svg" width="32" height="32"><circle cx="16" cy="16" r="15" fill="gold"/></svg>' > assets/coin.svg
	@echo '<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24"><polygon points="12,2 22,20 2,20" fill="red"/></svg>' > assets/gem.svg
	@echo '<svg xmlns="http://www.w3.org/2000/svg" width="800" height="600"><rect width="800" height="600" fill="url(#grad)"/><defs><linearGradient id="grad"><stop offset="0%" stop-color="purple"/><stop offset="100%" stop-color="black"/></linearGradient></defs></svg>' > assets/background.svg
	@echo '<svg xmlns="http://www.w3.org/2000/svg" width="100" height="100"><circle cx="50" cy="50" r="40" fill="cyan" opacity="0.7"/><circle cx="50" cy="50" r="20" fill="white"/></svg>' > assets/metaverse_portal.svg
	@echo "Assets generated successfully!"

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y libsfml-dev g++ make

# Install dependencies (macOS with Homebrew)
install-deps-mac:
	brew install sfml

# Clean targets
clean:
	rm -rf $(OBJDIR)

distclean: clean
	rm -rf $(BINDIR)
	rm -f *.svg game_frame_*.svg

# Development helpers
debug: CXXFLAGS += -g -DDEBUG
debug: all

release: CXXFLAGS += -O3 -DNDEBUG
release: all

# Static analysis
analyze:
	cppcheck --enable=all --std=c++17 $(ALL_SOURCES)

# Format code
format:
	clang-format -i $(ALL_SOURCES) $(EXTERNAL)/*.h

# Show help
help:
	@echo "Epoch of Elria Game Engine - Build System"
	@echo "=========================================="
	@echo ""
	@echo "Available targets:"
	@echo "  all              - Build all game demos"
	@echo "  directories      - Create build directories"
	@echo "  assets          - Generate sample SVG assets"
	@echo ""
	@echo "Individual builds:"
	@echo "  game_engine_parallel_demo  - Main parallel processing demo"
	@echo "  game_engine_simple_graphics - Simple graphics demo"
	@echo "  game_walkable_simple       - Walkable game demo"
	@echo "  game_3d_openworld          - 3D open world demo"
	@echo "  rpg_characters             - RPG character demo"
	@echo "  dream_weaver_complete      - Complete game"
	@echo ""
	@echo "Run targets:"
	@echo "  run-parallel    - Run parallel demo"
	@echo "  run-simple      - Run simple graphics demo"
	@echo "  run-walkable    - Run walkable demo"
	@echo "  run-3d          - Run 3D demo"
	@echo "  run-rpg         - Run RPG demo"
	@echo "  run-complete    - Run complete game"
	@echo ""
	@echo "Utility targets:"
	@echo "  clean           - Remove object files"
	@echo "  distclean       - Remove all build artifacts"
	@echo "  debug           - Build with debug symbols"
	@echo "  release         - Build optimized release"
	@echo "  install-deps    - Install dependencies (Ubuntu/Debian)"
	@echo "  install-deps-mac - Install dependencies (macOS)"
	@echo "  analyze         - Run static analysis"
	@echo "  format          - Format source code"
	@echo "  help            - Show this help message"

.PHONY: all directories clean distclean debug release run-parallel run-simple run-walkable run-3d run-rpg run-complete assets install-deps install-deps-mac analyze format help
