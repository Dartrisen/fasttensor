SRC_DIR = src
INCLUDE_DIR = include
SHADER_DIR = shaders
BUILD_DIR = build

CPP_FILES = $(SRC_DIR)/main.cpp $(SRC_DIR)/Tensor.cpp $(SRC_DIR)/MetalExecutor.cpp
H_FILES = $(INCLUDE_DIR)/Tensor.h $(INCLUDE_DIR)/MetalExecutor.h
SHADER_FILES = $(SHADER_DIR)/tensor.metal
METALLIB = $(BUILD_DIR)/tensor.metallib

CXX = clang++
CXXFLAGS = -x objective-c++ -std=c++17 -O3 -march=native -flto -I$(INCLUDE_DIR) -framework Metal -framework Foundation

METAL = xcrun -sdk macosx metal -O3
METALLIB_CMD = xcrun -sdk macosx metallib

all: $(METALLIB) tensor_app

$(METALLIB): $(SHADER_FILES)
	@mkdir -p $(BUILD_DIR)
	$(METAL) -c $(SHADER_FILES) -o $(BUILD_DIR)/tensor.air
	$(METALLIB_CMD) $(BUILD_DIR)/tensor.air -o $(METALLIB)
	rm $(BUILD_DIR)/tensor.air

tensor_app: $(CPP_FILES) $(H_FILES) $(METALLIB)
	$(CXX) $(CXXFLAGS) $(CPP_FILES) -o $(BUILD_DIR)/tensor_app

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean