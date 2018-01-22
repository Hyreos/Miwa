#pragma once
#include <linmath.h>
#include <gsl/gsl>

namespace miwa {
    class View {
    public:
        View() {
            mat4x4_identity(m);
        }

        void translate(gsl::span<float, 2> pos) noexcept {
            mat4x4_translate(m, -pos[0], -pos[1], 0.f);
        }

        void resize(gsl::span<float, 2> size) noexcept {
            mat4x4_ortho(m, 0, size[0], size[1], 0, 1.f, -1.f);
        }

        inline mat4x4& get_matrix() noexcept {
            return m;
        }
    private:
        mat4x4 m;
    };
}