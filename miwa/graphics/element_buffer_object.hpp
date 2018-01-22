#pragma once

#include "glloader.hpp"
#include "glchecker.hpp"

namespace miwa::priv {
    class ElementBuffer {
    public:
        ElementBuffer() {
            gl_check(glGenBuffers(1, &gl_id_));
        }

        virtual ~ElementBuffer() {
            gl_check(glDeleteBuffers(1, &get()));
        }

        inline void bind() noexcept {
            gl_check(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, get()));
        }

        inline GLuint& get() noexcept {
            return gl_id_;
        }
    private:
        GLuint gl_id_;
    };
}
