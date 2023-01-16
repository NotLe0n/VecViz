#ifndef VECVIZ_UTILS_H
#define VECVIZ_UTILS_H

#include <vector>
#include <string>
#include "raylib.h"

std::vector<std::string> split(std::string s, const std::string& del);
int GetRealWindowHeight();
Matrix MatrixLerp(Matrix m, float t);

#endif //VECVIZ_UTILS_H
