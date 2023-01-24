#include <string>
#include "imgui.h"
#include "SubWindows.h"
#include "../Settings.h"
#include <algorithm>

float addVectorVals[] = {0, 0};
std::vector<char> selectVectorList = {}; // replacement for vector<bool>

void DrawVectorsWindow(std::unique_ptr<VectorSpace>& currentVs)
{
    if (!currentVs) {
        return;
    }

    Settings& settings = Settings::GetSettings();

    if (!settings.showVectorsWindow) {
        return;
    }

    if (ImGui::Begin("Vectors", &settings.showVectorsWindow))
    {
        float numberInputWidth = ImGui::CalcTextSize("8.88888").x;

        ImGui::SetNextItemWidth(numberInputWidth + 100);
        ImGui::InputFloat2("##AddVectorInput", addVectorVals, settings.GetDecimalPrecisionStr().c_str());

        ImGui::SameLine();

        // Add Button
        if (ImGui::Button("Add Vector")) {
            currentVs->vectors.emplace_back(addVectorVals[0], addVectorVals[1]);
            //selectVectorList.push_back(false);
        }

        ImGui::SameLine();

        // Delete Button
        bool noneSelected = std::all_of(currentVs->vectors.begin(), currentVs->vectors.end(), [](DrawVector v) {return !v.selected;});
        ImGui::BeginDisabled(noneSelected);
        if (ImGui::Button(" - ")) {
            for (int n = 0; n < currentVs->vectors.size(); n++) {
                if (currentVs->vectors[n].selected) {
                    currentVs->vectors.erase(currentVs->vectors.begin() + n);
                    //selectVectorList.erase(selectVectorList.begin() + n);
                    n--;
                }
            }
        }
        ImGui::EndDisabled();

        if (ImGui::BeginListBox("##VectorList", ImVec2(500, 10 * ImGui::GetTextLineHeightWithSpacing()))) {
            for (int n = 0; n < currentVs->vectors.size(); n++) {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));
                ImGui::PushID(n);

                const std::string indexStr = std::to_string(n);
                DrawVector& vector = currentVs->vectors[n];

                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 3));

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
                ImGui::Selectable("", &vector.selected/*(bool*)&selectVectorList[n]*/, ImGuiSelectableFlags_AllowItemOverlap);
                ImGui::PopStyleVar();

                ImGui::SameLine();

                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y - 3));

                ImGui::ColorEdit4(("##VectorColorSelect" + indexStr).c_str(), (float*)&vector.color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoBorder);

                ImGui::SameLine();

                ImGui::Text("v%d:", n);

                ImGui::SameLine();

                ImGui::SetNextItemWidth(numberInputWidth + 100);
                ImGui::InputFloat(("##VectorInputX" + indexStr).c_str(), &vector.vector.x, 1, 0, settings.GetDecimalPrecisionStr().c_str());

                ImGui::SameLine();

                ImGui::SetNextItemWidth(numberInputWidth + 100);
                ImGui::InputFloat(("##VectorInputY" + indexStr).c_str(), &vector.vector.y, 1, 0, settings.GetDecimalPrecisionStr().c_str());

                ImGui::PopStyleVar();
                ImGui::PopID();
            }
            ImGui::EndListBox();
        }
        ImGui::End();
    }
}
