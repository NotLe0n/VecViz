#include "SubWindows.h"
#include "imgui.h"
#include "rlImGui.h"
#include "../utils.h"

void DrawVectorSpaceWindow(unsigned int& currentVs, std::vector<std::unique_ptr<VectorSpace>>& vectorSpaces)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    
    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&window_class);
    if (ImGui::Begin("Vector space view", nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse)) {
        for (int i = 0; i < vectorSpaces.size(); ++i) {
            if (!vectorSpaces[i]->windowOpen) {
                if (i == currentVs) {
                    currentVs--;
                }
                vectorSpaces.erase(vectorSpaces.begin() + i);
                i--;
            }
        }

        if (vectorSpaces.empty()) {
            TextCentered("Create a new vector space under 'File -> New Vector Space...'");
            ImGui::End();
            ImGui::PopStyleVar(2);
            return;
        }

        if (ImGui::IsWindowHovered()) {
            vectorSpaces[currentVs]->Update();
        }
        vectorSpaces[currentVs]->Draw();

        ImGui::BeginTabBar("Vector space tabs");
        for (int i = 0; i < vectorSpaces.size(); ++i) {
            std::string vsName = "Vectorspace " + std::to_string(vectorSpaces[i]->GetDimension()) + "D #" + std::to_string(i + 1);

            if (ImGui::BeginTabItem(vsName.c_str(), &vectorSpaces[i]->windowOpen)) {
                currentVs = i;
                Texture* renderTexture = &vectorSpaces[currentVs]->GetRenderTexture()->texture;
                VectorSpace::drawOffset = {ImGui::GetWindowPos().x, ImGui::GetWindowPos().y};

                float rtw = ImGui::GetWindowWidth();
                float rth = ImGui::GetWindowHeight() - ImGui::GetFrameHeight();
                rlImGuiImageRect(renderTexture, rtw, rth, {0, 0, rtw, rth});

                if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered()) {
                    ImGui::OpenPopup("rightClickPopup");
                }

                if (ImGui::BeginPopup("rightClickPopup")) {
                    ImVec2 pos = ImGui::GetMousePosOnOpeningCurrentPopup();
                    Vector3 newVec = vectorSpaces[currentVs]->VecToWorldSpace({pos.x - vectorSpaces[currentVs]->drawOffset.x, pos.y - vectorSpaces[currentVs]->drawOffset.y - ImGui::GetFrameHeight()});

                    if (ImGui::Button("Add vector here")) {
                        vectorSpaces[currentVs]->vectors.emplace_back(newVec.x, newVec.y, newVec.z);
                        ImGui::CloseCurrentPopup();
                    }

                    if (ImGui::Button("Transform X basis Vector here")) {
                        vectorSpaces[currentVs]->ApplyTransformation({
                            newVec.x, vectorSpaces[currentVs]->GetBasisY().X(), vectorSpaces[currentVs]->GetBasisZ().X(), 0,
                            newVec.y, vectorSpaces[currentVs]->GetBasisY().Y(), vectorSpaces[currentVs]->GetBasisZ().Y(), 0,
                            newVec.z, vectorSpaces[currentVs]->GetBasisY().Z(), vectorSpaces[currentVs]->GetBasisZ().Z(), 0,
                            0, 0, 0, 1
                        });
                    }
                    if (vectorSpaces[currentVs]->GetDimension() > 1) {
                        if (ImGui::Button("Transform Y basis Vector here")) {
                            vectorSpaces[currentVs]->ApplyTransformation({
                                   vectorSpaces[currentVs]->GetBasisX().X(), newVec.x, vectorSpaces[currentVs]->GetBasisZ().X(), 0,
                                   vectorSpaces[currentVs]->GetBasisX().Y(), newVec.y, vectorSpaces[currentVs]->GetBasisZ().Y(), 0,
                                   vectorSpaces[currentVs]->GetBasisX().Z(), newVec.z, vectorSpaces[currentVs]->GetBasisZ().Z(), 0,
                                   0, 0, 0, 1
                           });
                        }
                    }
                    if (vectorSpaces[currentVs]->GetDimension() > 2) {
                        if (ImGui::Button("Transform Z basis Vector here")) {
                            vectorSpaces[currentVs]->ApplyTransformation({
                                   vectorSpaces[currentVs]->GetBasisX().X(), vectorSpaces[currentVs]->GetBasisY().X(), newVec.x, 0,
                                   vectorSpaces[currentVs]->GetBasisX().Y(), vectorSpaces[currentVs]->GetBasisY().Y(), newVec.y, 0,
                                   vectorSpaces[currentVs]->GetBasisX().Z(), vectorSpaces[currentVs]->GetBasisY().Z(), newVec.z, 0,
                                   0, 0, 0, 1
                           });
                        }
                    }
                    ImGui::EndPopup();
                }
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
}
