#include "SubWindows.h"
#include "../Settings.h"
#include "../CalcInterpreter.h"

void DrawCalcWindow(unsigned int& currentVs, std::vector<std::unique_ptr<VectorSpace>>& vectorSpaces) {
    Settings& settings = Settings::GetSettings();
    if (!settings.showCalcWindow) {
        return;
    }

    static char calcText[100];
    static std::vector<std::tuple<std::string, CalcInterpreter::CalcVal>> calcHistory;
    if (ImGui::Begin("Calculations", &settings.showCalcWindow)) {
        if (ImGui::BeginListBox("##CalcHistory", {ImGui::GetWindowWidth() - 15, ImGui::GetWindowHeight() - 72})) {
            for (int i = 0; i < calcHistory.size(); ++i) {
                ImGui::SetCursorPosY(i * (ImGui::GetFontSize() + 3));

                std::string calStr = std::get<0>(calcHistory[i]);
                ImGui::Text("%s", calStr.c_str());

                CalcInterpreter::CalcVal solVal = std::get<1>(calcHistory[i]);
                std::string solStr = CalcInterpreter::calVal_to_string(solVal);
                ImGui::SameLine(ImGui::GetWindowWidth() / 2);
                ImGui::Text("= %s", solStr.c_str());

                ImGui::SameLine();
                ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 64);
                if (ImGui::Button(("C##" + std::to_string(i)).c_str())) {
                    strcpy_s(calcText, calStr.c_str());
                }

                ImGui::SameLine();
                if (ImGui::Button(("D##" + std::to_string(i)).c_str())) {
                    calcHistory.erase(calcHistory.begin() + i);
                }

                if (solVal.index() == 0) {
                    ImGui::SameLine();
                    if (ImGui::Button(("+##" + std::to_string(i)).c_str())) {
                        Vector3 sv = std::get<Vector3>(solVal);
                        vectorSpaces[currentVs]->vectors.emplace_back(sv.x, sv.y, sv.z);
                    }
                }
            }
            ImGui::EndListBox();
        }

        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 32);
        ImGui::InputText("##calcInput", calcText, 100);
        ImGui::SameLine();
        if (ImGui::Button("=")) {
            try {
                CalcInterpreter::CalcVal solution = CalcInterpreter::Interpret(calcText, vectorSpaces[currentVs]->vectors);
                std::string solStr = CalcInterpreter::calVal_to_string(solution);

                calcHistory.emplace_back(calcText, solution);
                strcpy_s(calcText, solStr.c_str());
            }
            catch (std::exception&){}
        }

        ImGui::SameLine();
        if (ImGui::Button("Cl")) {
            strcpy_s(calcText, ""); // clear text box
        }
    }
    ImGui::End();
}
