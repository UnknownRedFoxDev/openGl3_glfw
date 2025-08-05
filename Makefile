CXX := g++
OUTPUT := my_program
SRC_DIR := src
BUILD_DIR := build

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')

OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

LIBS := -lGL -lglfw -lGLEW
CXXFLAGS := -std=c++17 -Wall -Wextra -O0 -g

all: $(BUILD_DIR)/$(OUTPUT)

# Compile the executable
$(BUILD_DIR)/$(OUTPUT): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

# Compile each cpp file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory (not needed for subdirs, see above mkdir -p)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: all
	./$(BUILD_DIR)/$(OUTPUT)

.PHONY: all run clean

