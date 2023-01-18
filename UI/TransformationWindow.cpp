#include <string>
#include "imgui.h"
#include "SubWindows.h"

float transformMatrixVals[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1,};
bool sinusMode;

void DrawTransformationWindow(std::unique_ptr<VectorSpace>& currentVs)
{
    if (!currentVs)
        return;

    ImGui::Begin("Transformations");
    {
        ImGui::Text("Transformation percentage:");
        ImGui::SetNextItemWidth(160);
        ImGui::SliderFloat("##tValue", &currentVs->t, 0, 1);
        ImGui::SameLine();
        ImGui::Checkbox("Sinus mode", &sinusMode);

        if (sinusMode) {
            currentVs->t = (sinf(GetTime()) + 1) / 2;
        }

        ImGui::Text("Transformation matrix:");
        for (int i = 0; i < currentVs->GetDimension(); ++i) {
            ImGui::SetNextItemWidth(80);
            ImGui::InputFloat((std::string("##transformMatrix:") + std::to_string(i)).c_str(),
                              &transformMatrixVals[i]);
            for (int j = 1; j < currentVs->GetDimension(); ++j) {
                ImGui::SameLine();
                ImGui::SetNextItemWidth(80);
                ImGui::InputFloat((std::string("##transformMatrix:") + std::to_string(i + j * 3)).c_str(),
                                  &transformMatrixVals[i + j * 3]);
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