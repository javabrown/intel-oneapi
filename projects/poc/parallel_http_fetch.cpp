// This require param to link: -lcurl EXAMPLE :dpcpp parallel_http_fetch.cpp -lcurl 
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <curl/curl.h>

std::mutex print_mutex;

// Callback for writing response (we discard it here)
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    return size * nmemb;
}

void fetch_url(const std::string& url, int id) {
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        CURLcode res = curl_easy_perform(curl);
        {
            std::lock_guard<std::mutex> lock(print_mutex);
            std::cout << "Thread " << id << " -> Status: " << (res == CURLE_OK ? "OK" : "FAIL") << "\n";
        }
        curl_easy_cleanup(curl);
    }
}

int main() {
    std::vector<std::thread> threads;
    std::string target_url = "https://httpbin.org/get"; // test endpoint

    int total_requests = 20; // Try 100 or 1000 if you want!

    curl_global_init(CURL_GLOBAL_ALL); // libcurl global init

    for (int i = 0; i < total_requests; ++i) {
        threads.emplace_back(fetch_url, target_url, i);
    }

    for (auto& t : threads) t.join();

    curl_global_cleanup();
    std::cout << "All HTTP requests done.\n";
    return 0;
}
