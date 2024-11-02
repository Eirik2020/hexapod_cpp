# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11 -I/usr/include/eigen3

# Source files
SOURCES = main.cpp TCP_server.cpp Hexapod.cpp Maestro.cpp Leg.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = hexapod_run

# Default target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to build .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJECTS) $(TARGET)
