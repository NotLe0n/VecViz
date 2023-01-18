#ifndef VECVIZ_UIWINDOW_H
#define VECVIZ_UIWINDOW_H

#include <functional>
#include <string>
#include "raylib.h"

// Singleton wraps the creation of Raylib window instance and draw loop
class UIWindow {

private:
    UIWindow(const std::string& title, const Color& backgroundColor, const ConfigFlags& flags);

private:
    Color backgroundColor;

public:
    UIWindow(UIWindow&&) = delete;
    UIWindow(UIWindow&) = delete;

    void operator=(const UIWindow&) = delete;
    void operator=(const UIWindow&&) = delete;

public:
    static UIWindow& GetWindow();

    Color GetBackgroundColor();

    // Begins draw loop
    void Draw(const std::function<bool()>&);

    // NOTE: Make sure to close all ImGui windows beforehand
    static void CloseCurrentWindow();
};

#endif //VECVIZ_UIWINDOW_H
