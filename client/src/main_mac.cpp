#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include "AppKit/AppKit.hpp"
#include "Foundation/Foundation.hpp"
#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"

int main() {
    NS::Application* app = NS::Application::sharedApplication();
    MTL::Device* device = MTL::CreateSystemDefaultDevice();
    if (app && device) {
        printf("App and Metal device created successfully!\n");
        device->release();
    }
    return 0;
}