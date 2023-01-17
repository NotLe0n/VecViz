#include "VectorSpaceWindow.h"
#include "imgui.h"
#include "rlImGui.h"

void DrawVectorSpaceWindow(std::unique_ptr<VectorSpace>& currentVs) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    if (ImGui::Begin("Vectorspace view"), 0, ImGuiWindowFlags_NoDocking) {
        Texture *renderTexture = &currentVs->GetRenderTexture()->texture;
        VectorSpace::drawOffset = {ImGui::GetWindowPos().x, ImGui::GetWindowPos().y};

        float rtw = ImGui::GetWindowWidth();
        float rth = ImGui::GetWindowHeight() - ImGui::GetFrameHeight();
        rlImGuiImageRect(renderTexture, rtw, rth, {0, ImGui::GetWindowPos().y, rtw, rth});

        if (ImGui::IsWindowHovered()) {
            currentVs->Update();
        }
        currentVs->Draw();
        ImGui::End();
    }
    ImGui::PopStyleVar(2);
}