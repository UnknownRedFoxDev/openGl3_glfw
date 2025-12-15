CXX := g++
OUTPUT := my_program
SRC_DIR := src
BUILD_DIR := build
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
LIBS := -lGL -lglfw -lGLEW
INCLUDES := -Isrc -Isrc/vendor -Isrc/tests
CXXFLAGS := -std=c++23 -Wall -Wextra -O0 -ggdb

all: $(BUILD_DIR)/$(OUTPUT)
$(BUILD_DIR)/$(OUTPUT): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	./test.sh

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
