#include <string>
#include "imgui.h"
#include "SubWindows.h"

float addVectorVals[] = {0, 0};
std::vector<bool> selectVectorList = {};

void DrawVectorsWindow(std::unique_ptr<VectorSpace>& currentVs) {
    ImGui::Begin("Vectors");
    {
        float numberInputWidth = ImGui::CalcTextSize("8.888").x;

        ImGui::SetNextItemWidth(numberInputWidth + 100);
        ImGui::InputFloat2("##AddVectorInput", addVectorVals);

        ImGui::SameLine();

        if (ImGui::Button("Add Vector")) {
            currentVs->vectors.emplace_back(addVectorVals[0], addVectorVals[1]);
            selectVectorList.push_back(false);
        }

        ImGui::SameLine();

        int selectIndex = -1;
        for (int i = 0; i < selectVectorList.size(); ++i) {
            if (selectVectorList[i]) {
                selectIndex = i;
                break;
            }
            selectIndex = -1;
        }

        ImGui::BeginDisabled(selectIndex == -1);
        if (ImGui::Button(" - ")) {
            currentVs->vectors.erase(currentVs->vectors.begin() + selectIndex);
            selectVectorList.pop_back();
        }
        ImGui::EndDisabled();

        if (ImGui::BeginListBox("##VectorList", ImVec2(400, 10 * ImGui::GetTextLineHeightWithSpacing()))) {
            for (int n = 0; n < currentVs->vectors.size(); n++) {
                float selectableHeight = ImGui::GetTextLineHeightWithSpacing() + 10;

                if (ImGui::Selectable(("##vector" + std::to_string(n)).c_str(), selectVectorList[n], 0, ImVec2(400, selectableHeight))) {
                    bool state = !selectVectorList[n];
                    std::fill(selectVectorList.begin(), selectVectorList.end(), false);
                    selectVectorList[n] = state;
                }
                ImGui::SetItemAllowOverlap();

                ImGui::SetCursorPos(ImVec2(10, selectableHeight * n + 15));
                ImGui::BeginGroup();
                {
                    ImGui::Text("v%d:", n);
                    ImGui::SameLine();

                    std::string xlabel = std::string("##VectorInputX") + std::to_string(n);
                    ImGui::SetNextItemWidth(numberInputWidth + 100);
                    ImGui::InputFloat(xlabel.c_str(), &currentVs->vectors[n].vector.x, 1);
                    ImGui::SetItemAllowOverlap();

                    ImGui::SameLine();

                    std::string ylabel = std::string("##VectorInputY") + std::to_string(n);
                    ImGui::SetNextItemWidth(numberInputWidth + 100);
                    ImGui::InputFloat(ylabel.c_str(), &currentVs->vectors[n].vector.y, 1);
                    ImGui::SetItemAllowOverlap();
                }
                ImGui::EndGroup();
                ImGui::SetItemAllowOverlap();
            }
            ImGui::EndListBox();
        }
    }
    ImGui::End();
}