/**
 * @file hello_vuld.cc
 * @author Qianyue He
 * @brief Hello vul(kan worl)d!
 * @date 2023-08-28
 */
#define GLFW_INCLUDE_VULKAN
#include <iomanip>
#include <ctime>
#include "very_first.h"

/**
 * So... where shall I pass this function to?
 * @param messageSeverity: Log level. We can implement the logic in our code, 
 *      when the severity is greater than specified then we output
 * @param messageType: This is not so clear as messageSeverity
 * @param pCallbackData: have debug info
 * @param pUserData: should be demand-specific
 */
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
) {
    // Maybe we can pass in the function name as pUserData
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::cerr << "[Time = " << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "] validation layer: " << pCallbackData->pMessage <<
    std::endl;
    return VK_FALSE;
}

void destroy_debug_utils_EXT(VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger, const
    VkAllocationCallbacks* pAllocator
) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
    vkGetInstanceProcAddr(instance,
    "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

VkResult create_debug_utils_messenger_EXT(VkInstance instance, const
    VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const
    VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT*
    pDebugMessenger
) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void HelloTriangleApplication::init_window() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // You can actually play around with this setting, make it resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan-Hello", nullptr, nullptr);
}

void HelloTriangleApplication::clean_up() {
    if (enable_validation_layers) {
        destroy_debug_utils_EXT(instance, debug_messenger, nullptr);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    vkDestroyInstance(instance, nullptr);
}

void HelloTriangleApplication::init_vulkan() {
    create_instance();
    setup_debug_messenger();
}

void HelloTriangleApplication::main_loop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

std::vector<const char*> HelloTriangleApplication::get_required_extensions() const {
    /// return vulkan extensions used for the instances, for example: GLFW / vulkan debug extensions
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enable_validation_layers) {
        // We might not need this if we skip rigorous debug
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
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

void HelloTriangleApplication::setup_debug_messenger() {
    if (!enable_validation_layers) return;
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType =
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = 
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT     |   
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT  |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debug_callback;
    createInfo.pUserData = this; // Optional
    if (create_debug_utils_messenger_EXT(instance, &createInfo, nullptr, &debug_messenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
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
    auto extensions = get_required_extensions();

    if constexpr(enable_validation_layers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        createInfo.ppEnabledLayerNames = validation_layers.data();
    } else{
        createInfo.enabledLayerCount = 0;
    }

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance!");
    }
    printf("Vulkan instance created.\n");
    show_supported_exts();
}
