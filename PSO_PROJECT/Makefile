# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -I. -I./pso -I./utils

# Source files
SRCS = main.cc \
       utils/Utils.cc \
       utils/Initializer.cc \
       utils/Dataset.cc  \
       pso/Particle.cc \
       pso/Fitness.cc \
       pso/PSO.cc 
       

# Object files
OBJS = $(SRCS:.cc=.o)

# Output binary
TARGET = pso_imputation

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Compile each .cc into .o
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
