// Tensor.cpp
#include "Tensor.h"
#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#include <cstring>

Tensor::Tensor(size_t size) : size(size), data(size, 0.0f) {}

void Tensor::fill(float value) {
    std::fill(data.begin(), data.end(), value);
}

void Tensor::add(const Tensor& other) {
    performOperation("add_tensors", other);
}

void Tensor::multiply(const Tensor& other) {
    performOperation("multiply_tensors", other);
}

void Tensor::performOperation(const std::string& functionName, const Tensor& other) {
    if (size != other.size) {
        throw std::runtime_error("Tensor sizes do not match for operation.");
    }

    @autoreleasepool {
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        if (!device) throw std::runtime_error("Failed to get Metal device");

        id<MTLCommandQueue> queue = [device newCommandQueue];
        if (!queue) throw std::runtime_error("Failed to create command queue");

        NSError* error = nil;
        NSString *currentDir = [[NSFileManager defaultManager] currentDirectoryPath];
        NSString *shaderPath = [currentDir stringByAppendingPathComponent:@"build/tensor.metallib"];
        NSURL *shaderURL = [NSURL fileURLWithPath:shaderPath];
        id<MTLLibrary> library = [device newLibraryWithURL:shaderURL error:&error];
        if (!library || error) {
            std::string errorMsg = "Failed to load shader library: ";
            errorMsg += [[error localizedDescription] UTF8String];
            throw std::runtime_error(errorMsg);
        }

        id<MTLFunction> function = [library newFunctionWithName:[NSString stringWithUTF8String:functionName.c_str()]];
        if (!function) throw std::runtime_error("Failed to find function: " + functionName);

        id<MTLComputePipelineState> pipeline = [device newComputePipelineStateWithFunction:function error:&error];
        if (!pipeline || error) throw std::runtime_error("Failed to create pipeline state");

        id<MTLBuffer> bufferA = [device newBufferWithBytes:data.data() length:size * sizeof(float) options:MTLResourceStorageModeShared];
        id<MTLBuffer> bufferB = [device newBufferWithBytes:other.data.data() length:size * sizeof(float) options:MTLResourceStorageModeShared];
        id<MTLBuffer> bufferC = [device newBufferWithLength:size * sizeof(float) options:MTLResourceStorageModeShared];

        id<MTLCommandBuffer> commandBuffer = [queue commandBuffer];
        id<MTLComputeCommandEncoder> encoder = [commandBuffer computeCommandEncoder];

        [encoder setComputePipelineState:pipeline];
        [encoder setBuffer:bufferA offset:0 atIndex:0];
        [encoder setBuffer:bufferB offset:0 atIndex:1];
        [encoder setBuffer:bufferC offset:0 atIndex:2];

        uint32_t threadsPerGroup = 32;
        uint32_t numThreadgroups = (size + threadsPerGroup - 1) / threadsPerGroup;
        [encoder dispatchThreadgroups:MTLSizeMake(numThreadgroups, 1, 1)
               threadsPerThreadgroup:MTLSizeMake(threadsPerGroup, 1, 1)];

        [encoder endEncoding];
        [commandBuffer commit];
        [commandBuffer waitUntilCompleted];

        memcpy(data.data(), [bufferC contents], size * sizeof(float));
    }
}