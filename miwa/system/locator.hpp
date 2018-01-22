#pragma once 
#include <gsl/gsl>
#include "../managers/audio.hpp"
#include "../managers/shaders.hpp"
#include "../managers/worlds.hpp"
#include "../managers/render.hpp"

namespace miwa {
    class Locator {
    public:
        Locator() = default;

        Locator(Locator&) = delete;

        Locator(Locator&&) = delete;

        virtual ~Locator() = default;

        static inline Locator& get() noexcept {
            static Locator* self = new Locator();
            return *self;
        }

        inline void add_audio_system(AudioSystem& audio_system) noexcept {
            audio_system_ = &audio_system;
        }

        inline void add_world_system(WorldSystem& world_system) noexcept {
            world_system_ = &world_system;
        }

        inline void add_shader_system(ShaderSystem& shader_system) noexcept {
            shader_system_ = &shader_system;
        }

        inline void add_render_system(RenderSystem& render_system) noexcept  {
            render_system_ = &render_system;
        }

        inline AudioSystem& get_audio_system() noexcept {
            assert(audio_system_ != nullptr);
            return *audio_system_;
        }

        inline WorldSystem& get_world_system() noexcept {
            assert(world_system_ != nullptr);
            return *world_system_;
        }

        inline ShaderSystem& get_shader_system() noexcept {
            assert(shader_system_ != nullptr);
            return *shader_system_;
        }

        inline RenderSystem& get_render_system() noexcept {
            return *render_system_;
        }

        inline const bool has_render_system() const noexcept {
            return render_system_ != nullptr;
        }

        inline const bool has_shader_system() const noexcept {
            return shader_system_ != nullptr;
        }

        inline const bool has_world_system() const noexcept {
            return world_system_ != nullptr;
        }

        inline const bool has_audio_system() const noexcept {
            return audio_system_ != nullptr;
        }
    private:
        WorldSystem* world_system_;
        ShaderSystem* shader_system_;
        AudioSystem* audio_system_;
        RenderSystem* render_system_;
    };
}