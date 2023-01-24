#include <string>
#include "imgui.h"
#include "SubWindows.h"
#include "../Settings.h"

float transformMatrixVals[9] = {1, 0, 0,
                                0, 1, 0,
                                0, 0, 1};
bool sinusMode;
bool autoApplyTransformation;

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

        if (currentVs->GetBasisX().X() != transformMatrixVals[0]
        || currentVs->GetBasisX().Y() != transformMatrixVals[3]
        || currentVs->GetBasisX().Z() != transformMatrixVals[7]) {
            transformMatrixVals[0] = currentVs->GetBasisX().X();
            transformMatrixVals[3] = currentVs->GetBasisX().Y();
            transformMatrixVals[6] = currentVs->GetBasisX().Z();
        }

        if (currentVs->GetBasisY().X() != transformMatrixVals[0]
            || currentVs->GetBasisY().Y() != transformMatrixVals[3]
            || currentVs->GetBasisY().Z() != transformMatrixVals[7]) {
            transformMatrixVals[1] = currentVs->GetBasisY().X();
            transformMatrixVals[4] = currentVs->GetBasisY().Y();
            transformMatrixVals[7] = currentVs->GetBasisY().Z();
        }

        if (currentVs->GetBasisY().X() != transformMatrixVals[0]
            || currentVs->GetBasisY().Y() != transformMatrixVals[3]
            || currentVs->GetBasisY().Z() != transformMatrixVals[7]) {
            transformMatrixVals[2] = currentVs->GetBasisZ().X();
            transformMatrixVals[5] = currentVs->GetBasisZ().Y();
            transformMatrixVals[8] = currentVs->GetBasisZ().Z();
        }

        ImGui::Text("Transformation matrix:");
        for (int i = 0; i < currentVs->GetDimension(); ++i) {
            ImGui::SetNextItemWidth(80);
            ImGui::InputFloat((std::string("##transformMatrix:") + std::to_string(i)).c_str(),
                              &transformMatrixVals[i], 0, 0, settings.GetDecimalPrecisionStr().c_str());
            for (int j = 1; j < currentVs->GetDimension(); ++j) {
                ImGui::SameLine();
                ImGui::SetNextItemWidth(80);
                ImGui::InputFloat((std::string("##transformMatrix:") + std::to_string(i + j * 3)).c_str(),
                                  &transformMatrixVals[i + j * 3], 0, 0, settings.GetDecimalPrecisionStr().c_str());
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
        ImGui::End();
    }
}
