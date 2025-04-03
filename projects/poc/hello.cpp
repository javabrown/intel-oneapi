#include <CL/sycl.hpp>
#include <iostream>

using namespace sycl;

int main() {
    try {
        // Create a queue using the default device selector
        queue q{default_selector_v};

        // Print device info
        std::cout << "Hello from SYCL!\n";
        std::cout << "Running on device: "
                  << q.get_device().get_info<info::device::name>() << "\n";
        std::cout << "Vendor: "
                  << q.get_device().get_info<info::device::vendor>() << "\n";
        std::cout << "Driver version: "
                  << q.get_device().get_info<info::device::driver_version>() << "\n";
    }
    catch (const sycl::exception& e) {
        std::cerr << "SYCL exception: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
