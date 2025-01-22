// Tensor.cpp
#include "Tensor.h"
#include "MetalExecutor.h"
#include <stdexcept>
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

    MetalExecutor executor;
    executor.execute(functionName, data, other.data, size);
}
