#include "UI/UIWindow.h"
#include "UI/SubWindows.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "utils.h"
#include "VectorSpaces/VectorSpace2D.h"

int main()
{
    // create window
    UIWindow& wnd = UIWindow::GetWindow();

    std::unique_ptr<VectorSpace> currentVs = std::make_unique<VectorSpace2D>();
    /*currentVs->AddVector(DrawVector{2, 4});
    currentVs->AddVector(DrawVector{sqrtf(2), 3});
    currentVs->AddVector(DrawVector{-10, -7});*/

    wnd.Draw([&currentVs] {
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
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::BeginMenu("New Vector Space...")) {
                    if (ImGui::MenuItem("1D Vector Space")) {

                    }
                    if (ImGui::MenuItem("2D Vector Space")) {

                    }
                    if (ImGui::MenuItem("3D Vector Space")) {

                    }
                    ImGui::EndMenu();
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Exit")) {
                    ImGui::EndMenu();
                    ImGui::EndMenuBar();
                    ImGui::End();
                    return false;
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Test");

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Options"))
            {
                ImGui::MenuItem("Test");

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();

        return true;
    });

    currentVs.reset();
    UIWindow::CloseCurrentWindow();
    return 0;
}