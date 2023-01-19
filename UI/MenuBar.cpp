#include "SubWindows.h"
#include "imgui.h"
#include <memory>
#include "../VectorSpaces/VectorSpace2D.h"
#include "../Settings.h"

bool DrawMenuBar(std::unique_ptr<VectorSpace>& currentVs)
{
    Settings& settings = Settings::GetSettings();

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::BeginMenu("New vector space")) {
                if (ImGui::MenuItem("1D vector space")) {

                }
                if (ImGui::MenuItem("2D vector space")) {
                    currentVs = std::make_unique<VectorSpace2D>();
                }
                if (ImGui::MenuItem("3D vector space")) {

                }
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Open...")) {
                // TODO
            }
            if (ImGui::MenuItem("Save...")) {
                // TODO
            }

            if (ImGui::MenuItem("Close all vector spaces")) {
                currentVs.reset();
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

        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Show vectors window", nullptr, &settings.showVectorsWindow);
            ImGui::MenuItem("Show transformations window", nullptr, &settings.showTransformationsWindow);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options")) {
            ImGui::Checkbox("Draw grid", &settings.drawGrid);
            ImGui::Checkbox("Draw axis", &settings.drawAxis);
            ImGui::Checkbox("Draw vector arrows", &settings.drawVectorArrow);
            ImGui::Checkbox("Draw vector point", &settings.drawVectorPoint);
            ImGui::Checkbox("Draw vector name", &settings.drawVectorName);
            ImGui::Checkbox("Draw vector coordinates", &settings.drawVectorCoords);
            ImGui::Checkbox("Draw vector label", &settings.drawVectorLabel);
            ImGui::Checkbox("Draw basis vector(s)", &settings.drawBasisVectors);

            ImGui::Separator();

            ImGui::DragInt("Decimal precision", &settings.decimalPrecision, 1, 0, 6);

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    return true;
}
