#include "constants.h"
#include "olcPixelGameEngine.h"

constexpr auto BYTES_PER_PIXEL = 3;
constexpr auto IMAGE_WIDTH = WIDTH;
constexpr auto IMAGE_STRIDE = IMAGE_WIDTH * BYTES_PER_PIXEL;
constexpr auto IMAGE_HEIGHT = HEIGHT;

struct rgb {
    unsigned char r, g, b;
};

class Viewer : public olc::PixelGameEngine {
  public:
    Viewer(std::vector<unsigned char *> &images) : m_images(images) {
        // Name your application
        sAppName = "Viewer";
    }

  public:
    bool OnUserCreate() override {
        // Called once at the start, so create things here
        for (int y = 0; y < ScreenHeight(); y++) {
            for (int x = 0; x < ScreenWidth(); x++) {
                unsigned char r, g, b;
                auto pixel = at(0, x, y);
                Draw(x, y, olc::Pixel(pixel.r, pixel.g, pixel.b));
            }
        }
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        // Called once per frame, draws random coloured pixels
        if (!thread_running() && GetKey(olc::Key::SPACE).bPressed) {
            m_kernel_size = 1;
            start_diff_thread();
        }

        if (int delta = GetMouseWheel(); delta != 0 && !thread_running()) {
            if (delta > 0) {
                m_kernel_size += 2;
                std::cout << m_kernel_size << std::endl;
                start_diff_thread();
            } else if (m_kernel_size > 1) {
                m_kernel_size -= 2;
                std::cout << m_kernel_size << std::endl;
                start_diff_thread();
            }
        }

        return true;
    }

  private:
    void start_diff_thread();
    bool thread_running();
    void do_diff();

    rgb &at(int image, int x, int y) {
        // std::cout << "at(" << image << ',' << x << ',' << y << ')' << std::endl;
        rgb *pixel = (rgb *)&m_images[image][(IMAGE_STRIDE * y) + (BYTES_PER_PIXEL * x)];
        return *pixel;
    }

    std::vector<unsigned char *> &m_images;
    int m_kernel_size = 1;
};
