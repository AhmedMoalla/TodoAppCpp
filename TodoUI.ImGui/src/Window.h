#pragma once

#include <print>
#include <GLFW/glfw3.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <functional>
#include <imgui.h>

namespace todo_imgui {
    struct WindowDimensions {
        int width;
        int height;
    };

    struct WindowSpecification {
        WindowDimensions dimensions;
        const char* title;
        const bool resizable;
    };

    typedef std::function<void()> KeyCallback;

    struct KeyCallbackMapping {
        int key;
        int mods;
        int action;
        KeyCallback callback;

        // Fix for Clang 15 : does not support Parenthesized initialization of aggregates (P0960R3) => (Implemented in Clang 16)
        // https://compiler-explorer.com/z/KTPr48PMG
        KeyCallbackMapping(const int key, const int mods, const int action, KeyCallback callback)
            : key(key),
              mods(mods),
              action(action),
              callback(std::move(callback)) {
        }

        void operator()() const {
            callback();
        }
    };

    class Window {
    public:
        ImVec4 clear_color = ImVec4(0, 0, 0, 1);

        explicit Window(const WindowSpecification &spec) {
            glfwSetErrorCallback([](int error_code, const char *description) {
                std::println(stderr, "GLFW Error {}: {}", error_code, description);
            });

            if (!glfwInit())
                throw std::runtime_error("Failed to initialize GLFW");

            const auto glsl_version = configure_window(spec.resizable);

            handle = glfwCreateWindow(spec.dimensions.width, spec.dimensions.height, spec.title, nullptr, nullptr);
            if (handle == nullptr)
                throw std::runtime_error("Failed to create GLFW window");

            // Hack to pass lambda to glfwSetKeyCallback without capture group
            glfwSetWindowUserPointer(handle, this);
            glfwSetKeyCallback(handle, [](GLFWwindow* window, const int key, [[maybe_unused]] const int scancode, const int action, const int mods) {
                const auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
                self->key_callback(key, action, mods);
            });

            glfwSetFramebufferSizeCallback(handle, []([[maybe_unused]] GLFWwindow* window, const int width, const int height) {
                glViewport(0, 0, width, height);
            });

            glfwMakeContextCurrent(handle);
            glfwSwapInterval(1); // Enable vsync

            set_window_icon();

            configure_imgui(glsl_version);
        }

        ~Window();

        void close() const;
        [[nodiscard]] bool should_close() const;
        [[nodiscard]] bool poll_events() const;
        void begin_imgui_frame() const;
        void end_imgui_frame() const;
        [[nodiscard]] float get_frame_rate() const;
        void add_key_callback(int key, int mods, int action, const KeyCallback& callback);
        void add_key_callback(int key, int action, const KeyCallback& callback);
        [[nodiscard]] WindowDimensions get_dimensions() const {
            WindowDimensions dimensions{};
            glfwGetFramebufferSize(handle, &dimensions.width, &dimensions.height);
            return dimensions;
        }

    private:
        GLFWwindow* handle;
        std::vector<KeyCallbackMapping> callbacks;

        const char* configure_window(bool resizable);
        void configure_imgui(const char *glsl_version) const;
        void set_window_icon() const;
        void key_callback(int key, int action, int mods) const;
    };
}