#include "MetalExecutor.h"
#include <Foundation/Foundation.h>
#include <stdexcept>

MetalExecutor::MetalExecutor() {
    @autoreleasepool {
        device = MTLCreateSystemDefaultDevice();
        if (!device) throw std::runtime_error("Failed to get Metal device");

        queue = [device newCommandQueue];
        if (!queue) throw std::runtime_error("Failed to create command queue");

        loadLibrary();
    }
}

MetalExecutor::~MetalExecutor() {}

void MetalExecutor::loadLibrary() {
    @autoreleasepool {
        NSError* error = nil;
        NSString *currentDir = [[NSFileManager defaultManager] currentDirectoryPath];
        NSString *shaderPath = [currentDir stringByAppendingPathComponent:@"build/tensor.metallib"];
        NSURL *shaderURL = [NSURL fileURLWithPath:shaderPath];
        library = [device newLibraryWithURL:shaderURL error:&error];
        if (!library || error) {
            std::string errorMsg = "Failed to load shader library: ";
            errorMsg += [[error localizedDescription] UTF8String];
            throw std::runtime_error(errorMsg);
        }
    }
}

void MetalExecutor::execute(const std::string& functionName, const std::vector<float>& inputA, const std::vector<float>& inputB, size_t size) {
    @autoreleasepool {
        NSError* error = nil;

        id<MTLFunction> function = [library newFunctionWithName:[NSString stringWithUTF8String:functionName.c_str()]];
        if (!function) throw std::runtime_error("Failed to find function: " + functionName);

        id<MTLComputePipelineState> pipeline = [device newComputePipelineStateWithFunction:function error:&error];
        if (!pipeline || error) throw std::runtime_error("Failed to create pipeline state");

        id<MTLBuffer> bufferA = [device newBufferWithBytes:inputA.data() length:size * sizeof(float) options:MTLResourceStorageModeShared];
        id<MTLBuffer> bufferB = [device newBufferWithBytes:inputB.data() length:size * sizeof(float) options:MTLResourceStorageModeShared];
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

        memcpy((void*)inputA.data(), [bufferC contents], size * sizeof(float));
    }
}
