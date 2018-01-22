#pragma once

#include "Ishader.hpp"
#include "glloader.hpp"
#include "glchecker.hpp"
#include <iostream>

namespace miwa {
    class VertexShader : public IShader {
    public:
        VertexShader() = delete;

        VertexShader(std::string_view source) {
            gl_check(id = glCreateShader(GL_VERTEX_SHADER));
            const GLchar* source_ptr = source.data();
            gl_check(glShaderSource(id, GLsizei(1), &source_ptr, nullptr));
            gl_check(glCompileShader(id));
            check_compile_status();
        }

        inline void check_compile_status() const override {
            assert(std::invoke([=]() -> bool {
                GLint success;
                glGetShaderiv(id, GL_COMPILE_STATUS, &success);
                if(!success) {
                    int len = 0;
                    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
                    std::string errstr;
                    errstr.resize(len);
                    glGetShaderInfoLog(id, errstr.size(), nullptr, errstr.data());
                    std::cerr << errstr << '\n';
                    return false;
                }
                return true;
            }));
        }

        virtual ~VertexShader() {
            glDeleteShader(get());
        }

        inline GLuint& get() noexcept override {
            return id;
        }
    private:
        GLuint id;
    };
}