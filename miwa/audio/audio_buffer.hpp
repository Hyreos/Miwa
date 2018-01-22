#pragma once 

#include <AL/al.h>
#include <AL/alc.h>
#include <vector>
#include <string_view>
#include "ogg_loader.hpp"
#include "alchecker.hpp"

namespace miwa {
    class AudioBuffer {
        public:
            AudioBuffer() {
                init();
            }

            AudioBuffer(std::string_view fname) {
                init();
                load_from_file(fname);
            }

            virtual ~AudioBuffer() noexcept {
                al_check(alDeleteBuffers(1, &buffer_id_));
            }

            inline void load_from_file(std::string_view fname) noexcept {
                std::vector<char> temp;
                int format, freq;
                internal::OGGloader::get().load(fname, temp, format, freq);
                al_check(alBufferData(buffer_id_, format, temp.data(), temp.size(), freq));
            }

            inline ALuint& get_al_id() noexcept {
                return buffer_id_;
            }
        private:
            void init() noexcept {
                al_check(alGenBuffers(1, &buffer_id_));
            }
            ALuint buffer_id_;
    };
}