#include <iostream>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
static void my_progress_report(float progress);
#define STBIR_PROGRESS_REPORT(val) my_progress_report(val)
#include "stb_image_resize.h"

static void my_progress_report(float progress) {
    std::cerr.precision(0);
    if (progress < 1)
        std::cerr << std::fixed << "\rProgress: " << progress * 100 << '%' << std::flush;
    else
        std::cerr << "\rProgress: 100%" << std::endl;
}
