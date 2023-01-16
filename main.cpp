#include "UI/UIWindow.h"
#include "VectorSpaces/VectorSpace.h"
#include "VectorSpaces/VectorSpace2D.h"
#include "rlImGui.h"
#include "imgui.h"
#include "raymath.h"
#include "FontManager.h"
#include "imgui_internal.h"

void UpdateDrawFrame();

VectorSpace* currentVs = nullptr;

int main()
{
    // create window
    UIWindow& wnd = UIWindow::GetWindow();

    // Temporary until VS selection is implemented
    currentVs = new VectorSpace2D();
    currentVs->AddVector(DrawVector{2, 4});
    currentVs->AddVector(DrawVector{sqrtf(2), 3});
    currentVs->AddVector(DrawVector{-10, -7});

    wnd.Draw(UpdateDrawFrame);

    return 0;
}

float addVectorVals[] = {0, 0};
void UpdateDrawFrame()
{
    rlImGuiBegin();

    ImGuiID dock_id =ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    if (ImGui::Begin("sidebar")) {
        ImGui::InputFloat2("Vector", addVectorVals);
        if (ImGui::Button("Add Vector")) {
            currentVs->AddVector(DrawVector{addVectorVals[0], addVectorVals[1]});
        }

        ImGui::Text("Vectors: ");
        if (ImGui::BeginListBox("vectors list:", ImVec2(-FLT_MIN, currentVs->GetVectors().size() * ImGui::GetTextLineHeightWithSpacing()))) {
            for (int n = 0; n < currentVs->GetVectors().size(); n++) {
                ImGui::Text("v%d: [%f, %f]", n, currentVs->GetVectors()[n].vector.x, currentVs->GetVectors()[n].vector.y);
            }
            ImGui::EndListBox();
        }
        ImGui::End();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    if (ImGui::Begin("Vectorspace view")) {
        Texture *renderTexture = &currentVs->GetRenderTexture()->texture;
        rlImGuiImageRect(renderTexture, ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - ImGui::GetFrameHeight(), {0, 0, ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - ImGui::GetFrameHeight()});

        VectorSpace::drawOffset = GetScreenWidth() - ImGui::GetWindowWidth();
        if(ImGui::IsWindowHovered()) {
            currentVs->Update();
        }
        currentVs->Draw();
        ImGui::End();
    }
    ImGui::PopStyleVar(2);

    static bool first_time = true;
    if (first_time) {
        first_time = false;
        ImGui::DockBuilderDockWindow("Vectorspace view", dock_id);
        //ImGui::DockBuilderDockWindow("sidebar", dock_id);
        ImGui::DockBuilderFinish(dock_id);
    }

    ImGui::ShowDemoWindow();

    rlImGuiEnd();
}