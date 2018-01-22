#pragma once
#include "../types.hpp"

namespace miwa {
        struct TexProperties {
            int32_t width;
            int32_t height;
            uint32_t id;
            uint32_t target;
            uint32_t format;
            uint32_t mipmap_level;
    };
}