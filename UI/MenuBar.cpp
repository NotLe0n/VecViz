#include "SubWindows.h"
#include "imgui.h"
#include <memory>
#include "../VectorSpaces/VectorSpace2D.h"

bool DrawMenuBar(std::unique_ptr<VectorSpace>& currentVs)
{
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::BeginMenu("New vector space")) {
                if (ImGui::MenuItem("1D vector space")) {

                }
                if (ImGui::MenuItem("2D vector space")) {
                    currentVs = std::make_unique<VectorSpace2D>();
                }
                if (ImGui::MenuItem("3D vector space")) {

                }
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Open...")) {

            }
            if (ImGui::MenuItem("Save...")) {

            }

            if (ImGui::MenuItem("Close all vector spaces")) {
                currentVs.reset();
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
            ImGui::MenuItem("Test");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options")) {
            ImGui::MenuItem("Test");

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    return true;
}