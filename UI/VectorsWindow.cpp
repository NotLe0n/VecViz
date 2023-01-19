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
            selectVectorList.push_back(false);
        }

        ImGui::SameLine();

        // Delete Button
        bool noneSelected = std::all_of(selectVectorList.begin(), selectVectorList.end(), [](auto v) {return !v;});
        ImGui::BeginDisabled(noneSelected);
        if (ImGui::Button(" - ")) {
            for (int n = 0; n < currentVs->vectors.size(); n++) {
                if (selectVectorList[n]) {
                    currentVs->vectors.erase(currentVs->vectors.begin() + n);
                    selectVectorList.erase(selectVectorList.begin() + n);
                    n--;
                }
            }
        }
        ImGui::EndDisabled();

        if (ImGui::BeginListBox("##VectorList", ImVec2(500, 10 * ImGui::GetTextLineHeightWithSpacing()))) {
            for (int n = 0; n < currentVs->vectors.size(); n++) {
                ImGui::BeginGroup();
                {
                    std::string checkLabel = std::string("##SelectedBox") + std::to_string(n);
                    ImGui::Checkbox(checkLabel.c_str(), (bool*)&selectVectorList[n]);
                    ImGui::SameLine();
                    ImGui::Text("v%d:", n);
                    ImGui::SameLine();

                    std::string xlabel = std::string("##VectorInputX") + std::to_string(n);
                    ImGui::SetNextItemWidth(numberInputWidth + 100);
                    ImGui::InputFloat(xlabel.c_str(), &currentVs->vectors[n].vector.x, 1, 0, settings.GetDecimalPrecisionStr().c_str());
                    ImGui::SetItemAllowOverlap();

                    ImGui::SameLine();

                    std::string ylabel = std::string("##VectorInputY") + std::to_string(n);
                    ImGui::SetNextItemWidth(numberInputWidth + 100);
                    ImGui::InputFloat(ylabel.c_str(), &currentVs->vectors[n].vector.y, 1, 0, settings.GetDecimalPrecisionStr().c_str());
                    ImGui::SetItemAllowOverlap();
                }
                ImGui::EndGroup();
            }
            ImGui::EndListBox();
        }
        ImGui::End();
    }
}
