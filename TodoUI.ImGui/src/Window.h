#pragma once

#include <iostream>
#include <print>
#include <GLFW/glfw3.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <functional>
#include <imgui.h>

struct WindowSpecification {
    int width;
    int height;
    const char* title;
};

typedef std::function<void()> KeyCallback;

struct KeyCallbackMapping {
    int key;
    int mods;
    int action;
    KeyCallback callback;

    void operator()() const {
        callback();
    }
};

class Window {
public:
    ImVec4 clear_color = ImVec4(0, 0, 0, 1);

    explicit Window(const WindowSpecification& spec) {
        glfwSetErrorCallback([](int error_code, const char* description) {
            std::println(std::cerr, "GLFW Error {}: {}", error_code, description);
        });

        if (!glfwInit())
            throw std::runtime_error("Failed to initialize GLFW");

        glsl_version = configure_window();

        handle = glfwCreateWindow(spec.width, spec.height, spec.title, nullptr, nullptr);
        if (handle == nullptr)
            throw std::runtime_error("Failed to create GLFW window");

        // Hack
        glfwSetWindowUserPointer(handle, this);
        glfwSetKeyCallback(handle, [](GLFWwindow* window, const int key, const int _, const int action, const int mods) {
            const auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
            self->key_callback(key, action, mods);
        });

        glfwMakeContextCurrent(handle);
        glfwSwapInterval(1); // Enable vsync

        set_window_icon();

        configure_imgui();
    }

    ~Window();

    bool should_close() const;
    bool poll_events() const;
    void begin_imgui_frame() const;
    void end_imgui_frame() const;
    float get_frame_rate() const;
    void add_key_callback(int key, int mods, int action, const KeyCallback& callback);
    void add_key_callback(int key, int action, const KeyCallback& callback);

private:
    GLFWwindow* handle;
    const char* glsl_version;
    std::vector<KeyCallbackMapping> callbacks;

    static const char* configure_window();
    void configure_imgui() const;
    void set_window_icon() const;
    void key_callback(int key, int action, int mods) const;
};
