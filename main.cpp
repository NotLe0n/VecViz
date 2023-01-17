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
ImGuiID DockFullScreen() {
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    // Submit the DockSpace
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    return dockspace_id;
}

void UpdateDrawFrame()
{
    rlImGuiBegin();

    ImGuiID dock_id = DockFullScreen();

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

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("New"))
        {
            ImGui::MenuItem("Test", nullptr);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options"))
        {
            ImGui::MenuItem("Test", nullptr);

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();

    rlImGuiEnd();
}

float transformMatrixVals[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, };
bool sinusMode;
void DrawSideBar() {
    ImGui::Begin("Vectors");
    {
        float numberInputWidth = ImGui::CalcTextSize("8.888").x;

        ImGui::SetNextItemWidth(numberInputWidth + 100);
        ImGui::InputFloat2("##AddVectorInput", addVectorVals);
        ImGui::SameLine();

        if (ImGui::Button("Add Vector")) {
            currentVs->AddVector(DrawVector{addVectorVals[0], addVectorVals[1]});
        }

        if (ImGui::BeginListBox("##VectorList", ImVec2(400, 10 * ImGui::GetTextLineHeightWithSpacing()))) {
            for (int n = 0; n < currentVs->vectors.size(); n++) {
                ImGui::Text("v%d:", n);
                ImGui::SameLine();

                std::string xlabel = std::string("##VectorInputX") + std::to_string(n);
                ImGui::SetNextItemWidth(numberInputWidth + 100);
                ImGui::InputFloat(xlabel.c_str(), &currentVs->vectors[n].vector.x, 1);

                ImGui::SameLine();

                std::string ylabel = std::string("##VectorInputY") + std::to_string(n);
                ImGui::SetNextItemWidth(numberInputWidth + 100);
                ImGui::InputFloat(ylabel.c_str(), &currentVs->vectors[n].vector.y, 1);
            }
            ImGui::EndListBox();
        }
    }
    ImGui::End();

    ImGui::Begin("Transformations");
    {
        ImGui::Text("Transformation percentage:");
        ImGui::SliderFloat("##tValue", &currentVs->t, 0, 1);
        ImGui::SameLine();
        ImGui::Checkbox("Sinus mode", &sinusMode);

        if (sinusMode) {
            currentVs->t = (sinf(GetTime()) + 1) / 2;
        }

        ImGui::Text("Transformation matrix:");
        for (int i = 0; i < currentVs->GetDimension(); ++i) {
            ImGui::SetNextItemWidth(80);
            ImGui::InputFloat((std::string("##transformMatrix:") + std::to_string(i)).c_str(), &transformMatrixVals[i]);
            for (int j = 1; j < currentVs->GetDimension(); ++j) {
                ImGui::SameLine();
                ImGui::SetNextItemWidth(80);
                ImGui::InputFloat((std::string("##transformMatrix:") + std::to_string(i + j * 3)).c_str(), &transformMatrixVals[i + j * 3]);
            }
        }

        if (ImGui::Button("Apply Transformation")) {
            currentVs->ApplyTransformation({
                transformMatrixVals[0], transformMatrixVals[3],
                transformMatrixVals[6], 0,
                transformMatrixVals[1], transformMatrixVals[4],
                transformMatrixVals[7], 0,
                transformMatrixVals[2], transformMatrixVals[5],
                transformMatrixVals[8], 0,
                0, 0, 0, 1
            });
        }
    }
    ImGui::End();
}