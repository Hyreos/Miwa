#include <miwa/miwa>
#include <iostream>
#include <thread>
#include <chrono>

using namespace miwa;
using namespace miwa::ecs;
using namespace std;

enum class EntityTags {
    Generic
};

class Textures : public Component {
public:
    Texture tex;
};

class GenericDraw : public System {
public:
    GenericDraw() {
        auto& entity = this->add_entity(static_cast<size_t>(EntityTags::Generic));
        auto& comp = entity.add<Textures>();
        comp.tex.load_from_file<ImageFormats::Png>("test2.png");
    }

    void update() override {
        this->all([](miwa::ecs::Entity& entity) {
            Sprite sprite(entity.get<Textures>().tex);
            sprite.draw();
        });
    }
};

class First : public ::World {
public:
    First() {
        add<GenericDraw>();
    }

    void update() {
        all([](auto& system) {
            system.update();
        });
    }
};

struct Application {
    std::unique_ptr<Window> window;

    struct {
        unique_ptr<ShaderSystem> shader_system;
        unique_ptr<WorldSystem> world_system;
        unique_ptr<AudioSystem> audio_system;
        unique_ptr<RenderSystem>render_system;
     } dependency;
        
    static inline Application& get() {
        static Application* self = new Application();
        return *self;
    }

    void init() noexcept {
        vec2 pos = { 0, 0 };
        vec2 size = { 800, 800 };
        
        window = make_unique<Window>(pos, size, "Miwa Test");

        dependency.render_system = make_unique<RenderSystem>(*window);
        dependency.shader_system = make_unique<ShaderSystem>();
        dependency.world_system  = make_unique<WorldSystem>();
        dependency.audio_system  = make_unique<AudioSystem>();

        Locator::get().add_render_system(*dependency.render_system);
        Locator::get().add_audio_system(*dependency.audio_system);
        Locator::get().add_world_system(*dependency.world_system);
        Locator::get().add_shader_system(*dependency.shader_system);
    
        Locator::get().get_world_system().bind(dependency.world_system->add<First>());
    }

    void run(uint32_t fps) {        
        AudioBuffer sndbuff("song.ogg");
            
        Emitter srcsnd;
        srcsnd.attach_audio_buffer(sndbuff);
        srcsnd.play();

        bool quit = false;

        while(!quit) {
            glClearColor(0.25f, 0.5f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            window->poll_events();
            
            auto& view = Locator::get().get_render_system().get_view();
            vec2 pos = { 0.f, 0.f };
            view.translate(pos);

            const auto& c_world = dependency.world_system->current();
            
            c_world.update();
            
            window->present();
            
            if(window->should_close())
                quit = true;
            
            this_thread::sleep_for(chrono::milliseconds(1000/fps));
        }
    } 
};

int main() {
    try {
        Application::get().init();
        Application::get().run(60u);
        return 0;
    } catch(std::runtime_error err) {
        std::cerr << "error: " << err.what() << std::endl;
        return 1;
    }
}