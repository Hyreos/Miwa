#pragma once
#include<gsl/gsl>
#include "ecs.hpp"
#include <miwa/graphics/view.hpp>
#include<cmath>

namespace miwa {
    class World {
    public:
        World() {
            view_ = &default_view_;
        }

        virtual void update() = 0;

        template<typename T, typename...TArgs>
        T& add(TArgs&&... mArgs) {
            auto uPtr = std::make_unique<T>(std::forward(mArgs)...);
            auto* ptr = uPtr.get();
            systems.push_back(std::move(uPtr));
            return *ptr;
        }

        inline void all(std::function<void(ecs::System&)>&& f) {
            std::for_each(systems.begin(), systems.end(), [&](auto& system) {
                f(*system);
            });
        }

        inline void set_view(View& view) noexcept {
            view_ = &view;
        }

        inline void reset_view() noexcept {
            view_ = &default_view_;
        }

        inline View& get_view() {
            return *view_;
        }
    private:
        std::vector<gsl::unique_ptr<ecs::System>> systems;
        View default_view_;
        View* view_;
    };
}