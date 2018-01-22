#pragma once

namespace miwa::priv {
    class VertexArray {
    public:
        VertexArray() {
            gl_check(glGenVertexArrays(1, &gl_id_));
        }

        virtual ~VertexArray() {
            gl_check(glDeleteVertexArrays(1, &get()));
        }

        inline void bind() noexcept {
            gl_check(glBindVertexArray(get()));
        }

        inline GLuint& get() noexcept {
            return gl_id_;
        }
        private:
            GLuint gl_id_;
        };
}