#pragma once

#include "glloader.hpp"
#include "glchecker.hpp"

namespace miwa::priv {
    class VertexBuffer {
    public:
        VertexBuffer() {
            gl_check(glGenBuffers(1, &gl_id_));
        }

        virtual ~VertexBuffer() {
            gl_check(glDeleteBuffers(1, &get()));
        }

        inline void bind() noexcept {
            gl_check(glBindBuffer(GL_ARRAY_BUFFER, get()));
        }

        inline GLuint& get() noexcept {
            return gl_id_;
        }
    private:
        GLuint gl_id_;
    };
}