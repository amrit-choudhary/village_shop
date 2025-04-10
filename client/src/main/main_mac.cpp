/*
 *
 * Copyright 2022 Apple Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cassert>

#include "../game/game.h"
#include "../game/village_game.h"
#include "../input/input_manager.h"
#include "../misc/global_vars.h"
#include "../net/connection.h"
// #include "../rendering/renderer.h"
#include "src/file_io/ini/ini_parser.h"
#include "src/logging.h"
#include "src/misc/utils.h"
#include "src/time/time_manager.h"

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include <simd/simd.h>

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

class Renderer {
   public:
    Renderer(MTL::Device* device);
    ~Renderer();
    void buildShaders();
    void buildBuffers();
    void draw(MTK::View* view);

   private:
    MTL::Device* device;
    MTL::CommandQueue* commandQueue;
    MTL::RenderPipelineState* PSO;
    MTL::Buffer* vertexPositionsBuffer;
    MTL::Buffer* vertexColorsBuffer;
};

class GameMain : public MTK::ViewDelegate {
   public:
    GameMain(MTL::Device* pDevice, MTK::View* pView);
    virtual ~GameMain() override;

    bool InitGameSystems();

    // Game Loop Update
    virtual void drawInMTKView(MTK::View* pView) override;

    void ShutDownGameSystems();

    void HandleKeyDown(unsigned short keyCode);
    void HandleKeyUp(unsigned short keyCode);

   private:
    MTK::View* mtkView;   // Pointer to the view it manages
    MTL::Device* device;  // Pointer to the Metal device

    // Game Systems - Now owned by the view controller
    ME::Time::TimeManager timeManager;
    ME::Input::InputManager inputManager;  // Needs adaptation for NSEvent
    ME::Connection connection;
    ME::VillageGame game;
    // ME::Renderer _renderer;
    Renderer* renderer;  // Needs adaptation for MTKView

    int fps = 0;
    int maxRunTime = 0;
};

class GameAppDelegate : public NS::ApplicationDelegate {
   public:
    ~GameAppDelegate();

    NS::Menu* createMenuBar();

    virtual void applicationWillFinishLaunching(NS::Notification* pNotification) override;
    virtual void applicationDidFinishLaunching(NS::Notification* pNotification) override;
    virtual bool applicationShouldTerminateAfterLastWindowClosed(NS::Application* pSender) override;

   private:
    NS::Window* window;
    MTK::View* mtkView;
    MTL::Device* device;
    GameMain* viewDelegate = nullptr;
};

int main(int argc, char* argv[]) {
    NS::AutoreleasePool* pAutoreleasePool = NS::AutoreleasePool::alloc()->init();

    ME::SetPaths(argv[0], argv[1]);

    GameAppDelegate del;

    NS::Application* pSharedApplication = NS::Application::sharedApplication();
    pSharedApplication->setDelegate(&del);
    pSharedApplication->run();

    pAutoreleasePool->release();

    return 0;
}

GameAppDelegate::~GameAppDelegate() {
    mtkView->release();
    window->release();
    device->release();
    delete viewDelegate;
}

NS::Menu* GameAppDelegate::createMenuBar() {
    using NS::StringEncoding::UTF8StringEncoding;

    NS::Menu* pMainMenu = NS::Menu::alloc()->init();
    NS::MenuItem* pAppMenuItem = NS::MenuItem::alloc()->init();
    NS::Menu* pAppMenu = NS::Menu::alloc()->init(NS::String::string("Village Game", UTF8StringEncoding));

    NS::String* appName = NS::RunningApplication::currentApplication()->localizedName();
    NS::String* quitItemName = NS::String::string("Quit ", UTF8StringEncoding)->stringByAppendingString(appName);
    SEL quitCb = NS::MenuItem::registerActionCallback("appQuit", [](void*, SEL, const NS::Object* pSender) {
        auto pApp = NS::Application::sharedApplication();
        pApp->terminate(pSender);
    });

    NS::MenuItem* pAppQuitItem = pAppMenu->addItem(quitItemName, quitCb, NS::String::string("q", UTF8StringEncoding));
    pAppQuitItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);
    pAppMenuItem->setSubmenu(pAppMenu);

    NS::MenuItem* pWindowMenuItem = NS::MenuItem::alloc()->init();
    NS::Menu* pWindowMenu = NS::Menu::alloc()->init(NS::String::string("Window", UTF8StringEncoding));

    SEL closeWindowCb = NS::MenuItem::registerActionCallback("windowClose", [](void*, SEL, const NS::Object*) {
        auto pApp = NS::Application::sharedApplication();
        pApp->windows()->object<NS::Window>(0)->close();
    });
    NS::MenuItem* pCloseWindowItem = pWindowMenu->addItem(NS::String::string("Close Window", UTF8StringEncoding),
                                                          closeWindowCb, NS::String::string("w", UTF8StringEncoding));
    pCloseWindowItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);

    pWindowMenuItem->setSubmenu(pWindowMenu);

    pMainMenu->addItem(pAppMenuItem);
    pMainMenu->addItem(pWindowMenuItem);

    pAppMenuItem->release();
    pWindowMenuItem->release();
    pAppMenu->release();
    pWindowMenu->release();

    return pMainMenu->autorelease();
}

void GameAppDelegate::applicationWillFinishLaunching(NS::Notification* pNotification) {
    NS::Menu* pMenu = createMenuBar();
    NS::Application* pApp = reinterpret_cast<NS::Application*>(pNotification->object());
    pApp->setMainMenu(pMenu);
    pApp->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void GameAppDelegate::applicationDidFinishLaunching(NS::Notification* pNotification) {
    // Create window.
    CGRect frame = (CGRect){{100.0, 100.0}, {512.0, 512.0}};

    window = NS::Window::alloc()->init(frame, NS::WindowStyleMaskClosable | NS::WindowStyleMaskTitled,
                                       NS::BackingStoreBuffered, false);
    window->setTitle(NS::String::string("Village Game", NS::StringEncoding::UTF8StringEncoding));

    // Create Metal Device.
    device = MTL::CreateSystemDefaultDevice();

    // Create Metal View.
    mtkView = MTK::View::alloc()->init(frame, device);
    mtkView->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
    mtkView->setClearColor(MTL::ClearColor::Make(1.0, 0.0, 0.0, 1.0));

    // Create GameMain.
    viewDelegate = new GameMain(device, mtkView);

    mtkView->setDelegate(viewDelegate);
    // Configure MTKView for continuous drawing loop
    mtkView->setPaused(false);                 // Ensure the delegate methods are called
    mtkView->setEnableSetNeedsDisplay(false);  // Use internal timer for continuous redraw

    window->setContentView(mtkView);

    window->makeKeyAndOrderFront(nullptr);

    NS::Application* app = reinterpret_cast<NS::Application*>(pNotification->object());
    app->activateIgnoringOtherApps(true);
}

bool GameAppDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application* pSender) {
    return true;
}

GameMain::GameMain(MTL::Device* device, MTK::View* view)
    : MTK::ViewDelegate(), mtkView(view), device(device), renderer(new Renderer(device)) {
    InitGameSystems();
}

bool GameMain::InitGameSystems() {
    // Read game params from file.
    INIMap iniMap = Load();
    fps = std::atoi(iniMap["settings"]["fps"].c_str());
    maxRunTime = std::atoi(iniMap["settings"]["maxRunTime"].c_str());

    // Init global variables.
    timeManager.Init(fps);
    bool shouldTick = false;
    double deltaTime = 0.0f;

    inputManager.Init();
    connection.Init();

    game.SetInputManagerRef(&inputManager);
    game.SetConnectionRef(&connection);
    game.Init(&timeManager);

    // ME::Renderer renderer;
    // renderer.Init();

    return true;
}

GameMain::~GameMain() {
    ShutDownGameSystems();
    delete renderer;
}

void GameMain::drawInMTKView(MTK::View* view) {
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

    bool shouldTick = timeManager.Update();
    double deltaTime = 0.0f;

    if (shouldTick) {
        deltaTime = timeManager.GetDeltaTime();

        inputManager.Update(deltaTime);

        game.Update(deltaTime);

        // renderer.Update();
        renderer->draw(view);

        connection.Update(deltaTime);
    }

    // Perform Rendering
    // The Renderer::DrawFrame method needs to be adapted to use the
    // pView's currentRenderPassDescriptor and currentDrawable.
    // _renderer.DrawFrame(pView);  // Example: Pass the view to the renderer

    // Check for exit condition (e.g., max run time)
    if (maxRunTime > 0 && timeManager.GetTimeSinceStartup() > maxRunTime) {
        NS::Application::sharedApplication()->windows()->object<NS::Window>(0)->close();
    }

    pPool->release();
}

void GameMain::ShutDownGameSystems() {
    game.End();
    connection.End();
    inputManager.End();
    timeManager.End();
}

Renderer::Renderer(MTL::Device* device) : device(device->retain()) {
    commandQueue = device->newCommandQueue();
    buildShaders();
    buildBuffers();
}

Renderer::~Renderer() {
    vertexPositionsBuffer->release();
    vertexColorsBuffer->release();
    PSO->release();
    commandQueue->release();
    device->release();
}

void Renderer::buildShaders() {
    using NS::StringEncoding::UTF8StringEncoding;

    const char* shaderSrc = R"(
         #include <metal_stdlib>
         using namespace metal;
 
         struct v2f
         {
             float4 position [[position]];
             half3 color;
         };
 
         v2f vertex vertexMain( uint vertexId [[vertex_id]],
                                device const float3* positions [[buffer(0)]],
                                device const float3* colors [[buffer(1)]] )
         {
             v2f o;
             o.position = float4( positions[ vertexId ], 1.0 );
             o.color = half3 ( colors[ vertexId ] );
             return o;
         }
 
         half4 fragment fragmentMain( v2f in [[stage_in]] )
         {
             return half4( in.color, 1.0 );
         }
     )";

    NS::Error* error = nullptr;
    MTL::Library* library = device->newLibrary(NS::String::string(shaderSrc, UTF8StringEncoding), nullptr, &error);
    if (!library) {
        __builtin_printf("%s", error->localizedDescription()->utf8String());
        assert(false);
    }

    MTL::Function* vertexFn = library->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
    MTL::Function* fragFn = library->newFunction(NS::String::string("fragmentMain", UTF8StringEncoding));

    MTL::RenderPipelineDescriptor* desc = MTL::RenderPipelineDescriptor::alloc()->init();
    desc->setVertexFunction(vertexFn);
    desc->setFragmentFunction(fragFn);
    desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);

    PSO = device->newRenderPipelineState(desc, &error);
    if (!PSO) {
        __builtin_printf("%s", error->localizedDescription()->utf8String());
        assert(false);
    }

    vertexFn->release();
    fragFn->release();
    desc->release();
    library->release();
}

void Renderer::buildBuffers() {
    const size_t NumVertices = 3;

    simd::float3 positions[NumVertices] = {{-0.8f, 0.8f, 0.0f}, {0.0f, -0.8f, 0.0f}, {+0.8f, 0.8f, 0.0f}};

    static float deltaR = 0;
    static float deltaG = 0;
    static float deltaB = 0;

    deltaR += 0.01;
    deltaG += 0.02;
    deltaB += 0.03;

    if (deltaR > 1.0f) deltaR = 0;
    if (deltaG > 1.0f) deltaG = 0;
    if (deltaB > 1.0f) deltaB = 0;

    simd::float3 colors[NumVertices] = {{deltaR, 0.0f, 0.0f}, {0.0, deltaG, 0.0f}, {0.0f, 0.0f, deltaB}};

    const size_t positionsDataSize = NumVertices * sizeof(simd::float3);
    const size_t colorDataSize = NumVertices * sizeof(simd::float3);

    MTL::Buffer* tempVertexPositionsBuffer = device->newBuffer(positionsDataSize, MTL::ResourceStorageModeManaged);
    MTL::Buffer* tempVertexColorsBuffer = device->newBuffer(colorDataSize, MTL::ResourceStorageModeManaged);

    vertexPositionsBuffer = tempVertexPositionsBuffer;
    vertexColorsBuffer = tempVertexColorsBuffer;

    memcpy(vertexPositionsBuffer->contents(), positions, positionsDataSize);
    memcpy(vertexColorsBuffer->contents(), colors, colorDataSize);

    vertexPositionsBuffer->didModifyRange(NS::Range::Make(0, vertexPositionsBuffer->length()));
    vertexColorsBuffer->didModifyRange(NS::Range::Make(0, vertexColorsBuffer->length()));
}

void Renderer::draw(MTK::View* view) {
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    buildBuffers();

    MTL::CommandBuffer* cmd = commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* rpd = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* enc = cmd->renderCommandEncoder(rpd);

    enc->setRenderPipelineState(PSO);
    enc->setVertexBuffer(vertexPositionsBuffer, 0, 0);
    enc->setVertexBuffer(vertexColorsBuffer, 0, 1);
    enc->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));

    enc->endEncoding();
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();

    pool->release();
}
