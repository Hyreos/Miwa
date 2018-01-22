#pragma once

#include <gsl/gsl>
#include <string_view>
#include <unordered_map>
#include "../core/ecs/world.hpp"

namespace miwa {
    class WorldSystem {
    public:
        WorldSystem() = default;

        WorldSystem(WorldSystem&) = delete;

        WorldSystem(WorldSystem&&) = default;
            
        template<typename T, typename ... TArgs>
        T& add(TArgs&&... args) noexcept {
            T* t = new T(std::forward(args)...);
             world_map_.insert(world_map_.end(), { 
                get_type_id<T>(), 
                gsl::unique_ptr<World>{ 
                    static_cast<World*>(t) 
                } 
            });
            return *t;
        }

        template<typename T>
        T& resolve() noexcept {
            auto it = world_map_.find( get_type_id<T>() );
            assert(it == world_map_.end());
            return reinterpret_cast<T&>(*it->second);
        }

        template<typename T>
        void WorldSystem::remove() noexcept {
            auto it = world_map_.find( get_type_id<T>());
            assert(it == world_map_.end());
            world_map_.erase(it);
        }

        template<typename T>
        void bind(T& target) noexcept {
            current_ = static_cast<World*>(&target);
        }

        inline World& current() noexcept {
            return *current_;
        }
    private:
        static size_t get_type_id() {
            static size_t id = 0;
            return id++;
        }

        template<typename T>
        static constexpr size_t get_type_id() {
            size_t id = get_type_id();
            return id;
        }

        World* current_;
        std::unordered_map<size_t, gsl::unique_ptr<World>> world_map_;
    };
}