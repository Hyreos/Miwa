#pragma once

#include <array>
#include "texture_buffer_object.hpp"
#include "vertex_array_object.hpp"
#include "vertex_buffer_object.hpp"
#include "element_buffer_object.hpp"
#include "glloader.hpp"
#include "glchecker.hpp"

namespace miwa {
    class FrameBuffer {
    public:
        FrameBuffer() {
            gl_check(glGenFramebuffers(1, &get()));
        }

        virtual ~FrameBuffer() {
            gl_check(glDeleteFramebuffers(1, &get()));
        }

        inline void bind() {
            gl_check(glBindFramebuffer(GL_FRAMEBUFFER, get()));
        }

        inline GLuint& get() noexcept {
            return fbo_;
        }
    private:
        GLuint fbo_;
    };

    class RenderBuffer {
    public:
        RenderBuffer() {
            gl_check(glGenRenderbuffers(1, &get()));
            gl_check();
        }

        void create(size_t w, size_t h) {
            bind();
            properties_.width = w;
            properties_.height = h;
            gl_check(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, get_properties().width, get_properties().height));
            gl_check(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, get()));

            GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
            gl_check(glDrawBuffers(1, draw_buffers));
            assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        }

        virtual ~RenderBuffer() {
            gl_check(glDeleteRenderbuffers(1, &get()));
        }

        inline void bind() noexcept {
            gl_check(glBindRenderbuffer(GL_RENDERBUFFER, get()));
        }

        inline GLuint& get() noexcept {
            return rbo_;
        }

        class Properties {
        public:
            Properties() = default;
            virtual ~Properties() = default;
            GLuint width;
            GLuint height;
        };

        const Properties& get_properties() noexcept {
            return properties_;
        }
    private:
        Properties properties_;
        GLuint rbo_;
    };

    class RenderTexture {
    public:
        RenderTexture() = default;

        RenderTexture(size_t w, size_t h) noexcept {
            create(w, h);
        }

        inline void create(size_t w, size_t h) noexcept {
            get_texture().create(w, h);
            get_frame_buffer().bind();
            get_render_buffer().create(w, h);
            gl_check(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, get_texture().get_texture_buffer().get(), 0));
            gl_check(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }

        inline void bind() noexcept {
            get_frame_buffer().bind();
        }

        inline Texture& get_texture() noexcept {
            return texture_;
        }

        inline FrameBuffer& get_frame_buffer() noexcept {
            return fbo_;
        }

        inline RenderBuffer& get_render_buffer() noexcept {
            return rbo_;
        }

        inline size_t get_width() const noexcept {
            return get_texture().get_properties().width;
        }

        inline size_t get_height() const noexcept {
            return get_texture().get_properties().height;
        }
    private:
        RenderBuffer rbo_;
        FrameBuffer fbo_;
        Texture texture_;
    };
}