/**
 * Main app entry point for macOS using Metal.
 * This file contains the main function and the application delegate.
 * */

#include <cassert>

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "../game/game.h"
#include "../game/game_breakout.h"
#include "../game/village_game.h"
#include "../input/input_manager.h"
#include "../misc/global_vars.h"
#include "../net/connection.h"
#include "../rendering/metal/renderer_metal.h"
#include "src/file_io/ini/ini_parser.h"
#include "src/logging.h"
#include "src/misc/utils.h"
#include "src/time/time_manager.h"

class GameMain : public MTK::ViewDelegate {
   public:
    GameMain(MTL::Device* pDevice, MTK::View* pView);
    virtual ~GameMain() override;

    bool InitGameSystems(MTL::Device* device, MTK::View* view);

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
    ME::GameBreakout game;
    ME::RendererMetal renderer;

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
    CGRect frame = (CGRect){{100.0, 100.0}, {1024.0, 768.0}};

    window = NS::Window::alloc()->init(frame, NS::WindowStyleMaskClosable | NS::WindowStyleMaskTitled,
                                       NS::BackingStoreBuffered, false);
    window->setTitle(NS::String::string("Village Game", NS::StringEncoding::UTF8StringEncoding));

    // Create Metal Device.
    device = MTL::CreateSystemDefaultDevice();

    // Create Metal View.
    mtkView = MTK::View::alloc()->init(frame, device);
    mtkView->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
    mtkView->setClearColor(MTL::ClearColor::Make(0.01, 0.01, 0.01, 1.0));
    mtkView->setDepthStencilPixelFormat(MTL::PixelFormat::PixelFormatDepth32Float);
    mtkView->setClearDepth(1.0);

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

GameMain::GameMain(MTL::Device* device, MTK::View* view) : MTK::ViewDelegate(), mtkView(view), device(device) {
    InitGameSystems(device, view);
}

bool GameMain::InitGameSystems(MTL::Device* device, MTK::View* view) {
    // Read game params from file.
    INIMap iniMap = Load();
    fps = std::atoi(iniMap["settings"]["fps"].c_str());
    maxRunTime = std::atoi(iniMap["settings"]["maxRunTime"].c_str());

    // Init global variables.
    timeManager.Init(fps, false);
    bool shouldTick = false;
    double deltaTime = 0.0f;

    inputManager.Init();
    connection.Init();

    game.SetInputManagerRef(&inputManager);
    game.SetConnectionRef(&connection);
    game.Init(&timeManager);

    renderer.InitMTL(device, view);
    renderer.SetScene(game.GetScene());

    return true;
}

GameMain::~GameMain() {
    ShutDownGameSystems();
}

void GameMain::drawInMTKView(MTK::View* view) {
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

    bool shouldTick = timeManager.Update();
    double deltaTime = 0.0f;

    if (shouldTick) {
        deltaTime = timeManager.GetDeltaTime();

        inputManager.Update(deltaTime);

        game.Update(deltaTime);

        renderer.Update();
        renderer.Draw(view);

        connection.Update(deltaTime);
    }

    // Perform Rendering
    // The Renderer::DrawFrame method needs to be adapted to use the
    // pView's currentRenderPassDescriptor and currentDrawable.
    // _renderer.DrawFrame(pView);  // Example: Pass the view to the renderer

    // Check for exit condition (e.g., max run time)
    if (maxRunTime > 0 && timeManager.GetTimeSinceStartup() > maxRunTime) {
        ShutDownGameSystems();
        // Close the application
        NS::Application::sharedApplication()->windows()->object<NS::Window>(0)->close();
    }

    pPool->release();
}

void GameMain::ShutDownGameSystems() {
    game.End();
    connection.End();
    inputManager.End();
    timeManager.End();
    renderer.End();
}
