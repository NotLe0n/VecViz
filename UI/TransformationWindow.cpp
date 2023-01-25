#include <string>
#include "imgui.h"
#include "SubWindows.h"
#include "../Settings.h"

bool updatedValues;

void DrawTransformationWindow(int currentVs, std::vector<std::shared_ptr<VectorSpace>> vectorSpaces)
{
    Settings& settings = Settings::GetSettings();

    if (!settings.showTransformationsWindow) {
        return;
    }

    if (ImGui::Begin("Transformations", &settings.showTransformationsWindow))
    {
        ImGui::Text("Transformation percentage:");
        ImGui::SetNextItemWidth(160);
        ImGui::SliderFloat("##tValue", &vectorSpaces[currentVs]->t, 0, 1, settings.GetDecimalPrecisionStr().c_str());
        ImGui::SameLine();
        ImGui::Checkbox("Sinus mode", &settings.sinusMode);

        if (settings.sinusMode) {
            vectorSpaces[currentVs]->t = (sinf(GetTime()) + 1) / 2;
        }

        ImGui::Text("Transformation matrix:");
        for (int i = 0; i < vectorSpaces[currentVs]->GetDimension(); ++i) {
            std::string name1 = std::string("##transformMatrix:") + std::to_string(i);
            ImGui::SetNextItemWidth(80);
            if (ImGui::InputFloat(name1.c_str(), &vectorSpaces[currentVs]->transformationMatrix[i], 0, 0, settings.GetDecimalPrecisionStr().c_str())) {
                updatedValues = true;
            }

            for (int j = 1; j < vectorSpaces[currentVs]->GetDimension(); ++j) {
                std::string name2 = std::string("##transformMatrix:") + std::to_string(i + j * 3);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(80);
                if (ImGui::InputFloat(name2.c_str(), &vectorSpaces[currentVs]->transformationMatrix[i + j * 3], 0, 0, settings.GetDecimalPrecisionStr().c_str())) {
                    updatedValues = true;
                }
            }
        }

        if (!updatedValues) {
            vectorSpaces[currentVs]->transformationMatrix[0] = vectorSpaces[currentVs]->GetBasisX().X();
            vectorSpaces[currentVs]->transformationMatrix[3] = vectorSpaces[currentVs]->GetBasisY().X();
            vectorSpaces[currentVs]->transformationMatrix[6] = vectorSpaces[currentVs]->GetBasisZ().X();

            vectorSpaces[currentVs]->transformationMatrix[1] = vectorSpaces[currentVs]->GetBasisX().Y();
            vectorSpaces[currentVs]->transformationMatrix[4] = vectorSpaces[currentVs]->GetBasisY().Y();
            vectorSpaces[currentVs]->transformationMatrix[7] = vectorSpaces[currentVs]->GetBasisZ().Y();

            vectorSpaces[currentVs]->transformationMatrix[2] = vectorSpaces[currentVs]->GetBasisX().Z();
            vectorSpaces[currentVs]->transformationMatrix[5] = vectorSpaces[currentVs]->GetBasisY().Z();
            vectorSpaces[currentVs]->transformationMatrix[8] = vectorSpaces[currentVs]->GetBasisZ().Z();

            updatedValues = false;
        }

        ImGui::BeginDisabled(!settings.drawGrid);
        ImGui::Checkbox("Draw transformed grid", &settings.drawTransformedGrid);
        ImGui::EndDisabled();

        ImGui::SameLine();

        ImGui::BeginDisabled(!(settings.drawVectorArrow || settings.drawVectorPoint || settings.drawVectorName || (settings.drawVectorLabel && (settings.drawVectorCoords || settings.drawCoordinateVectorPos))));
        ImGui::Checkbox("Draw basis vectors", &settings.drawBasisVectors);
        ImGui::EndDisabled();
    }
    ImGui::End();
}
