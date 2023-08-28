#include "very_first.h"

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