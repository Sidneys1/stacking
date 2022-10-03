#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "constants.h"
#include "stb_image.h"
#include "util.h"
#include "viewer.h"

int main() {
    int width, height, bytes_per_pixel;
    unsigned char *data = stbi_load("image.jpg", &width, &height, &bytes_per_pixel, 0);
    std::cout << FormatWithCommas(width) << 'x' << FormatWithCommas(height) << " (" << (bytes_per_pixel * 8) << "bpp)"
              << std::endl;

    Viewer viewer(data);
    if (viewer.Construct(WIDTH, HEIGHT, ZOOM, ZOOM))
        viewer.Start();

    stbi_image_free(data);

    return 0;
}
