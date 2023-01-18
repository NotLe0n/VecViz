#ifndef VECVIZ_UTILS_H
#define VECVIZ_UTILS_H

#include <vector>
#include <string>
#include "raylib.h"
#include "imgui.h"

std::vector<std::string> split(std::string s, const std::string& del);
Matrix MatrixLerp(Matrix m, float t);
ImGuiID DockFullScreen();
void TextCentered(const std::string& text);

#endif //VECVIZ_UTILS_H
