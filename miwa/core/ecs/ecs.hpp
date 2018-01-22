#pragma once

#include <iostream>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <typeindex>
#include <gsl/gsl>
#include <algorithm>
#include <vector>
#include <utils/utils.hpp>
#include <cassert>
#include <functional>

namespace miwa {

    namespace ecs {
        using entity_tag_t = size_t;

        class Component {
        public:
            Component() = default;

            virtual ~Component() = default;
        };

        class Entity {
        public:
            Entity() = default;
            
            ~Entity() = default;

            template<typename T>
            inline T& get() noexcept {
                auto it = components_.find(std::type_index(typeid(T)));
                assert(it != components_.end());
                return reinterpret_cast<T&>(*it->second);
            }

            template<typename T>
            inline bool has() noexcept {
                return components_.count(std::type_index(typeid(T))) > 0;
            }

            template<typename T, typename ... TArgs>
            inline T& add(TArgs&&... args) noexcept {
                T* t = new T(std::forward(args)...);
                auto c_ptr = gsl::unique_ptr<Component>(static_cast<Component*>(t));

                assert(components_.find(std::type_index(typeid(T))) == components_.end());
                
                components_.insert(components_.end(), { std::type_index(typeid(T)), std::move(c_ptr) });
                return *t;
            }

            template<typename T>
            inline void remove() noexcept {
                auto it = components_.find(std::type_index(typeid(T)));
                assert(it != components_.end());
                components_.erase(it);
            }
        private:
            std::unordered_map<std::type_index, gsl::unique_ptr<Component>> components_;
        };

        class System {
        public:
            System() = default;

            virtual ~System() = default;

            virtual void update() = 0;

           inline Entity& add_entity(entity_tag_t tag) noexcept {
                auto it = entities_.find(tag);
                auto* entity = new Entity();
                auto uPtr = gsl::unique_ptr<Entity>{ entity };
                if(it == entities_.end())
                    it = entities_.insert(entities_.end(), { tag, std::vector<gsl::unique_ptr<Entity>>() });
                it->second.push_back(std::move(uPtr));
                return *entity;
           }

            inline auto& get_entities(entity_tag_t tag) noexcept {
                auto it = entities_.find(tag);
                return it->second;
            }

            inline void remove_entity(entity_tag_t tag, Entity& entity) noexcept {
                auto it = entities_.find(tag);
                auto& vector = it->second;

                auto eit = std::find_if(vector.begin(), vector.end(), [&](auto& qentity) -> bool {
                    if(qentity.get() == &entity)
                        return true;
                    return false;
                });

                utils::quick_remove_at(vector, vector.begin() + std::distance(std::begin(vector), eit));
            }

            inline void remove_entities(entity_tag_t tag) noexcept {
                auto it = entities_.find(tag);
                it->second.clear();
            }

            inline const size_t get_entity_number(entity_tag_t tag) noexcept {
                auto it = entities_.find(tag);
                assert(it != entities_.end());
                return it->second.size();
            }

            inline void all(std::function<void(Entity&)> f) noexcept {
                std::for_each(entities_.begin(), entities_.end(), [&f](auto& entities) {
                    std::for_each(entities.second.begin(), entities.second.end(), [&f](auto& entity_ptr) {
                        f(*entity_ptr);
                    });
                });
            }

            inline void each(entity_tag_t entity_tag, std::function<void(Entity&)> f) noexcept {
                auto& entities = this->get_entities(entity_tag);
                std::for_each(entities.begin(), entities.end(), [&](auto& entity) {
                    f(*entity);
                });
            }

            inline void clear_cache() noexcept {
                auto it = entities_.begin();
                for(auto i = 0; i < entities_.size();) {
                    auto& val = it->second;
                    if(val.empty()) {
                        auto old_it = it;
                        it++;
                        entities_.erase(old_it);
                    } else {
                        it++;
                    }            
               }
            }
        private:
            std::unordered_map<entity_tag_t, std::vector<gsl::unique_ptr<Entity>>> entities_;
        };
    }
}