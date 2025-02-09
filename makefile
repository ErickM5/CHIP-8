CXX       = g++
CXXFLAGS  = -std=c++17 -Wall `sdl2-config --cflags` -I lib
LDFLAGS   = `sdl2-config --libs`

SRC_DIR   = lib
OBJS      = $(SRC_DIR)/chip8.o $(SRC_DIR)/memory.o $(SRC_DIR)/IOdevices.o

all: main clean

main: $(OBJS)
	$(CXX) $(OBJS) -o main $(LDFLAGS)

$(SRC_DIR)/chip8.o: $(SRC_DIR)/chip8.cpp $(SRC_DIR)/chip8.hpp $(SRC_DIR)/memory.hpp $(SRC_DIR)/IOdevices.hpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/chip8.cpp -o $(SRC_DIR)/chip8.o

$(SRC_DIR)/memory.o: $(SRC_DIR)/memory.cpp $(SRC_DIR)/memory.hpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/memory.cpp -o $(SRC_DIR)/memory.o

$(SRC_DIR)/IOdevices.o: $(SRC_DIR)/IOdevices.cpp $(SRC_DIR)/IOdevices.hpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/IOdevices.cpp -o $(SRC_DIR)/IOdevices.o

remove:
	rm -f main

clean:
	rm -f $(SRC_DIR)/*.o
