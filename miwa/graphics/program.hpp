#pragma once

#include <string_view>
#include <experimental/filesystem>
#include "../../utils/utils.hpp"
#include "vertex_shader.hpp"
#include "fragment_shader.hpp"
#include "glloader.hpp"
#include "glchecker.hpp"
#include <iostream>

namespace miwa {
    class Program {
    public:
        class Location {
        public:
            Location() = delete;
            Location(uint32_t location) : location_(location) { }
            Location(Location&) = default;
            Location(Location&&) = default;

            inline const GLuint get() const noexcept {
                return location_;
            }
        private:
            GLuint location_;
        };

        Program() {
            gl_check(id = glCreateProgram());
        }

        virtual ~Program() {
            gl_check(glDeleteProgram(id));
        }

        Location get_location(std::string_view uniform) {
            return std::move(Location(glGetUniformLocation(get(), uniform.data())));
        }

        template<typename T, typename... U>
        inline void uniform(Program::Location&& location, U... v) {
            constexpr auto pn = sizeof...(v);

            static_assert(pn > 0, "no value found");
            static_assert(pn <= 4, "number of arguments is too long");

            if constexpr(pn == 1) {
                if constexpr(std::is_same_v<std::decay_t<T>, int>) {
                    gl_check(glUniform1i(location.get(), std::move(v)...));
                    constexpr bool valid_type = 1;
                } else if constexpr(std::is_same_v<std::decay_t<T>, float>) {
                    gl_check(glUniform1f(location.get(), std::move(v)...));
                } else if constexpr(std::is_same_v<std::decay_t<T>, double>) {
                    gl_check(glUniform1d(location.get(), std::move(v)...));
                } else {
                    gl_check(glUniformMatrix4fv(location.get(), 1, GL_FALSE, v...));
                }
            } else if constexpr(pn == 2) {
                if constexpr(std::is_same_v<std::decay_t<T>, float>) {
                    gl_check(glUniform2f(location.get(), std::move(v)...));
                } else if constexpr(std::is_same_v<std::decay_t<T>, int>) {
                    gl_check(glUniform2i(location.get(), std::move(v)...));
                } else {
                    gl_check(glUniform2d(location.get(), std::move(v)...));
                }
            } else if constexpr(pn == 3) {
                if constexpr(std::is_same_v<std::decay_t<T>, float>) {
                    gl_check(glUniform3f(location.get(), std::move(v)...));
                } else if constexpr(std::is_same_v<std::decay_t<T>, int>) {
                    gl_check(glUniform3i(location.get(), std::move(v)...));
                } else {
                    gl_check(glUniform3d(location.get(), std::move(v)...));
                }
            } else if constexpr(pn == 4) {
                if constexpr(std::is_same_v<std::decay_t<T>, float>) {
                    gl_check(glUniform4f(location.get(), std::move(v)...));
                } else if constexpr(std::is_same_v<std::decay_t<T>, int>) {
                    gl_check(glUniform4i(location.get(), std::move(v)...));
                } else {
                    gl_check(glUniform4d(location.get(), std::move(v)...));
                }
            }
        }

        inline void attach(IShader& shader) {
            glAttachShader(id, shader.get());
        }

        inline void load_from_memory(std::string_view&& cvert, std::string_view&& cfrag) {
            VertexShader vertex(cvert);
            FragmentShader fragment(cfrag);

            attach(vertex);
            attach(fragment);

            GLint success;

            glLinkProgram(get());
            glGetProgramiv(get(), GL_LINK_STATUS, &success);

            if(!success) {
                int len = 0;
                glGetProgramiv(get(), GL_INFO_LOG_LENGTH, &len);
                std::string errstr;
                errstr.resize(len);
                std::cout << len << '\n';
                glGetProgramInfoLog(get(), errstr.size(), nullptr, errstr.data());
                std::cerr << errstr << '\n';
            }
        }

        inline GLuint& get() noexcept {
            return id;
        }
    private:
        GLuint id;
    };
}