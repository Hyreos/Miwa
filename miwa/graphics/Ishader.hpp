#pragma once

namespace miwa {
    class IShader {
    public:
        virtual inline uint32_t& get() noexcept = 0;

        virtual inline void check_compile_status() const = 0;
    };
}