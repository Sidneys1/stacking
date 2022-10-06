#include <chrono>
#include <future>
#include <memory>
#include <type_traits>

#include "viewer.h"

using namespace std::chrono_literals;

using worker_future = std::future<std::_Invoke_result_t<std::decay_t<void (Viewer::*)()>, std::decay_t<Viewer *>>>;
static worker_future future;

bool Viewer::thread_running() {
    if (!future.valid())
        return false;
    auto status = future.wait_for(0s);
    return status != std::future_status::ready;
}

void Viewer::start_diff_thread() {
    if (m_kernel_size == 1) {
        for (int y = 0; y < ScreenHeight(); y++) {
            for (int x = 0; x < ScreenWidth(); x++) {
                unsigned char r, g, b;
                auto pixel = at(0, x, y);
                Draw(x, y, olc::Pixel(pixel.r, pixel.g, pixel.b));
            }
        }
        return;
    }

    if (!thread_running()) {
        future = std::async(std::launch::async, &Viewer::do_diff, this);
    }
}

void Viewer::do_diff() {
    // std::cout << "IN Thread." << std::endl;
    auto kernel = m_kernel_size / 2;
    for (int y = 0; y < ScreenHeight(); y++) {
        // std::cout << "IN y." << std::endl;
        for (int x = 0; x < ScreenWidth(); x++) {
            // std::cout << "IN x." << std::endl;
            Draw(x, y, olc::YELLOW);
            int maxi = 0, maxg = 0;
            for (int i = 0; i < m_images.size(); ++i) {
                unsigned char r, g, b;
                const auto &pixel = at(i, x, y);
                unsigned long long count = 0, sum = 0;
                for (int iy = -kernel; iy <= kernel; ++iy) {
                    if (y + iy < 0 || y + iy >= HEIGHT - 1)
                        continue;
                    // std::cout << "IN iy." << std::endl;
                    for (int ix = -kernel; ix <= kernel; ++ix) {
                        if (x + ix < 0 || x + ix >= WIDTH - 1 || (ix == 0 && iy == 0))
                            continue;
                        // std::cout << "IN ix." << std::endl;
                        ++count;
                        const auto &ref_pixel = at(i, x + ix, y + iy);
                        sum += std::abs(pixel.r - ref_pixel.r) + std::abs(pixel.g - ref_pixel.g) +
                               std::abs(pixel.b - ref_pixel.b);
                    }
                }
                auto general = sum / (count * 3);
                if (general > maxg) {
                    maxg = general;
                    maxi = i;
                }
            }
            auto best_pixel = at(maxi, x, y);
            Draw(x, y, olc::Pixel(best_pixel.r, best_pixel.g, best_pixel.b));
        }
    }
    // std::cout << "Thread done." << std::endl;
}
