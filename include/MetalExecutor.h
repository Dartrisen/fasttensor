#pragma once
#include <Metal/Metal.h>
#include <vector>
#include <string>

class MetalExecutor {
public:
    MetalExecutor();
    ~MetalExecutor();
    void execute(const std::string& functionName, const std::vector<float>& inputA, const std::vector<float>& inputB, size_t size);

private:
    id<MTLDevice> device;
    id<MTLCommandQueue> queue;
    id<MTLLibrary> library;

    void loadLibrary();
};
