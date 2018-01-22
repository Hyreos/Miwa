#pragma once

#include "texture.hpp"
#include "renderable.hpp"
#include <gsl/gsl>
#include <linmath.h>
#include <array>
#include "../system/locator.hpp"

namespace miwa {
    class Sprite : public IRenderable {
    public:
        Sprite(Sprite&) = default;

        Sprite(Sprite&&) = default;

        Sprite() {
            reset_model();
        }

        Sprite(Texture& texture) : texture_(&texture) {
            reset_model();
            rect_ = {
                0, 0,
                texture.get_width(),
                texture.get_height()
            };
        }

        inline void translate(gsl::span<float, 2> pos) noexcept {
            mat4x4_translate(model_, pos[0], pos[1], 0.f);
        }

        inline void rotate(float a) noexcept {
            mat4x4_rotate_Z(model_, model_, a);
        }
            
        inline void draw() {
            using namespace miwa;

            assert(Locator::get().has_render_system());
            assert(Locator::get().has_shader_system());

            mat4x4 diff;
            mat4x4_sub(diff, model_, draw_model_);
            mat4x4_add(draw_model_, draw_model_, diff);

            assert(texture_ != nullptr);

            const auto& texture = get_texture();
                
            const auto& shader_manager = Locator::get().get_shader_system();
            const auto& render_system = Locator::get().get_render_system();

            const auto& c_shader = shader_manager.current();
            const auto& c_view = render_system.get_view();

            texture.bind();

            texture.get_vertex_array().bind();
            texture.get_vertex_buffer().bind();
            texture.get_element_buffer().bind();
                
            const vec2 uv = { rect_[0], rect_[1] };
            mat4x4_translate(draw_model_, -uv[0], -uv[1], 0.f);

            c_shader.uniform<mat4x4>(c_shader.get_location("model"), (GLfloat*)draw_model_);
            c_shader.uniform<int>(c_shader.get_location("tex"), 0);

            mat4x4& view_matrix = c_view.get_matrix();
            c_shader.uniform<mat4x4>(c_shader.get_location("view"), (GLfloat*)view_matrix);

            const vec2 tsize = { texture_->get_width(), texture_->get_height() };
            const vec2 uvlen = { rect_[2], rect_[3] };

            c_shader.uniform<float>(c_shader.get_location("uv"), uv[0], uv[1]);
            c_shader.uniform<float>(c_shader.get_location("tex_size"), tsize[0], tsize[1]);
            c_shader.uniform<float>(c_shader.get_location("crop_size"), uvlen[0], uvlen[1]);

            auto& projection = render_system.get_projection_matrix();
            c_shader.uniform<mat4x4>(c_shader.get_location("projection"), (GLfloat*)projection);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
                
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            
            reset_model();
        }

        inline Texture& get_texture() noexcept {
            return *texture_;
        }

        inline void set_texture(Texture& texture) noexcept {
            texture_ = &texture;
        }

        inline void set_rect(gsl::span<int, 4> rect) noexcept {
            std::copy(rect.begin(), rect.end(), rect_.begin());
        }
        
        inline void reset_model() noexcept {
            gsl::multi_span<float, 4, 4> mspan = model_;
            std::fill(mspan.begin(), mspan.end(), 0.f);
            mat4x4_identity(model_);
        }

    private:
        std::array<int, 4> rect_;
        mat4x4 model_;
        mat4x4 draw_model_;
        Texture* texture_;
    };
}