#include <CL/sycl.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdint>

using namespace sycl;

constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;
constexpr int MAX_ITER = 1000;

void write_int(std::ofstream &f, uint32_t val) {
    f.put(val & 0xFF);
    f.put((val >> 8) & 0xFF);
    f.put((val >> 16) & 0xFF);
    f.put((val >> 24) & 0xFF);
}

int main() {
    std::vector<uint8_t> image(WIDTH * HEIGHT * 3);

    queue q;
    {
        buffer<uint8_t> img_buf(image.data(), range<1>(image.size()));

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

                float t = (float)iter / MAX_ITER;
                uint8_t r = (uint8_t)(9 * (1 - t) * t * t * t * 255);
                uint8_t g = (uint8_t)(15 * (1 - t) * (1 - t) * t * t * 255);
                uint8_t b = (uint8_t)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

                int offset = ((HEIGHT - 1 - y) * WIDTH + x) * 3;
                acc[offset]     = b;
                acc[offset + 1] = g;
                acc[offset + 2] = r;
            });
        });
        q.wait();
    }

    std::ofstream f("mandelbrot_color.bmp", std::ios::binary);
    int row_padded = (WIDTH * 3 + 3) & (~3);
    int filesize = 54 + row_padded * HEIGHT;

    f.put('B'); f.put('M');
    write_int(f, filesize);
    write_int(f, 0);
    write_int(f, 54);

    write_int(f, 40);
    write_int(f, WIDTH);
    write_int(f, HEIGHT);
    f.put(1); f.put(0);
    f.put(24); f.put(0);
    write_int(f, 0);
    write_int(f, row_padded * HEIGHT);
    write_int(f, 2835); write_int(f, 2835);
    write_int(f, 0); write_int(f, 0);

    for (int y = 0; y < HEIGHT; y++) {
        int row_start = y * WIDTH * 3;
        f.write(reinterpret_cast<char*>(&image[row_start]), WIDTH * 3);
        for (int p = 0; p < row_padded - WIDTH * 3; ++p)
            f.put(0);
    }

    f.close();
    std::cout << "✅ Colored Mandelbrot saved as mandelbrot_color.bmp\n";
    return 0;
}

