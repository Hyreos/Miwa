#pragma once

#include "image_loader.hpp"
#include "glloader.hpp"
#include "glchecker.hpp"
#include "texture_buffer_object.hpp"
#include "vertex_array_object.hpp"
#include "vertex_buffer_object.hpp"
#include "element_buffer_object.hpp"
#include "blend_mode.hpp"
#include <string_view>
#include <array>
#include <tuple>
#include <functional>

namespace miwa {
    class Texture {
    public:
        Texture() = default;

        void create(size_t w, size_t h) {
            gl_check(glBindTexture(GL_TEXTURE_2D, get_texture_buffer().get()));
            gl_check(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            gl_check(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            gl_check(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GLsizei(w), GLsizei(h), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
            gl_check(glBindTexture(GL_TEXTURE_2D, 0));

            properties_.format = GL_RGBA;
            properties_.width = w;
            properties_.height = h;
            properties_.mipmap_level = 0;
            properties_.target = GL_TEXTURE_2D;
            properties_.id = get_texture_buffer().get();
        }

        template<ImageFormats ImgFormat>
        void Texture::load_from_file(std::string_view fname) {
            properties_ = priv::ImageLoader::get().load_file<ImgFormat>(fname, get_texture_buffer());

            constexpr std::array<GLuint, 6> indices =
            {
                0, 1, 3,
                1, 2, 3
            };

            const std::array<GLfloat, 20> vertices =
            {
                 properties_.width, properties_.height, 0.0f, 1.0f, 1.0f,
                 properties_.width, 0.f, 0.0f, 1.0f, 0.0f,
                 0.f, 0.f, 0.0f, 0.0f, 0.0f,
                 0.f,  properties_.height, 0.0f, 0.0f, 1.0f
            };

            const auto[vao, vbo, ebo] = std::make_tuple(
                std::ref(get_vertex_array()),
                std::ref(get_vertex_buffer()),
                std::ref(get_element_buffer()));
            
            vao.bind();
            vbo.bind();
            ebo.bind();

            gl_check(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_DYNAMIC_DRAW));
            gl_check(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_DYNAMIC_DRAW));

            gl_check(glEnableVertexAttribArray(0));
            gl_check(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, reinterpret_cast<void*>(0)));
            
            gl_check(glEnableVertexAttribArray(1));
            gl_check(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, reinterpret_cast<void*>(sizeof(GLfloat) * 3)));

            gl_check(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
            gl_check(glBindBuffer(GL_ARRAY_BUFFER, 0));
            gl_check(glBindVertexArray(0));
        }

        template<BlendModes Bm = BlendModes::Normal>
        inline void set_blend_mode() {
            if constexpr(Bm == BlendModes::Normal) {
                gl_check(glBlendEquation(GL_ZERO));
            } else if constexpr(Bm == BlendModes::Add) {
                gl_check(glBlendEquation(GL_FUNC_ADD));
            } else if constexpr(Bm ==  BlendModes::Subtract) {
                gl_check(glBlendEquation(GL_FUNC_SUBTRACT));
            } else if constexpr(Bm == BlendModes::Src) {
                gl_check(glBlendEquation(GL_SRC_COLOR));
            }
        }

        inline priv::VertexArray& get_vertex_array() noexcept {
            return vertex_array_object_;
        }

        inline priv::VertexBuffer& get_vertex_buffer() noexcept {
            return vertex_buffer_object_;
        }

        inline priv::ElementBuffer& get_element_buffer() noexcept {
            return element_buffer_object_;
        }

        inline priv::TextureBuffer& get_texture_buffer() noexcept {
            return texture_buffer_object_;
        }

        inline size_t get_width() const noexcept {
            return get_properties().width;
        }

        inline size_t get_height() const noexcept {
            return get_properties().height;
        }

        inline void Texture::bind() noexcept {
            gl_check(glActiveTexture(GL_TEXTURE0));
            const auto& props = get_properties();
            gl_check(glBindTexture(props.target, props.id));
        }

        inline TexProperties& Texture::get_properties() noexcept {
            return properties_;
        }
    private:
        priv::VertexArray vertex_array_object_;
        priv::VertexBuffer vertex_buffer_object_;
        priv::ElementBuffer element_buffer_object_;
        priv::TextureBuffer texture_buffer_object_;
        TexProperties properties_;
    };
}