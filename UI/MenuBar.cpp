#include "SubWindows.h"
#include "imgui.h"
#include <memory>
#include "../VectorSpaces/VectorSpace2D.h"
#include "../Settings.h"
#include "../VectorSpaces/VectorSpace1D.h"

bool DrawMenuBar(unsigned int& currentVs, std::vector<std::unique_ptr<VectorSpace>>& vectorSpaces)
{
    Settings& settings = Settings::GetSettings();

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::BeginMenu("New vector space")) {
                if (ImGui::MenuItem("1D vector space")) {
                    vectorSpaces.push_back(std::make_unique<VectorSpace1D>());
                    currentVs = vectorSpaces.size() - 1;
                }
                if (ImGui::MenuItem("2D vector space")) {
                    vectorSpaces.push_back(std::make_unique<VectorSpace2D>());
                    currentVs = vectorSpaces.size() - 1;
                }
                if (ImGui::MenuItem("3D vector space")) {

                }
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Open...")) {
                // TODO
            }
            if (ImGui::MenuItem("Save...")) {
                // TODO
            }

            if (ImGui::MenuItem("Close current vector spaces")) {
                vectorSpaces.erase(vectorSpaces.begin() + currentVs);
            }

            if (ImGui::MenuItem("Close all vector spaces")) {
                vectorSpaces.clear();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Exit")) {
                ImGui::EndMenu();
                ImGui::EndMenuBar();
                ImGui::End();
                return false;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Show vectors window", nullptr, &settings.showVectorsWindow);
            ImGui::MenuItem("Show transformations window", nullptr, &settings.showTransformationsWindow);

            if (ImGui::MenuItem("Open options...")) {
                settings.showOptionsWindow = true;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    return true;
}
