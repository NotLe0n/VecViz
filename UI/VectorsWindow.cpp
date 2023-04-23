#include <string>
#include "imgui.h"
#include "SubWindows.h"
#include "../Settings.h"
#include <algorithm>

float addVectorVals[] = {0, 0};

void DrawVectorsWindow(unsigned int currentVs, const std::vector<std::unique_ptr<VectorSpace>>& vectorSpaces)
{
    Settings& settings = Settings::GetSettings();

    if (!settings.showVectorsWindow) {
        return;
    }

    if (ImGui::Begin("Vectors", &settings.showVectorsWindow)) {
        float numberInputWidth = ImGui::CalcTextSize("8.88888").x;

        for (int i = 0; i < vectorSpaces[currentVs]->GetDimension(); ++i) {
            ImGui::SetNextItemWidth(numberInputWidth + 100);
            ImGui::InputFloat("##AddVectorInput", addVectorVals, 0, 0, settings.GetDecimalPrecisionStr().c_str());
            ImGui::SameLine();
        }

        // Add Button
        if (ImGui::Button("Add Vector")) {
            vectorSpaces[currentVs]->vectors.emplace_back(addVectorVals[0], addVectorVals[1]);
            //selectVectorList.push_back(false);
        }

        ImGui::SameLine();

        // Delete Button
        bool noneSelected = std::all_of(vectorSpaces[currentVs]->vectors.begin(), vectorSpaces[currentVs]->vectors.end(), [](DrawVector v) {return !v.selected;});
        ImGui::BeginDisabled(noneSelected);
        if (ImGui::Button(" - ")) {
            for (int n = 0; n < vectorSpaces[currentVs]->vectors.size(); n++) {
                if (vectorSpaces[currentVs]->vectors[n].selected) {
                    vectorSpaces[currentVs]->vectors.erase(vectorSpaces[currentVs]->vectors.begin() + n);
                    n--;
                }
            }
        }
        ImGui::EndDisabled();

        ImGui::SameLine();
        if (ImGui::Button("Cl")) {
            vectorSpaces[currentVs]->vectors.clear();
        }

        if (ImGui::BeginListBox("##VectorList", ImVec2(ImGui::GetWindowWidth() - 16, ImGui::GetWindowHeight() - 68))) {
            for (int n = 0; n < vectorSpaces[currentVs]->vectors.size(); n++) {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));
                ImGui::PushID(n);

                const std::string indexStr = std::to_string(n);
                DrawVector& vector = vectorSpaces[currentVs]->vectors[n];

                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 3));

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
                ImGui::Selectable("", &vector.selected, ImGuiSelectableFlags_AllowItemOverlap);
                ImGui::PopStyleVar();

                ImGui::SameLine();

                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 3));
                ImGui::ColorEdit4(("##VectorColorSelect" + indexStr).c_str(), (float*)&vector.color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoBorder);

                ImGui::SameLine();

                ImGui::Text("v%d:", n);

                for (int i = 0; i < vectorSpaces[currentVs]->GetDimension(); ++i) {
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(numberInputWidth + 100);
                    std::string str = "##VectorInputX" + indexStr + "_" + std::to_string(i);
                    float* ptr = i == 0 ? &vector.vector.x : (i == 1 ? &vector.vector.y : &vector.vector.z);
                    ImGui::InputFloat(str.c_str(), ptr, 1, 0, settings.GetDecimalPrecisionStr().c_str());
                }

                ImGui::PopStyleVar();
                ImGui::PopID();
            }
            ImGui::EndListBox();
        }
    }
    ImGui::End();
}
