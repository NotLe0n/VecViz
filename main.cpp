#include "UI/UIWindow.h"
#include "VectorSpaces/VectorSpace.h"
#include "VectorSpaces/VectorSpace2D.h"
#include "rlImGui.h"
#include "imgui.h"

void UpdateDrawFrame();

VectorSpace* currentVs = nullptr;

int main()
{
    // create window
    UIWindow& wnd = UIWindow::GetWindow();

    // Temporary until VS selection is implemented
    currentVs = new VectorSpace2D();
    currentVs->AddVector(DrawVector{2, 4});
    currentVs->AddVector(DrawVector{sqrtf(2), 3});
    currentVs->AddVector(DrawVector{-10, -7});

    wnd.Draw(UpdateDrawFrame);

    return 0;
}

void UpdateDrawFrame()
{
    DrawRectangle(0, 0, 300, GetScreenHeight(), {33, 33, 33, 255}); // Sidebar rectangle
    rlImGuiBegin();
    //ImGui::Text("hey");
    rlImGuiEnd();
    currentVs->Render();
}