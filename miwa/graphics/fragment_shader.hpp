#pragma once

#include "Ishader.hpp"
#include "glloader.hpp"
#include "glchecker.hpp"
#include <iostream>

namespace miwa {
    class FragmentShader : public IShader {
    public:
        FragmentShader() = delete;

        FragmentShader(std::string_view source) {
            gl_check(id = glCreateShader(GL_FRAGMENT_SHADER));
            const GLchar* source_ptr = source.data();
            gl_check(glShaderSource(id, GLsizei(1), &source_ptr, nullptr));
            gl_check(glCompileShader(id));
            check_compile_status();
        }

        virtual ~FragmentShader() {
            gl_check(glDeleteShader(get()));
        }

        inline void check_compile_status() const override {
            assert(std::invoke([=]() -> bool {
                GLint success;
                gl_check(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
                if(!success) {
                    int len = 0;
                    gl_check(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
                    std::string errstr;
                    errstr.resize(len);
                    gl_check(glGetShaderInfoLog(id, errstr.size(), nullptr, errstr.data()));
                    std::cerr << errstr << '\n';
                    return false;
                }
                return true;
            }));
        }

        inline GLuint& get() noexcept override {
            return id;
        }
    private:
        GLuint id;
    };
}