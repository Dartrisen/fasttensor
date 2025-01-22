# Metal-Accelerated Tensor Operations (fasttensor)

This project demonstrates GPU-accelerated tensor operations using Apple's Metal framework. It performs element-wise addition and multiplication on large tensors, leveraging Metal for efficient computation.

---

## Features
- GPU acceleration using the Metal framework.
- Lightweight and efficient tensor operations.
- Automated build process using a `Makefile`.

---

## Directory Structure

```plaintext
├── include/
│   ├── MetalExecutor.h
│   └── Tensor.h
├── src/
│   ├── main.cpp
│   ├── MetalExecutor.cpp
│   └── Tensor.cpp
├── shaders/
│   └── tensor.metal
├── build/
│   ├── tensor_app
│   └── tensor.metallib
├── Makefile
└── README.md
```

---

## Prerequisites

- macOS with Xcode installed.
- Command-line tools for Xcode (`xcode-select --install`).

---

## Build and Run

Run the Makefile to compile the Metal shader and C++ source files: `make`

Run the Application: `./build/tensor_app`

Clean the build directory: `make clean`

---

## Metal Shader Details

The `shaders/tensor.metal` file defines two GPU compute functions:

- `add_tensors`: Performs element-wise addition of two tensors.
- `multiply_tensors`: Performs element-wise multiplication of two tensors.

These functions are called from the C++ application using Metal’s pipeline.

---

## Example Output

After running the program, you should see output similar to this:

```plaintext
Metal GPU Tensor Multiply Time: 0.00234 s
```

---

## Acknowledgments

Thanks to Apple’s Metal framework documentation. Inspiration drawn from GPU-accelerated tensor libraries like PyTorch and TensorFlow.
