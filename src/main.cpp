// main.cpp
#include "Tensor.h"
#include <iostream>
#include <chrono>

int main() {
    try {
        Tensor A(1000000), B(1000000);
        A.fill(1.0f);
        B.fill(2.0f);

        auto start = std::chrono::high_resolution_clock::now();
        A.multiply(B);
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Metal GPU Tensor Operation Time: "
                  << std::chrono::duration<double>(end - start).count() << " s\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}