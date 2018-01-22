#pragma once 
#include <string_view>
#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <gsl/gsl>
#include <array>
#include <fstream>

#include "internal_values.hpp"

namespace miwa::internal {
    class OGGloader {
    public:
        OGGloader() = default;
        OGGloader(OGGloader&) = delete;
        OGGloader(OGGloader&&) = delete;

        static inline OGGloader& get() noexcept {
            static OGGloader* self = new OGGloader();
            return *self;
        }

        inline void load(std::string_view fname, std::vector<char>& data, ALint& format, ALint& freq) {
            int endian = 0;
            int bit_stream;
            long bytes;

            char array[internal::buffer_size];

            FILE* file;
            file = fopen(fname.data(), "rb");
            assert(file);
            vorbis_info *vb_info;
            OggVorbis_File vb_file;

            ov_open(file, &vb_file, nullptr, 0);
            vb_info = ov_info(&vb_file, -1);

            if(vb_info->channels == 1)
                format = AL_FORMAT_MONO16;
            else if (vb_info->channels == 2)
                format = AL_FORMAT_STEREO16;

            freq = vb_info->rate;

            do {
                bytes = ov_read(&vb_file, array, internal::buffer_size, endian, 2, 1, &bit_stream);
                data.insert(data.end(), array, array + bytes);
            } while(bytes > 0);

            ov_clear(&vb_file);
            fclose(file);
        }
    };
}