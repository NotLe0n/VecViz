#ifndef VECVIZ_UTILS_H
#define VECVIZ_UTILS_H

#include <vector>
#include <string>
#include "raylib.h"
#include "imgui.h"

std::vector<std::string> split(std::string s, const std::string& del);
Matrix MatrixLerp(Matrix m, float t);
ImGuiID DockFullScreen();

#endif //VECVIZ_UTILS_H
