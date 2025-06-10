# Makefile for Epoch of Elria Game Engine

CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./External
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC = \
    main.cpp \
    External/GameEnvironment.cpp \
    External/GameObject.cpp \
    External/GameObject3D.cpp \
    External/graphics.cpp \
    External/rendering_subsystem.cpp

OBJ = $(SRC:.cpp=.o)

TARGET = game_engine

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
