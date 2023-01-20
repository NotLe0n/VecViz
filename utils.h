#ifndef VECVIZ_UTILS_H
#define VECVIZ_UTILS_H

#include <vector>
#include <string>
#include "raylib.h"
#include "imgui.h"

// splits a string by the given delimiter
std::vector<std::string> split(std::string s, const std::string& del);

// Lerps a vector from the identity to 'm' with the control value 't'.
Matrix MatrixLerp(Matrix m, float t);

constexpr Vector3 V2ToV3(const Vector2& vector) {
    return {vector.x, vector.y, 0};
}

// Creates a new fullscreen dock-space window.
ImGuiID DockFullScreen();

// Draws text centered to the middle of the window
void TextCentered(const std::string& text);

ImVec4 ColorToImVec4(Color color);

#endif //VECVIZ_UTILS_H
