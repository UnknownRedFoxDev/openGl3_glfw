CXX := g++
OUTPUT := my_program
SRC_DIR := src
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(notdir $(SRCS:.cpp=.o))
LIBS := -lGL -lglfw
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

all: $(BUILD_DIR)/$(OUTPUT)

$(BUILD_DIR)/$(OUTPUT): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f *.o

run:
	cd $(BUILD_DIR) && ./$(OUTPUT) && cd ..

.PHONY: all run clean
