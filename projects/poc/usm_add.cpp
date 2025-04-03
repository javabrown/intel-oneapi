#include <CL/sycl.hpp>
#include <iostream>

using namespace sycl;

int main() {
    constexpr size_t N = 10;

    // Use default device selector (CPU/GPU/FPGA)
    queue q{default_selector_v};

    std::cout << "Running on: "
              << q.get_device().get_info<info::device::name>() << "\n";

    // Allocate Unified Shared Memory (USM) — shared between host and device
    int* a = malloc_shared<int>(N, q);
    int* b = malloc_shared<int>(N, q);
    int* c = malloc_shared<int>(N, q);

    // Initialize input
    for (size_t i = 0; i < N; i++) {
        a[i] = i;
        b[i] = i * 2;
    }

    // Submit kernel with USM pointers
    q.parallel_for(range<1>(N), [=](id<1> i) {
        c[i] = a[i] + b[i];
    }).wait();  // wait for kernel to finish

    std::cout << "Results:\n";
    for (size_t i = 0; i < N; i++) {
        std::cout << "c[" << i << "] = " << c[i] << "\n";
    }

    // Free USM
    free(a, q);
    free(b, q);
    free(c, q);

    return 0;
}
