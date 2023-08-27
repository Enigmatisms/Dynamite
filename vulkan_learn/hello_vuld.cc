/**
 * @file hello_vuld.cc
 * @author Qianyue He
 * @brief Hello vul(kan worl)d!
 * @date 2023-08-28
 */

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

class HelloTriangleApplication {
public:
    /// how the application gets called
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // You can actually play around with this setting, make it resizable
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan-Hello", nullptr, nullptr);
    }

    /// initializing
    void initVulkan() {

    }

    /// main logic loop
    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    /// can be thought of as a customized deconstructor 
    void cleanup() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
private:
    GLFWwindow* window;
};

int main() {
    HelloTriangleApplication app;

    try {
        printf("Vulkan APP tries to run.\n");
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}