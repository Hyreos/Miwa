#pragma once

#include <unordered_map>
#include "../audio/emitter.hpp"

namespace miwa {
    class AudioSystem {
    public:
        AudioSystem() {
            set_device(alcOpenDevice(nullptr));
            set_context(alcCreateContext(get_device(), nullptr));
            assert(make_context_current() != ALC_FALSE);
        }

        AudioSystem(AudioSystem&) = delete;

        AudioSystem(AudioSystem&&) = delete;

        virtual ~AudioSystem() {
            alcDestroyContext(get_context());
            alcCloseDevice(get_device());
        }

        inline bool make_context_current() noexcept {
            return alcMakeContextCurrent(get_context());
        }

        inline void set_device(gsl::not_null<ALCdevice*> device) noexcept {
            device_ = device;
        }

        inline ALCdevice* get_device() noexcept {
            return device_;
        }

         inline void set_context(gsl::not_null<ALCcontext*> context) noexcept {
            context_ = context;
        }

        inline ALCcontext* get_context() noexcept {
            return context_;
        }
    private:
        gsl::owner<ALCdevice*> device_;
        gsl::owner<ALCcontext*> context_;
    };
}