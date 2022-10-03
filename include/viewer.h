#include "constants.h"
#include "olcPixelGameEngine.h"

constexpr auto BYTES_PER_PIXEL = 3;
constexpr auto IMAGE_WIDTH = 6000;
constexpr auto IMAGE_STRIDE = IMAGE_WIDTH * BYTES_PER_PIXEL;
constexpr auto IMAGE_HEIGHT = 4000;

constexpr auto KERNEL_SIZE = 11;
constexpr auto KERNEL = KERNEL_SIZE / 2;

struct rgb {
    unsigned char r, g, b;
};

class Viewer : public olc::PixelGameEngine {
  public:
    Viewer(unsigned char const *const data) : m_data(data) {
        // Name your application
        sAppName = "Viewer";
    }

  public:
    bool OnUserCreate() override {
        // Called once at the start, so create things here
        for (int y = 0; y < ScreenHeight(); y++) {
            for (int x = 0; x < ScreenWidth(); x++) {
                unsigned char r, g, b;
                auto pixel = at(x, y);
                unsigned long long count = 0, sumr = 0, sumg = 0, sumb = 0;
                for (int iy = -KERNEL; iy <= KERNEL; ++iy) {
                    if (y + iy < 0 || y + iy >= HEIGHT - 1)
                        continue;
                    for (int ix = -KERNEL; ix <= KERNEL; ++ix) {
                        if (x + ix < 0 || x + ix >= WIDTH - 1 || (ix == 0 && iy == 0))
                            continue;
                        ++count;
                        auto ref_pixel = at(x + ix, y + iy);
                        auto diffr = std::abs(pixel.r - ref_pixel.r);
                        sumr += diffr;
                        auto diffg = std::abs(pixel.g - ref_pixel.g);
                        sumg += diffg;
                        auto diffb = std::abs(pixel.b - ref_pixel.b);
                        sumb += diffb;
                    }
                }
                sumr /= count;
                sumg /= count;
                sumb /= count;
                auto general = (sumr + sumb + sumg) / 3;
                Draw(x, y, olc::Pixel(general, general, general));
            }
        }

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        // Called once per frame, draws random coloured pixels
        if (GetKey(olc::Key::SPACE).bPressed) {
            if (mode = !mode) {
                for (int y = 0; y < ScreenHeight(); y++) {
                    for (int x = 0; x < ScreenWidth(); x++) {
                        unsigned char r, g, b;
                        auto pixel = at(x, y);
                        Draw(x, y, olc::Pixel(pixel.r, pixel.g, pixel.b));
                    }
                }
            } else {
                for (int y = 0; y < ScreenHeight(); y++) {
                    for (int x = 0; x < ScreenWidth(); x++) {
                        unsigned char r, g, b;
                        auto pixel = at(x, y);
                        unsigned long long count = 0, sumr = 0, sumg = 0, sumb = 0;
                        for (int iy = -KERNEL; iy <= KERNEL; ++iy) {
                            if (y + iy < 0 || y + iy >= HEIGHT - 1)
                                continue;
                            for (int ix = -KERNEL; ix <= KERNEL; ++ix) {
                                if (x + ix < 0 || x + ix >= WIDTH - 1 || (ix == 0 && iy == 0))
                                    continue;
                                ++count;
                                auto ref_pixel = at(x + ix, y + iy);
                                auto diffr = std::abs(pixel.r - ref_pixel.r);
                                sumr += diffr;
                                auto diffg = std::abs(pixel.g - ref_pixel.g);
                                sumg += diffg;
                                auto diffb = std::abs(pixel.b - ref_pixel.b);
                                sumb += diffb;
                            }
                        }
                        sumr /= count;
                        sumg /= count;
                        sumb /= count;
                        auto general = (sumr + sumb + sumg) / 3;
                        Draw(x, y, olc::Pixel(general, general, general));
                    }
                }
            }
        }
        return true;
    }

  private:
    rgb &at(int x, int y) {
        rgb *pixel = (rgb *)&m_data[(IMAGE_STRIDE * y * 5) + (BYTES_PER_PIXEL * x * 5)];
        return *pixel;
    }
    unsigned char const *const m_data;
    bool mode = false;
};
