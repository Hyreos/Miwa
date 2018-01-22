#pragma once

#include"../graphics/program.hpp"
#include <unordered_map>
#include <gsl/gsl>
#include <string_view>
#include <cassert>

namespace miwa {
    class ShaderSystem {
        public:
        using key_type = std::string_view;
        using value_type = Program;

        ShaderSystem() {
            default_program_.load_from_memory(
                #include "../graphics/shaders/default.vert"
                ,
                #include "../graphics/shaders/default.frag"
            );
            bind(default_program_);
        }
            
        ShaderSystem(ShaderSystem&) = delete;
        
        ShaderSystem(ShaderSystem&&) = delete;

        virtual ~ShaderSystem() = default;

        inline value_type& add(key_type&& unique_identifier) noexcept {
            const auto it = shader_map_.insert(shader_map_.end(), {
                unique_identifier, 
                std::make_unique<value_type>()
            });

            return *it->second;
        }

        inline value_type& resolve(key_type&& unique_identifier) {
            auto it = shader_map_.find(unique_identifier);
            assert(it != shader_map_.end());
            return *it->second;
        }

        inline value_type& current() const noexcept {
            return *current_program_;
        }

        inline void remove(key_type&& unique_identifier) noexcept {
            auto it = shader_map_.find(unique_identifier);
            assert(it != shader_map_.end());
            shader_map_.erase(it);
        }

        inline void bind(value_type& shader) noexcept {
            current_program_ = &shader;
            glUseProgram(current_program_->get());
        }

        inline value_type& get_default_program() noexcept {
            return default_program_;
        }
    private:
        value_type default_program_;
        value_type* current_program_;
        std::unordered_map<key_type, gsl::unique_ptr<value_type>> shader_map_;
    };
}