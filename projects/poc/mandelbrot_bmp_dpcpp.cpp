#include <CL/sycl.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

using namespace sycl;

constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;
constexpr int MAX_ITER = 1000;

// Helper to write little-endian bytes
void write_int(std::ofstream &f, uint32_t val) {
    f.put(val & 0xFF);
    f.put((val >> 8) & 0xFF);
    f.put((val >> 16) & 0xFF);
    f.put((val >> 24) & 0xFF);
}

int main() {
    std::vector<uint8_t> image(WIDTH * HEIGHT * 3); // RGB

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
                int color = (255 * iter) / MAX_ITER;
                int offset = ((HEIGHT - 1 - y) * WIDTH + x) * 3;
                acc[offset] = color;         // Blue
                acc[offset + 1] = color;     // Green
                acc[offset + 2] = color;     // Red
            });
        });
        q.wait();
    }

    // Save BMP file
    std::ofstream f("mandelbrot.bmp", std::ios::binary);
    int row_padded = (WIDTH * 3 + 3) & (~3);
    int filesize = 54 + row_padded * HEIGHT;

    // BMP Header
    f.put('B'); f.put('M');
    write_int(f, filesize);
    write_int(f, 0);
    write_int(f, 54);

    // DIB Header
    write_int(f, 40);                 // Header size
    write_int(f, WIDTH);
    write_int(f, HEIGHT);
    f.put(1); f.put(0);               // Planes
    f.put(24); f.put(0);              // Bits per pixel
    write_int(f, 0);                  // Compression
    write_int(f, row_padded * HEIGHT);
    write_int(f, 2835); write_int(f, 2835); // DPI
    write_int(f, 0); write_int(f, 0);       // Color palette

    // Write pixel data (with padding)
    for (int y = 0; y < HEIGHT; y++) {
        int row_start = y * WIDTH * 3;
        f.write(reinterpret_cast<char*>(&image[row_start]), WIDTH * 3);
        for (int p = 0; p < row_padded - WIDTH * 3; ++p)
            f.put(0); // Padding
    }

    f.close();
    std::cout << "Mandelbrot BMP saved as mandelbrot.bmp\n";
    return 0;
}
