CC = g++
CFLAGS = -std=c++17 $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image
TARGET = ./game
SRC = ./main.cpp ./Game.cpp ./TextureManager.cpp ./GameObject.cpp ./Map.cpp
HEADERS = ./Game.hpp ./TextureManager.hpp ./GameObject.hpp ./Map.hpp ./ECS.hpp ./Components.hpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
