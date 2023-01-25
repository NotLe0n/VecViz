#include <string>
#include "imgui.h"
#include "SubWindows.h"
#include "../Settings.h"

float transformMatrixVals[9] = {1, 0, 0,
                                0, 1, 0,
                                0, 0, 1};
bool sinusMode;
bool autoApplyTransformation;
bool updatedValues;

void DrawTransformationWindow(std::unique_ptr<VectorSpace>& currentVs)
{
    if (!currentVs) {
        return;
    }

    Settings& settings = Settings::GetSettings();

    if (!settings.showTransformationsWindow) {
        return;
    }

    if (ImGui::Begin("Transformations", &settings.showTransformationsWindow))
    {
        ImGui::Text("Transformation percentage:");
        ImGui::SetNextItemWidth(160);
        ImGui::SliderFloat("##tValue", &currentVs->t, 0, 1, settings.GetDecimalPrecisionStr().c_str());
        ImGui::SameLine();
        ImGui::Checkbox("Sinus mode", &sinusMode);

        if (sinusMode) {
            currentVs->t = (sinf(GetTime()) + 1) / 2;
        }

        ImGui::Text("Transformation matrix:");
        for (int i = 0; i < currentVs->GetDimension(); ++i) {
            std::string name1 = std::string("##transformMatrix:") + std::to_string(i);
            ImGui::SetNextItemWidth(80);
            if (ImGui::InputFloat(name1.c_str(), &transformMatrixVals[i], 0, 0, settings.GetDecimalPrecisionStr().c_str())) {
                updatedValues = true;
            }

            for (int j = 1; j < currentVs->GetDimension(); ++j) {
                std::string name2 = std::string("##transformMatrix:") + std::to_string(i + j * 3);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(80);
                if (ImGui::InputFloat(name2.c_str(), &transformMatrixVals[i + j * 3], 0, 0, settings.GetDecimalPrecisionStr().c_str())) {
                    updatedValues = true;
                }
            }
        }

        if (!updatedValues) {
            transformMatrixVals[0] = currentVs->GetBasisX().X();
            transformMatrixVals[1] = currentVs->GetBasisY().X();
            transformMatrixVals[2] = currentVs->GetBasisZ().X();

            transformMatrixVals[3] = currentVs->GetBasisX().Y();
            transformMatrixVals[4] = currentVs->GetBasisY().Y();
            transformMatrixVals[5] = currentVs->GetBasisZ().Y();

            transformMatrixVals[6] = currentVs->GetBasisX().Z();
            transformMatrixVals[7] = currentVs->GetBasisY().Z();
            transformMatrixVals[8] = currentVs->GetBasisZ().Z();

            updatedValues = false;
        }

        if (ImGui::Button("Apply Transformation")) {
            currentVs->ApplyTransformation({
                   transformMatrixVals[0], transformMatrixVals[3], transformMatrixVals[6], 0,
                   transformMatrixVals[1], transformMatrixVals[4], transformMatrixVals[7], 0,
                   transformMatrixVals[2], transformMatrixVals[5], transformMatrixVals[8], 0,
                   0, 0, 0, 1
            });
        }

        ImGui::SameLine();
        ImGui::Checkbox("Auto apply", &autoApplyTransformation);
        if (autoApplyTransformation) {
            currentVs->ApplyTransformation({
                   transformMatrixVals[0], transformMatrixVals[3], transformMatrixVals[6], 0,
                   transformMatrixVals[1], transformMatrixVals[4], transformMatrixVals[7], 0,
                   transformMatrixVals[2], transformMatrixVals[5], transformMatrixVals[8], 0,
                   0, 0, 0, 1
           });
        }

        ImGui::BeginDisabled(!settings.drawGrid);
        ImGui::Checkbox("Draw transformed grid", &settings.drawTransformedGrid);
        ImGui::EndDisabled();

        ImGui::SameLine();

        ImGui::BeginDisabled(!(settings.drawVectorArrow || settings.drawVectorPoint || settings.drawVectorName || (settings.drawVectorLabel && (settings.drawVectorCoords || settings.drawCoordinateVectorPos))));
        ImGui::Checkbox("Draw basis vectors", &settings.drawBasisVectors);
        ImGui::EndDisabled();
        ImGui::End();
    }
}
