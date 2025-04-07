#include <CL/sycl.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

using namespace sycl;

constexpr size_t N = 100'000'000; // Make the work heavy enough

int main() {
    std::vector<float> data(N, 1.0001f);
    std::vector<float> result_seq(N);
    std::vector<float> result_par(N);

    // === Sequential
    auto start_seq = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
        result_seq[i] = std::log(std::sqrt(data[i])) + std::exp(data[i]);
    }
    auto end_seq = std::chrono::high_resolution_clock::now();
    auto time_seq = std::chrono::duration<double, std::milli>(end_seq - start_seq).count();
    std::cout << "Sequential compute time: " << time_seq << " ms\n";

    // === Parallel DPC++ (computation only timed)
    queue q{default_selector{}, property::queue::enable_profiling{}};
    buffer<float> data_buf(data.data(), range<1>(N));
    buffer<float> result_buf(result_par.data(), range<1>(N));

    // Launch kernel and capture profiling info
    auto e = q.submit([&](handler& h) {
        auto in = data_buf.get_access<access::mode::read>(h);
        auto out = result_buf.get_access<access::mode::write>(h);

        h.parallel_for(range<1>(N), [=](id<1> i) {
            out[i] = sycl::log(sycl::sqrt(in[i])) + sycl::exp(in[i]);
        });
    });
    e.wait_and_throw(); // wait for kernel to finish

    // Profiling compute time
    auto start = e.get_profiling_info<info::event_profiling::command_start>();
    auto end   = e.get_profiling_info<info::event_profiling::command_end>();
    double time_par = (end - start) / 1e6; // ns → ms
    std::cout << "DPC++ parallel compute time: " << time_par << " ms\n";

    // === Verify
    for (size_t i = 0; i < N; ++i) {
        if (std::abs(result_seq[i] - result_par[i]) > 1e-4) {
            std::cerr << "Mismatch at index " << i << "\n";
            return 1;
        }
    }

    std::cout << "Results match!\n";
    return 0;
}
