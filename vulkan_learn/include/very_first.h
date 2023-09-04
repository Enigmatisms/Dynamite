/**
 * @file hello_vuld.cc
 * @author Qianyue He
 * @brief Hello vul(kan worl)d!
 * @date 2023-08-28
 */
#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

const std::vector<const char*> validation_layers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
    constexpr bool enable_validation_layers = false;
#else
    constexpr bool enable_validation_layers = true;
#endif

class HelloTriangleApplication {
public:
    /// how the application gets called
    void run() {
        init_window();
        init_vulkan();
        main_loop();
        clean_up();
    }

private:
    void init_window();

    /// initializing
    void init_vulkan();

    /// main logic loop
    void main_loop();

    /// can be thought of as a customized deconstructor 
    void clean_up();
    
    void show_supported_exts() const;

    /// customized instance creation
    void create_instance();

    void setup_debug_messenger();

    std::vector<const char*> HelloTriangleApplication::get_required_extensions() const;

    bool check_validation_layer_support(bool verbose = false) const;
private:
    GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
};