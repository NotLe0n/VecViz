#include "SubWindows.h"
#include "imgui.h"
#include "rlImGui.h"
#include "../utils.h"

void DrawVectorSpaceWindow(std::unique_ptr<VectorSpace>& currentVs)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    if (ImGui::Begin("Vectorspace view"), 0, ImGuiWindowFlags_NoDocking) {
        if (!currentVs) {
            TextCentered("Create a new vector space under 'File -> New Vector Space...'");
            ImGui::End();
            ImGui::PopStyleVar(2);
            return;
        }

        Texture* renderTexture = &currentVs->GetRenderTexture()->texture;
        VectorSpace::drawOffset = {ImGui::GetWindowPos().x, ImGui::GetWindowPos().y};

        float rtw = ImGui::GetWindowWidth();
        float rth = ImGui::GetWindowHeight() - ImGui::GetFrameHeight();
        rlImGuiImageRect(renderTexture, rtw, rth, {0, 0, rtw, rth});

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("rightClickPopup");
        }

        if (ImGui::BeginPopup("rightClickPopup")) {
            ImVec2 pos = ImGui::GetMousePosOnOpeningCurrentPopup();
            Vector3 newVec = currentVs->VecToWorldSpace({pos.x - currentVs->drawOffset.x, pos.y - currentVs->drawOffset.y - ImGui::GetFrameHeight()});

            if (ImGui::Button("Add vector here")) {
                Vector3 newTransformedVec = Vector3Transform(newVec, MatrixLerp(currentVs->GetTransformationMatrix(), currentVs->t));
                currentVs->vectors.emplace_back(newTransformedVec.x, newTransformedVec.y, newTransformedVec.z);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Transform X basis Vector here")) {
                currentVs->ApplyTransformation({
                    newVec.x, currentVs->GetBasisY().X(), currentVs->GetBasisZ().X(), 0,
                    newVec.y, currentVs->GetBasisY().Y(), currentVs->GetBasisZ().Y(), 0,
                    newVec.z, currentVs->GetBasisY().Z(), currentVs->GetBasisZ().Z(), 0,
                    0, 0, 0, 1
                });
            }
            if (currentVs->GetDimension() > 1) {
                if (ImGui::Button("Transform Y basis Vector here")) {
                    currentVs->ApplyTransformation({
                           currentVs->GetBasisX().X(), newVec.x, currentVs->GetBasisZ().X(), 0,
                           currentVs->GetBasisX().Y(), newVec.y, currentVs->GetBasisZ().Y(), 0,
                           currentVs->GetBasisX().Z(), newVec.z, currentVs->GetBasisZ().Z(), 0,
                           0, 0, 0, 1
                   });
                }
            }
            if (currentVs->GetDimension() > 2) {
                if (ImGui::Button("Transform Z basis Vector here")) {
                    currentVs->ApplyTransformation({
                           currentVs->GetBasisX().X(), currentVs->GetBasisY().X(), newVec.x, 0,
                           currentVs->GetBasisX().Y(), currentVs->GetBasisY().Y(), newVec.y, 0,
                           currentVs->GetBasisX().Z(), currentVs->GetBasisY().Z(), newVec.z, 0,
                           0, 0,    0,     1
                   });
                }
            }
            ImGui::EndPopup();
        }

        if (ImGui::IsWindowHovered()) {
            currentVs->Update();
        }
        currentVs->Draw();

        ImGui::End();
    }
    ImGui::PopStyleVar(2);
}
