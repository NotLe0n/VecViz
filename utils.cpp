#include "utils.h"
#include "raylib.h"
#include "raymath.h"

std::vector<std::string> split(std::string s, const std::string& del) {
    size_t pos = 0;
    std::string token;
    std::vector<std::string> result;
    while ((pos = s.find(del)) != std::string::npos) {
        token = s.substr(0, pos);
        result.push_back(token);
        s.erase(0, pos + del.length());
    }
    result.push_back(token);

    return std::move(result);
}

int GetRealWindowHeight() {
    return GetMonitorHeight(GetCurrentMonitor()) - 63;
}

Matrix MatrixLerp(Matrix m, float t) {
    return {
            Lerp(1, m.m0, t), Lerp(0, m.m4, t), Lerp(0, m.m8, t), Lerp(0, m.m12, t),
            Lerp(0, m.m1, t), Lerp(1, m.m5, t), Lerp(0, m.m9, t), Lerp(0, m.m13, t),
            Lerp(0, m.m2, t), Lerp(0, m.m6, t), Lerp(1, m.m10, t), Lerp(0, m.m14, t),
            Lerp(0, m.m3, t), Lerp(0, m.m7, t), Lerp(0, m.m11, t), Lerp(1, m.m15, t),
    };
}
