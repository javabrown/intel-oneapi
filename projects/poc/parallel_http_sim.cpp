#include <CL/sycl.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using namespace sycl;

void fetch_stock_data(const std::string& stock, float& price, float& change) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500 + (rand() % 500)));
    price = 100 + (rand() % 50);         // mock price
    change = -2 + static_cast<float>(rand() % 500) / 100.0f - 2.0f;  // mock change
}

int main() {
    float priceA = 0.0f, priceB = 0.0f;
    float changeA = 0.0f, changeB = 0.0f;

    std::thread t1([&]() {
        fetch_stock_data("AAPL", priceA, changeA);
    });

    std::thread t2([&]() {
        fetch_stock_data("GOOG", priceB, changeB);
    });

    t1.join();
    t2.join();

    // Display results side-by-side
    std::cout << "\n--- AAPL ---\t\t--- GOOG ---" << std::endl;
    std::cout << "Price: " << priceA << "\t\tPrice: " << priceB << std::endl;
    std::cout << "Change: " << changeA << "%\t\tChange: " << changeB << "%" << std::endl;

    return 0;
}
