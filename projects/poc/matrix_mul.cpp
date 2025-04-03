#include <CL/sycl.hpp>
#include <iostream>
#include <vector>

using namespace sycl;

int main() {
    const int M = 4, N = 4, P = 4;

    std::vector<float> A = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 8, 7, 6,
        5, 4, 3, 2
    };

    std::vector<float> B = {
        1, 0, 0, 1,
        0, 1, 1, 0,
        1, 0, 0, 1,
        0, 1, 1, 0
    };

    std::vector<float> C(M * P, 0);

    queue q;
    buffer<float, 1> a_buf(A.data(), range<1>(M * N));
    buffer<float, 1> b_buf(B.data(), range<1>(N * P));
    buffer<float, 1> c_buf(C.data(), range<1>(M * P));

    q.submit([&](handler& h) {
        auto a = a_buf.get_access<access::mode::read>(h);
        auto b = b_buf.get_access<access::mode::read>(h);
        auto c = c_buf.get_access<access::mode::write>(h);

        h.parallel_for(range<2>(M, P), [=](id<2> idx) {
            int row = idx[0];
            int col = idx[1];
            float sum = 0;
            for (int k = 0; k < N; k++) {
                sum += a[row * N + k] * b[k * P + col];
            }
            c[row * P + col] = sum;
        });
    });
    
    q.wait();
    
    // Read results from buffer
    auto c_host = c_buf.get_access<access::mode::read>();
    
    // Output result
    std::cout << "Result C:\n";
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < P; ++j) {
            std::cout << c_host[i * P + j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
