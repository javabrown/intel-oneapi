#include <CL/sycl.hpp>
#include <iostream>
#include <vector>

using namespace sycl;

int main() {
    std::string input = "DPC++ is powerful!";
    std::vector<int> ascii(input.size());

    queue q;
    {
        buffer<char> in_buf(input.data(), range<1>(input.size()));
        buffer<int> out_buf(ascii.data(), range<1>(input.size()));

        q.submit([&](handler& h) {
            auto in = in_buf.get_access<access::mode::read>(h);
            auto out = out_buf.get_access<access::mode::write>(h);

            h.parallel_for(range<1>(input.size()), [=](id<1> i) {
                out[i] = static_cast<int>(in[i]); // convert char to ASCII code
            });
        });
        q.wait();
    }

    std::cout << "Input: " << input << "\nASCII: ";
    for (int i : ascii) std::cout << i << " ";
    std::cout << "\n";

    return 0;
}
