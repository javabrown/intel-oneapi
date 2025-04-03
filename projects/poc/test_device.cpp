#include <CL/sycl.hpp>
#include <iostream>

using namespace sycl;

int main() {
    try {
        // Automatically selects the best available Intel device (CPU, GPU, FPGA)
        queue q{ default_selector_v };

        // Print device info
        auto device = q.get_device();
        std::cout << "Running on device: " << device.get_info<info::device::name>() << "\n";
        std::cout << "Vendor: " << device.get_info<info::device::vendor>() << "\n";
        std::cout << "Driver version: " << device.get_info<info::device::driver_version>() << "\n";
        std::cout << "Max compute units: " << device.get_info<info::device::max_compute_units>() << "\n";
        std::cout << "Global memory size: " << device.get_info<info::device::global_mem_size>() / (1024 * 1024) << " MB\n";

        constexpr int N = 8;
        int data[N] = {0};

        // Allocate buffer and launch a kernel to fill data
        {
            buffer<int, 1> buf(data, range<1>(N));
            q.submit([&](handler& h) {
                auto acc = buf.get_access<access::mode::write>(h);
                h.parallel_for(range<1>(N), [=](id<1> i) {
                    acc[i] = i[0] * i[0];  // simple square operation
                });
            });
        }

        // Output result
        std::cout << "Output from device kernel:\n";
        for (int i = 0; i < N; ++i) {
            std::cout << "data[" << i << "] = " << data[i] << "\n";
        }
    } catch (const sycl::exception& e) {
        std::cerr << "SYCL exception caught: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

