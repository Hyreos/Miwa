#pragma once
#include <string_view>
#include <stdexcept>
#include <stbi/stb_image.h>
#include "glloader.hpp"
#include "tex_properties.hpp"
#include "image_formats.hpp"
#include "texture_buffer_object.hpp"

namespace miwa::priv {
    struct ImageLoader {
        unsigned char* load_data(std::string_view fname, int& w, int& h, int& channels);
            
        template<ImageFormats imgformat>
        [[nodiscard]] TexProperties load_file(std::string_view filename, TextureBuffer& texbuff) {
            struct {
                int32_t base_level;
                int32_t max_level;
                int32_t min_filter;
                int32_t mag_filter;
                int32_t internal_format;
                int32_t format;
                int32_t type;
                ubyte_t* data;
                int32_t width;
                int32_t height;
                int32_t channels;
                } img_file;

                img_file.base_level = 0;
                img_file.max_level = 5;
                img_file.min_filter = GL_LINEAR;
                img_file.mag_filter = GL_LINEAR;
                
                img_file.internal_format = GL_RGB;
                img_file.format = GL_RGB;

                stbi_set_flip_vertically_on_load(false);

                if constexpr(imgformat == ImageFormats::Png) {            
                    img_file.internal_format = GL_RGBA;
                    img_file.format = GL_RGBA;
                } else if constexpr(imgformat == ImageFormats::Jpg) {
                    img_file.internal_format = GL_RGB;
                    img_file.format = GL_RGB;
                }

                img_file.type = GL_UNSIGNED_BYTE;
                
                img_file.data = load_data(filename, img_file.width, img_file.height, img_file.channels);

                if (!img_file.data)
                    throw std::runtime_error((std::string("no file \"") + filename.data() + "\" found").c_str());

                constexpr auto target = GL_TEXTURE_2D;
                
                gl_check(glBindTexture(target, texbuff.get()));

                gl_check(glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, img_file.base_level));
                gl_check(glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, img_file.max_level));
                gl_check(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, img_file.mag_filter));
                gl_check(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, img_file.min_filter));

                gl_check(glTexImage2D(target, 
                img_file.base_level, 
                img_file.internal_format, 
                img_file.width, 
                img_file.height, 
                0, 
                img_file.format, 
                img_file.type, 
                img_file.data));
                gl_check( glGenerateMipmap(target));

                gl_check( glBindTexture(target, 0));

            TexProperties props = { 
                img_file.width, 
                img_file.height, 
                texbuff.get(), 
                target, 
                img_file.format, 
                img_file.max_level };

            stbi_image_free(img_file.data);
                            
            return std::move(props);
        }

        [[nodiscard]] static inline ImageLoader& ImageLoader::get() {
            static ImageLoader* self_ = new ImageLoader();
            return *self_;
        }
    };
}