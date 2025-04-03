#include <CL/sycl.hpp>
#include <iostream>

using namespace sycl;

int main() {
    constexpr size_t N = 8;

    // Emulate FPGA device using CPU
    queue q{default_selector_v};

    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    int* a = malloc_shared<int>(N, q);
    int* b = malloc_shared<int>(N, q);
    int* c = malloc_shared<int>(N, q);

    for (size_t i = 0; i < N; i++) {
        a[i] = i;
        b[i] = N - i;
    }

    // Submit a kernel to emulate FPGA behavior
    q.single_task([=]() {
            for (size_t i = 0; i < N; i++) {
                c[i] = a[i] + b[i];
            }
        }).wait();

    std::cout << "Result:\n";
    for (size_t i = 0; i < N; i++) {
        std::cout << "c[" << i << "] = " << c[i] << "\n";
    }

    free(a, q);
    free(b, q);
    free(c, q);

    return 0;
}
