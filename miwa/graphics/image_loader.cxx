 #include "image_loader.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>
#include <unordered_map>
#include <functional>

namespace miwa::priv {
    unsigned char* ImageLoader::load_data(std::string_view fname, int& w, int& h, int& channels) {
        return stbi_load(fname.data(), &w, &h, &channels, 0);
    }
}