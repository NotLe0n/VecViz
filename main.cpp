#include "UI/UIWindow.h"
#include "UI/VectorsWindow.h"
#include "UI/TransformationWindow.h"
#include "UI/VectorSpaceWindow.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "utils.h"
#include "rlImGui.h"
#include "VectorSpaces/VectorSpace2D.h"

int main()
{
    // create window
    UIWindow& wnd = UIWindow::GetWindow();

    std::unique_ptr<VectorSpace> currentVs = std::make_unique<VectorSpace2D>(VectorSpace2D());
    /*currentVs->AddVector(DrawVector{2, 4});
    currentVs->AddVector(DrawVector{sqrtf(2), 3});
    currentVs->AddVector(DrawVector{-10, -7});*/

    wnd.Draw([&currentVs] {
        rlImGuiBegin();

        ImGuiID dock_id = DockFullScreen();

        DrawVectorsWindow(currentVs);
        DrawTransformationWindow(currentVs);
        DrawVectorSpaceWindow(currentVs);

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

            if (ImGui::BeginMenu("View"))
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
    });
    return 0;
}