# Convenience wrapper around the CMake build.
# Usage:
#   make            - configure (if needed) and build
#   make run        - build and run the test app
#   make clean      - remove the build directory
#   make rebuild    - clean + build

BUILD_DIR   := build
BUILD_TYPE  ?= Debug
TARGET      := AliPhysicsEngine

ifeq ($(OS),Windows_NT)
    EXE := $(BUILD_DIR)/bin/$(TARGET).exe
else
    EXE := $(BUILD_DIR)/bin/$(TARGET)
endif

.PHONY: all configure build run clean rebuild

all: build

configure: $(BUILD_DIR)/CMakeCache.txt

$(BUILD_DIR)/CMakeCache.txt:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

build: configure
	cmake --build $(BUILD_DIR) --config $(BUILD_TYPE)

run: build
	"$(EXE)"

clean:
	cmake -E rm -rf $(BUILD_DIR)

rebuild: clean build
