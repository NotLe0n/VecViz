#include "UI/UIWindow.h"
#include "VectorSpaces/VectorSpace.h"
#include "VectorSpaces/VectorSpace2D.h"
#include "rlImGui.h"
#include "imgui.h"
#include "raymath.h"
#include "FontManager.h"
#include "imgui_internal.h"
#include <memory>

void UpdateDrawFrame();
void DrawSideBar();

std::unique_ptr<VectorSpace> currentVs; // temporary TODO: put in main and pass reference

int main()
{
    // create window
    UIWindow& wnd = UIWindow::GetWindow();

    // Temporary until VS selection is implemented
    currentVs = std::make_unique<VectorSpace2D>(VectorSpace2D());
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
        DrawSideBar();
        ImGui::End();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    if (ImGui::Begin("Vectorspace view"), 0, ImGuiWindowFlags_NoDocking) {
        Texture *renderTexture = &currentVs->GetRenderTexture()->texture;
        float rtw = ImGui::GetWindowWidth();
        float rth = ImGui::GetWindowHeight() - ImGui::GetFrameHeight();
		rlImGuiImageRect(renderTexture, rtw, rth, {0, 0, rtw, rth});

        VectorSpace::drawOffset = {ImGui::GetWindowPos().x, ImGui::GetWindowPos().y};
        if (ImGui::IsWindowHovered()) {
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
        ImGui::DockBuilderFinish(dock_id);
    }

    ImGui::ShowDemoWindow();

    rlImGuiEnd();
}

float transformMatrixVals[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, };
bool sinusMode;
void DrawSideBar() {
    ImGui::InputFloat2("Vector", addVectorVals);
    if (ImGui::Button("Add Vector")) {
        currentVs->AddVector(DrawVector{addVectorVals[0], addVectorVals[1]});
    }

    ImGui::Separator();

    ImGui::Text("Vectors: ");
    if (ImGui::BeginListBox("vectors list:", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing()))) {
        for (int n = 0; n < currentVs->vectors.size(); n++) {
            ImGui::Text("v%d:", n);
            ImGui::SameLine();

            ImGui::SetNextItemWidth(200);
            std::string xlabel = std::string("##VectorInputX") + std::to_string(n);
            ImGui::InputFloat(xlabel.c_str(), &currentVs->vectors[n].vector.x, 1);

            ImGui::SameLine();
            ImGui::SetNextItemWidth(200);
            std::string ylabel = std::string("##VectorInputY") + std::to_string(n);
            ImGui::InputFloat(ylabel.c_str(), &currentVs->vectors[n].vector.y, 1);
        }
        ImGui::EndListBox();
    }
    ImGui::Separator();
    ImGui::Text("Transformation percentage:");
    ImGui::SliderFloat("##tValue", &currentVs->t, 0, 1);
    ImGui::SameLine();
    ImGui::Checkbox("Sinus mode", &sinusMode);

    if (sinusMode) {
        currentVs->t = (sinf(GetTime()) +1) / 2;
    }

    ImGui::Text("Transformation matrix:");
    for (int i = 0; i < currentVs->GetDimension(); ++i) {
        ImGui::SetNextItemWidth(80);
        ImGui::InputFloat((std::string("##transformMatrix:") + std::to_string(i)).c_str(), &transformMatrixVals[i]);
        for (int j = 1; j < currentVs->GetDimension(); ++j) {
            ImGui::SameLine();
            ImGui::SetNextItemWidth(80);
            ImGui::InputFloat((std::string("##transformMatrix:") + std::to_string(i+j*3)).c_str(), &transformMatrixVals[i+j*3]);
        }
    }

    if (ImGui::Button("Apply Transformation")) {
        currentVs->ApplyTransformation({
            transformMatrixVals[0], transformMatrixVals[3], transformMatrixVals[6], 0,
            transformMatrixVals[1], transformMatrixVals[4], transformMatrixVals[7], 0,
            transformMatrixVals[2], transformMatrixVals[5], transformMatrixVals[8], 0,
            0, 0, 0, 1
        });
    }
}