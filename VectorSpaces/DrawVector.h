#ifndef VECVIZ_DRAWVECTOR_H
#define VECVIZ_DRAWVECTOR_H

#include "raylib.h"
#include "imgui.h"

class DrawVector {
private:
    static int vectorCount;

public:
    int vectorIndex = 0;
    Vector3 vector;
    Vector3 origin;
    ImVec4 color;
    bool selected;

public:
    explicit DrawVector(float x = 0, float y = 0, float z = 0);

private:
    [[nodiscard]] Color GetRandomColor() const;

public:
    [[nodiscard]] Color GetColor() const;
    [[nodiscard]] float X() const;
    [[nodiscard]] float Y() const;
    [[nodiscard]] float Z() const;
};


#endif //VECVIZ_DRAWVECTOR_H
