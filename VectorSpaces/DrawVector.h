#ifndef VECVIZ_DRAWVECTOR_H
#define VECVIZ_DRAWVECTOR_H

#include "raylib.h"

class DrawVector {
private:
    static int vectorCount;

public:
    int vectorIndex = 0;
    Vector3 vector;
    Vector3 origin;
    Color color{};

public:
    explicit DrawVector(float x = 0, float y = 0, float z = 0);

private:
    Color GetRandomColor();

public:
    [[nodiscard]] float X() const;
    [[nodiscard]] float Y() const;
    [[nodiscard]] float Z() const;
};


#endif //VECVIZ_DRAWVECTOR_H
