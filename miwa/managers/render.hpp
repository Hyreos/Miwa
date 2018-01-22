#pragma once
#include "../window/window.hpp"
#include "../graphics/view.hpp"

namespace miwa {
    class RenderSystem {
    public:
        RenderSystem() = delete;

        RenderSystem(RenderSystem&) = delete;

        RenderSystem(RenderSystem&&) = delete;

        explicit RenderSystem(Window& window) noexcept : view_(&default_view_), window_(&window) {
            auto[wx, wy] = get_framebuffer_size();
            mat4x4_ortho(projection_matrix_, -0.f, wx, wy, 0.f, 0.f, -1.f);
            window.make_context_current();
        }

        virtual ~RenderSystem() = default;

        inline mat4x4& get_projection_matrix() noexcept {
            auto[wx, wy] = get_framebuffer_size();
            mat4x4_ortho(projection_matrix_, -0.f, wx, wy, 0.f, 0.f, -1.f);
            return projection_matrix_;
        }

        inline void set_view(View& view) noexcept {
            view_ = &view;
        }

        inline View& get_view() noexcept {
            return *view_;
        }

        inline View& get_default_view() noexcept {
            return default_view_;
        }

        inline const std::pair<int, int> get_framebuffer_size() noexcept {
            int w, h;
            glfwGetFramebufferSize(get_window().get(), &w, &h);
            return { w, h };
        }

        inline Window& get_window() noexcept {
            return *window_;
        }
    private:
        mat4x4 projection_matrix_;

        View* view_;
        View default_view_;

        Window* window_;
    };
}