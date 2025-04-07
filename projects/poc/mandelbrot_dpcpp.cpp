#include <CL/sycl.hpp>
#include <iostream>
#include <fstream>
#include <vector>

using namespace sycl;

constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;
constexpr int MAX_ITER = 1000;

// Maps pixel coordinates to the complex plane
std::pair<float, float> pixel_to_complex(int x, int y) {
    float re = (x - WIDTH / 2.0f) * 4.0f / WIDTH;
    float im = (y - HEIGHT / 2.0f) * 4.0f / HEIGHT;
    return {re, im};
}

int main() {
    std::vector<int> image(WIDTH * HEIGHT);

    queue q;
    {
        buffer<int> img_buf(image.data(), range<1>(WIDTH * HEIGHT));

        q.submit([&](handler& h) {
            auto acc = img_buf.get_access<access::mode::write>(h);

            h.parallel_for(range<2>(HEIGHT, WIDTH), [=](id<2> idx) {
                int y = idx[0], x = idx[1];
                float cr = (x - WIDTH / 2.0f) * 4.0f / WIDTH;
                float ci = (y - HEIGHT / 2.0f) * 4.0f / HEIGHT;
                float zr = 0, zi = 0;
                int iter = 0;
                while (zr * zr + zi * zi < 4.0f && iter < MAX_ITER) {
                    float tmp = zr * zr - zi * zi + cr;
                    zi = 2.0f * zr * zi + ci;
                    zr = tmp;
                    iter++;
                }
                acc[y * WIDTH + x] = iter;
            });
        });
        q.wait();
    }

    // Save to PPM image file (simple format)
    std::ofstream ofs("mandelbrot.ppm");
    ofs << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        int c = (255 * image[i]) / MAX_ITER;
        ofs << c << " " << c << " " << c << " ";
    }
    ofs.close();

    std::cout << "Mandelbrot image saved as mandelbrot.ppm\n";
    return 0;
}
