#pragma once

#include <array>
#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>
#include <algorithm>
#include <string_view>
#include <AL/al.h>
#include <AL/alc.h>
#include <cassert>
#include <vector>
#include <functional>
#include "audio_buffer.hpp"
#include "alchecker.hpp"

namespace miwa {
    enum class EmitterStates {
        Playing, Stopped, Paused, End
    };

    class Emitter {
    private:
        volatile EmitterStates state_;
        std::unordered_map<ALuint, std::reference_wrapper<AudioBuffer>> buffers_;
        ALuint source_id_;
    public:
        Emitter() noexcept {
            al_check(alGenSources(1, &source_id_));
            state_ = EmitterStates::Stopped;
        }

        virtual ~Emitter() {
            al_check(alSourcei(source_id_, AL_BUFFER, 0));
            al_check(alDeleteSources(1, &source_id_));   
        }

        inline void attach_audio_buffer(AudioBuffer& buffer) noexcept {
            al_check(alSourceQueueBuffers(source_id_, 1, &buffer.get_al_id()));
            buffers_.insert(buffers_.end(), { buffer.get_al_id(), std::ref(buffer) });
        }

        inline void detach_audio_buffer(AudioBuffer& buffer) noexcept {
            al_check(alSourceUnqueueBuffers(source_id_, 1, &buffer.get_al_id()));
            auto it = buffers_.find(buffer.get_al_id());
            assert(it == buffers_.end());
            buffers_.erase(it);
        }

        inline void detach_all_audio_buffers() noexcept {
            al_check(alSourcei(source_id_, AL_BUFFER, 0));
            buffers_.clear();
        }

        inline void play() noexcept {
            al_check(alSource3f(source_id_, AL_POSITION, 0.f, 0.f, 0.f));
            al_check(alSourcePlay(source_id_));
            ALint nbq = 0;
            al_check(alGetSourcei(get_source_id(), AL_BUFFERS_QUEUED, &nbq));
            assert(nbq != 0);
            state_ = EmitterStates::Playing;
        }

        inline void stop() noexcept {
            al_check(alSourceStop(source_id_));
            state_ = EmitterStates::Stopped;
        }

        inline void pause() noexcept {
            al_check(alSourcePause(source_id_));
            state_ = EmitterStates::Paused;
        }

        inline void set_volume(float vol) noexcept {
            al_check(alSourcef(source_id_, AL_GAIN, vol));
        }

        inline float get_volume() const noexcept {
            float vol;
            al_check(alGetSourcef(source_id_, AL_GAIN, &vol));
            return vol;
        }

        inline ALint const get_source_id() const noexcept {
            return source_id_;
        }

        inline std::unordered_map<ALuint, std::reference_wrapper<AudioBuffer>>& get_buffers() noexcept {
            return buffers_;
        }

        inline EmitterStates const get_state() noexcept {
            ALint state;
            al_check(alGetSourcei(source_id_, AL_SOURCE_STATE, &state));
            switch(state) {
                case AL_STOPPED:
                    return EmitterStates::Stopped;
                    break;
                case AL_PLAYING:
                    return EmitterStates::Playing;
                    break;
                case AL_PAUSED:
                    return EmitterStates::Paused;
                    break;
            }
            return EmitterStates::End;
        }

        inline void all_buffers(std::function<void(AudioBuffer&)>&& f) {
            for(auto&[id, buff] : buffers_)
                f(buff);
        }

        inline void wait() noexcept {
            do {
                state_ = get_state();
            } while(state_ != EmitterStates::Stopped);
        }
    };
}