/**
 * @file hello_vuld.cc
 * @author Qianyue He
 * @brief Hello vul(kan worl)d!
 * @date 2023-08-28
 */
#define GLFW_INCLUDE_VULKAN
#include "very_first.h"

void HelloTriangleApplication::init_window() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // You can actually play around with this setting, make it resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan-Hello", nullptr, nullptr);
}

void HelloTriangleApplication::init_vulkan() {
    create_instance();
}

void HelloTriangleApplication::main_loop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

void HelloTriangleApplication::show_supported_exts() const {
    uint32_t ext_counts = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &ext_counts, nullptr);
    std::vector<VkExtensionProperties> ext_props(ext_counts);
    vkEnumerateInstanceExtensionProperties(nullptr, &ext_counts, ext_props.data());
    for (const auto& prop: ext_props) {
        std::cout << prop.extensionName << ". version: " << prop.specVersion << std::endl;
    }
}

bool HelloTriangleApplication::check_validation_layer_support(bool verbose) const {
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
    if (verbose) {
        for (const auto& prop: available_layers) {
            std::cout << prop.layerName << ". version: " << prop.specVersion << std::endl;
            std::cout << "\t" << prop.description << std::endl;
        }
    }
    for (const auto& layer_name: validation_layers) {
        bool found_support = false;
        for (const auto& prop: available_layers) {
            if (strcmp(prop.layerName, layer_name) == 0) {
                if (verbose) {
                    std::cout << prop.layerName << " is found within the listed support.\n";
                }
                found_support = true;
                break;
            }
        }
        if (!found_support) {
            return false;
        }
    }
    return true;
}

void HelloTriangleApplication::create_instance() {
    // Step1: create application info for vk-instance (what for? It doesn't seems so useful)
    if constexpr (enable_validation_layers) {
        if (!check_validation_layer_support()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }
    }
    VkApplicationInfo appInfo{};
    // type of the structure, a lot of enumeration variables
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    // ... this is so detailed
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";                          // Engine? What engine
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Step2: create instance creation info (for some device-specific information)
    /**
     * @ref vulkan-tutorial
     * Vulkan is a platform agnostic API, which means that you need an extension to interface with the window system
     */
    VkInstanceCreateInfo createInfo{};
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    if constexpr(enable_validation_layers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        createInfo.ppEnabledLayerNames = validation_layers.data();
    } else{
        createInfo.enabledLayerCount = 0;
    }
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance!");
    }
    printf("Vulkan instance created.\n");
    show_supported_exts();
}
