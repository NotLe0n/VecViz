#include "utils.h"
#include "raylib.h"

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