#pragma once 

#include <GLFW/glfw3.h>
#include <linmath.h>
#include <string_view>
#include <gsl/gsl>

namespace  miwa {
    class Window {
    public:
         Window(vec2 pos, vec2 size, std::string_view title) {
            assert(glfwInit());
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            window_ = glfwCreateWindow(size[0], size[1], title.data(), nullptr, nullptr);
            glfwSetFramebufferSizeCallback(get(), &on_resize);
        }

        virtual ~Window() {
            glfwDestroyWindow(get());
        }

        GLFWwindow* get() noexcept {
            return window_;
        }

        void resize(vec2 size) noexcept {
            glfwSetWindowSize(get(), size[0], size[1]);
        }

        void move(vec2 pos) noexcept {
            glfwSetWindowPos(get(), pos[0], pos[1]);
        }

        void set_caption(std::string_view title) noexcept {
            glfwSetWindowTitle(get(), title.data());
        }

        void present() noexcept {
            glfwSwapBuffers(get());
        }

        void make_context_current() noexcept {
            glfwMakeContextCurrent(get());
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        }

        void poll_events() noexcept {
            glfwPollEvents();
            int w, h;
            glfwGetFramebufferSize(window_, &w, &h);
        }

        bool should_close() noexcept {
            return glfwWindowShouldClose(get());
        }

        const std::pair<int, int> get_screen_size() noexcept {
            int w, h;
            glfwGetWindowSize(get(), &w, &h);
            return { w, h };
        }
    private:
        static void on_resize(GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        }
        gsl::owner<GLFWwindow*> window_;
    };
}