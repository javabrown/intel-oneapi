#include <CL/sycl.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace sycl;

std::vector<std::string> split_words(const std::string& input) {
    std::vector<std::string> words;
    std::string word;
    for (char c : input) {
        if (std::isalnum(c))
            word += std::tolower(c);
        else if (!word.empty()) {
            words.push_back(word);
            word.clear();
        }
    }
    if (!word.empty())
        words.push_back(word);
    return words;
}

int main() {
    std::string text = "FPGA HDL OneAPI DPC++ FPGA Verilog HDL DPC++ FPGA";

    auto words = split_words(text);
    std::map<std::string, int> freq;
    std::vector<int> counters(words.size(), 1);

    queue q;
    {
        buffer<int> freq_buf(counters.data(), range<1>(counters.size()));
        q.submit([&](handler& h) {
            auto acc = freq_buf.get_access<access::mode::read>(h);
            h.parallel_for(range<1>(words.size()), [=](id<1> i) {
                // Simulated: each word has weight 1
                // In real world, you’d hash words & reduce frequency
            });
        });
    }

    // Count words serially (you can move this to device later with hashing)
    for (const auto& word : words)
        freq[word]++;

    // Find most frequent word
    auto max_it = std::max_element(freq.begin(), freq.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

    std::cout << "Most frequent word: " << max_it->first << " (" << max_it->second << " times)" << std::endl;

    // Answer sample question
    std::cout << "Frequency of 'HDL': " << freq["hdl"] << std::endl;

    return 0;
}
