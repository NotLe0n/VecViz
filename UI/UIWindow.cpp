#include "UIWindow.h"
#include "rlImGui.h"
#include "../FontManager.h"

namespace UIWindow
{
    static void StyleImGui();

    bool Init()
    {
        SetWindowState(FLAG_MSAA_4X_HINT);
        InitWindow(800, 600, "Vector Visualization");
        SetWindowState(FLAG_WINDOW_RESIZABLE);
        MaximizeWindow();

        if (!IsWindowReady()) {
            return false;
        }

        SetTargetFPS(120);

        rlImGuiSetup(true);

        FontManager::LoadFonts();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        StyleImGui();

        return true;
    }

    static void StyleImGui()
    {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
        colors[ImGuiCol_Border] = ImVec4(0.59f, 0.59f, 0.59f, 0.29f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.07f, 0.08f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.05f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_Button] = ImVec4(0.18f, 0.18f, 0.18f, 0.54f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.35f, 0.54f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.28f, 0.30f, 0.86f);
        colors[ImGuiCol_Header] = ImVec4(0.21f, 0.21f, 0.23f, 0.64f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.21f, 0.21f, 0.22f, 0.36f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.40f, 0.42f, 0.33f);
        colors[ImGuiCol_Separator] = ImVec4(0.76f, 0.74f, 0.74f, 0.38f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.59f, 0.66f, 0.66f, 0.65f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.62f, 0.67f, 0.70f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.05f, 0.18f, 0.24f, 0.52f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.27f, 0.44f, 0.63f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.37f, 0.46f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.25f, 0.31f, 0.38f, 0.43f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.25f, 0.27f, 0.29f, 1.00f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.43f, 0.73f, 0.76f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.15f, 0.36f, 0.69f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.43f, 0.73f, 0.76f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.15f, 0.36f, 0.69f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.35f);

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 5;
        style.ChildRounding = 5;
        style.FrameRounding = 5;
        style.GrabRounding = 10;
        style.TabRounding = 10;
        style.AntiAliasedFill = true;
        style.AntiAliasedLines = true;
        style.AntiAliasedLinesUseTex = false;
        style.FrameBorderSize = 1;
        style.WindowTitleAlign = ImVec2(.5, .5);
        style.WindowMenuButtonPosition = ImGuiDir_Right;
    }

    void Draw(const std::function<bool()>& drawFunc)
    {
        while (!WindowShouldClose()) {
            BeginDrawing();
            {
                rlImGuiBegin();
                {
                    if (!drawFunc()) {
                        break;
                    }
                }
                rlImGuiEnd();
            }
            EndDrawing();
        }
    }

    void CloseCurrentWindow()
    {
        rlImGuiShutdown();
        FontManager::UnloadFonts();
        CloseWindow();
    }
}
