#include <string_view>
#include <vcruntime_string.h>
#include <vector>

#include "constants.h"
#include "stb_image.h"
#include "stb_image_resize.h"
#include "util.h"
#include "viewer.h"

int main(int argc, char **argv) {
    std::vector<std::string_view> args(argv + 1, argv + argc);
    std::vector<unsigned char *> images(args.size(), nullptr);

    int i = 0;
    for (const auto &arg : args) {
        int width, height, bytes_per_pixel;
        unsigned char *data = stbi_load(arg.data(), &width, &height, &bytes_per_pixel, 0);
        std::cout << arg << ": " << FormatWithCommas(width) << 'x' << FormatWithCommas(height) << " ("
                  << (bytes_per_pixel) << "Bpp)" << std::endl;
        images[i] = new unsigned char[WIDTH * HEIGHT * bytes_per_pixel];
        if (width != WIDTH || height != HEIGHT) {
            std::cout << "Resizing..." << std::endl;
            if (!stbir_resize_uint8(data, width, height, 0, images[i], WIDTH, HEIGHT, 0, bytes_per_pixel)) {
                std::cerr << "Failed!" << std::endl;
                return 1;
            }
            std::cout << "Done!" << std::endl;
        } else {
            memcpy(images[i], data, WIDTH * HEIGHT * bytes_per_pixel);
        }
        stbi_image_free(data);
        ++i;
    }

    Viewer viewer(images);
    if (viewer.Construct(WIDTH, HEIGHT, ZOOM, ZOOM))
        viewer.Start();

    for (unsigned char *image : images)
        delete[] image;

    return 0;
}
