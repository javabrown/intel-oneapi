//#include <CL/sycl.hpp>
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <cmath>
//#include <iomanip>
//#include <sstream>
//
//using namespace sycl;
//
//constexpr int WIDTH = 800;
//constexpr int HEIGHT = 800;
//constexpr int MAX_ITER = 1000;
//constexpr int TOTAL_FRAMES = 60;
//
//void write_int(std::ofstream &f, uint32_t val) {
//    f.put(val & 0xFF);
//    f.put((val >> 8) & 0xFF);
//    f.put((val >> 16) & 0xFF);
//    f.put((val >> 24) & 0xFF);
//}
//
//void save_bmp(const std::vector<uint8_t>& image, const std::string& filename) {
//    int row_padded = (WIDTH * 3 + 3) & (~3);
//    int filesize = 54 + row_padded * HEIGHT;
//
//    std::ofstream f(filename, std::ios::binary);
//    f.put('B'); f.put('M');
//    write_int(f, filesize);
//    write_int(f, 0);
//    write_int(f, 54);
//
//    write_int(f, 40);
//    write_int(f, WIDTH);
//    write_int(f, HEIGHT);
//    f.put(1); f.put(0);
//    f.put(24); f.put(0);
//    write_int(f, 0);
//    write_int(f, row_padded * HEIGHT);
//    write_int(f, 2835); write_int(f, 2835);
//    write_int(f, 0); write_int(f, 0);
//
//    for (int y = 0; y < HEIGHT; y++) {
//        int row_start = y * WIDTH * 3;
//        f.write(reinterpret_cast<const char*>(&image[row_start]), WIDTH * 3);
//        for (int p = 0; p < row_padded - WIDTH * 3; ++p)
//            f.put(0);
//    }
//
//    f.close();
//}
//
//int main() {
//    queue q;
//    std::cout << "🔁 Generating " << TOTAL_FRAMES << " Mandelbrot frames...\n";
//
//    for (int frame = 0; frame < TOTAL_FRAMES; ++frame) {
//        float zoom = std::pow(0.95f, frame); // exponential zoom
//        float centerX = -0.75f;
//        float centerY = 0.0f;
//        float scale = 2.0f * zoom;
//
//        std::vector<uint8_t> image(WIDTH * HEIGHT * 3);
//        {
//            buffer<uint8_t> img_buf(image.data(), range<1>(image.size()));
//
//            q.submit([&](handler& h) {
//                auto acc = img_buf.get_access<access::mode::write>(h);
//
//                h.parallel_for(range<2>(HEIGHT, WIDTH), [=](id<2> idx) {
//                    int y = idx[0], x = idx[1];
//                    float cr = centerX + (x - WIDTH / 2.0f) * scale / WIDTH;
//                    float ci = centerY + (y - HEIGHT / 2.0f) * scale / HEIGHT;
//                    float zr = 0, zi = 0;
//                    int iter = 0;
//                    while (zr * zr + zi * zi < 4.0f && iter < MAX_ITER) {
//                        float tmp = zr * zr - zi * zi + cr;
//                        zi = 2.0f * zr * zi + ci;
//                        zr = tmp;
//                        iter++;
//                    }
//
//                    float t = (float)iter / MAX_ITER;
//                    uint8_t r = (uint8_t)(9 * (1 - t) * t * t * t * 255);
//                    uint8_t g = (uint8_t)(15 * (1 - t) * (1 - t) * t * t * 255);
//                    uint8_t b = (uint8_t)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
//
//                    int offset = ((HEIGHT - 1 - y) * WIDTH + x) * 3;
//                    acc[offset] = b;
//                    acc[offset + 1] = g;
//                    acc[offset + 2] = r;
//                });
//            });
//            q.wait();
//        }
//
//        std::ostringstream oss;
//        oss << "frame_" << std::setw(4) << std::setfill('0') << frame << ".bmp";
//        save_bmp(image, oss.str());
//        std::cout << "Saved " << oss.str() << "\n";
//    }
//
//    std::cout << "🎬 Done! Use ffmpeg to create video: \n";
//    std::cout << "   ffmpeg -framerate 10 -i frame_%04d.bmp -pix_fmt yuv420p zoom.mp4\n";
//    return 0;
//}
