#pragma once
#include "glloader.hpp"
#include "glchecker.hpp"

namespace miwa::priv {
    class TextureBuffer {
    public:
        TextureBuffer() {
            gl_check(glGenTextures(1, &id_));
        }

        virtual ~TextureBuffer() {
            gl_check(glDeleteTextures(1, &id_));
        }

        inline GLuint& get() noexcept {
            return id_;
        }

        inline void bind() noexcept {
            gl_check(glBindTexture(GL_TEXTURE_2D, get()));
        }
    private:
        GLuint id_;
    };
}