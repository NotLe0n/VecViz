#include "UIWindow.h"
#include "../rlImGui.h"
#include "../FontManager.h"

UIWindow::UIWindow(const std::string& title, const Color& backgroundColor, const ConfigFlags& flags)
    : backgroundColor(backgroundColor)
{
    InitWindow(800, 600, title.c_str());
    SetWindowState(flags | FLAG_WINDOW_HIDDEN);
    MaximizeWindow();
    ClearWindowState(FLAG_WINDOW_HIDDEN);

    FontManager::LoadFonts();
}

UIWindow &UIWindow::GetWindow() {
    static UIWindow window("Vector Visualization", Color{20, 20, 20, 255}, FLAG_WINDOW_RESIZABLE);
    return window;
}

void UIWindow::Draw(const std::function<void()>& drawFunc) {
    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(drawFunc, 0, 1);
    #else
        SetTargetFPS(120);
    #endif

    rlImGuiSetup(true);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(backgroundColor);
        drawFunc();
        EndDrawing();
    }

    rlImGuiShutdown();

    CloseWindow();
}

Color UIWindow::GetBackgroundColor() {
    return backgroundColor;
}
