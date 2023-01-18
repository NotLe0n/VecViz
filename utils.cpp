#include "utils.h"
#include "raymath.h"

std::vector<std::string> split(std::string s, const std::string &del)
{
    size_t pos = 0;
    std::string token;
    std::vector<std::string> result;
    while ((pos = s.find(del)) != std::string::npos) {
        token = s.substr(0, pos);
        result.push_back(token);
        s.erase(0, pos + del.length());
    }
    result.push_back(token);

    return std::move(result);
}

Matrix MatrixLerp(Matrix m, float t)
{
    return {
            Lerp(1, m.m0, t), Lerp(0, m.m4, t), Lerp(0, m.m8, t), Lerp(0, m.m12, t),
            Lerp(0, m.m1, t), Lerp(1, m.m5, t), Lerp(0, m.m9, t), Lerp(0, m.m13, t),
            Lerp(0, m.m2, t), Lerp(0, m.m6, t), Lerp(1, m.m10, t), Lerp(0, m.m14, t),
            Lerp(0, m.m3, t), Lerp(0, m.m7, t), Lerp(0, m.m11, t), Lerp(1, m.m15, t),
    };
}

ImGuiID DockFullScreen()
{
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    // Submit the DockSpace
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    return dockspace_id;
}

void TextCentered(const std::string &text)
{
    ImVec2 windowSize = ImGui::GetWindowSize();
    float textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPos(ImVec2((windowSize.x - textWidth) * 0.5f, windowSize.y * 0.5f));
    ImGui::Text("%s", text.c_str());
}