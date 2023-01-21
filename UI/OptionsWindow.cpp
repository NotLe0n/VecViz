#include "SubWindows.h"
#include "../Settings.h"

void DrawOptionsWindow() {
    Settings& settings = Settings::GetSettings();
    if (!settings.showOptionsWindow) {
        return;
    }

    if (ImGui::Begin("Options", &settings.showOptionsWindow)) {
        if (ImGui::BeginTabBar("OptionsTabs")) {
            if (ImGui::BeginTabItem("General")) {
                ImGui::SetNextItemWidth(100);
                ImGui::SliderInt("Target FPS", &settings.targetFPS, 10, GetMonitorRefreshRate(GetCurrentMonitor()));
                ImGui::SameLine();
                if (ImGui::Button("Apply")) {
                    SetTargetFPS(settings.targetFPS);
                }

                ImGui::SetNextItemWidth(100);
                ImGui::DragInt("Decimal precision", &settings.decimalPrecision, .2f, 0, 6);

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Vector space options")) {
                ImGui::Checkbox("Draw untransformed grid", &settings.drawOrigGrid);
                ImGui::Checkbox("Draw transformed grid", &settings.drawTransformedGrid);
                ImGui::Checkbox("Draw grid", &settings.drawGrid);
                ImGui::Checkbox("Draw axis", &settings.drawAxis);
                ImGui::Checkbox("Draw vector arrows", &settings.drawVectorArrow);
                ImGui::Checkbox("Draw vector point", &settings.drawVectorPoint);
                ImGui::Checkbox("Draw vector name", &settings.drawVectorName);
                ImGui::Checkbox("Draw vector coordinates", &settings.drawVectorCoords);
                ImGui::Checkbox("Draw coordinate vector position", &settings.drawCoordinateVectorPos);
                ImGui::Checkbox("Draw vector label", &settings.drawVectorLabel);
                ImGui::Checkbox("Draw basis vector(s)", &settings.drawBasisVectors);
                
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}
