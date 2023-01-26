#include "UI/UIWindow.h"
#include "UI/SubWindows.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "utils.h"

int main()
{
    // create window
    if (!UIWindow::Init()){
        return -1;
    }

    std::vector<std::unique_ptr<VectorSpace>> vectorSpaces = {};
    int currentVs = -1;
    //std::unique_ptr<VectorSpace> currentVs;

    UIWindow::Draw([&currentVs, &vectorSpaces] {
        ImGuiID dock_id = DockFullScreen();

        DrawVectorSpaceWindow(currentVs, vectorSpaces);
        if (!vectorSpaces.empty()) {
            DrawVectorsWindow(currentVs, vectorSpaces);
            DrawTransformationWindow(currentVs, vectorSpaces);
            DrawOptionsWindow();
        }

        static bool first_time = true;
        if (first_time) {
            first_time = false;
            ImGui::DockBuilderDockWindow("Vector space view", dock_id);
            ImGui::DockBuilderFinish(dock_id);
        }

        ImGui::ShowDemoWindow();

        if (!DrawMenuBar(currentVs, vectorSpaces)) {
            return false;
        }

        ImGui::End();

        return true;
    });

    vectorSpaces.clear();
    UIWindow::CloseCurrentWindow();
    return 0;
}
