#pragma once

namespace  miwa {
    class Window;
}

namespace miwa {
    class IRenderable {
    public:
        virtual void draw() = 0;
    };
}