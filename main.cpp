#include "UI/UIWindow.h"
#include "UI/SubWindows.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "utils.h"

int main()
{
    // create window
    UIWindow::Init();

    std::unique_ptr<VectorSpace> currentVs;

    UIWindow::Draw([&currentVs] {
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

        if (!DrawMenuBar(currentVs)) {
            return false;
        }

        ImGui::End();

        return true;
    });

    currentVs.reset();
    UIWindow::CloseCurrentWindow();
    return 0;
}
