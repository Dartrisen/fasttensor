// Tensor.h
#pragma once
#include <vector>
#include <string>
#include <stdexcept>

class Tensor {
public:
    std::vector<float> data;
    size_t size;

    Tensor(size_t size);

    void fill(float value);
    void add(const Tensor& other);
    void multiply(const Tensor& other);

private:
    void performOperation(const std::string& functionName, const Tensor& other);
};