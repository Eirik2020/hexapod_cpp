# Compiler and compiler flags
CXX = g++
CXXFLAGS = -Wall -Wno-unused-variable -std=c++17 -I/usr/include/eigen3 -I/usr/include/boost

# Linking flags for Boost libraries (and pthread if needed)
LDFLAGS = -lboost_system -lboost_serialization -lpthread

# Source files and target
SOURCES = hexapod.cpp Leg.cpp Maestro.cpp main.cpp TCP_server.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = hexapod_run

# Default rule to build the program
all: $(TARGET)

# Link object files into the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Rule to compile each .cpp file to a .o file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJECTS) $(TARGET)
