#include "SubWindows.h"
#include "../Settings.h"
#include "../CalcInterpreter.h"
#include "sstream"

void DrawCalcWindow(unsigned int& currentVs, std::vector<std::unique_ptr<VectorSpace>>& vectorSpaces) {
    Settings& settings = Settings::GetSettings();
    if (!settings.showCalcWindow) {
        return;
    }

    static char calcText[100];
    static std::variant<Vector3, float> solution;
    if (ImGui::Begin("Calculations", &settings.showCalcWindow)) {
        ImGui::InputText("##calcInput", calcText, 100);
        ImGui::SameLine();
        if (ImGui::Button("Calculate")) {
            solution = CalcInterpreter::Interpret(calcText, vectorSpaces[currentVs]->vectors);
            std::stringstream stringBuilder;
            stringBuilder.precision(4);
            if (solution.index() == 0) {
                Vector3 sv = std::get<Vector3>(solution);
                stringBuilder << "{ " << sv.x << ", " << sv.y << ", " << sv.z << " }";
            }
            else {
                stringBuilder << std::get<float>(solution);
            }

            strcpy(calcText, stringBuilder.str().c_str());
        }
        ImGui::SameLine();
        if (ImGui::Button("Add Vector") && solution.index() == 0) {
            Vector3 sv = std::get<Vector3>(solution);
            vectorSpaces[currentVs]->vectors.emplace_back(sv.x, sv.y, sv.z);
        }
    }
    ImGui::End();
}
