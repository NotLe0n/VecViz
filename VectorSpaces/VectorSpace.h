#include "DrawVector.h"
#include "vector"
#include "raymath.h"

#ifndef VECVIZ_VECTORSPACE_H
#define VECVIZ_VECTORSPACE_H

class VectorSpace {
public:
    static Vector2 drawOffset;

protected:
    RenderTexture rt = LoadRenderTexture(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));

public:
    float t = 0; // Transformation percentage
    std::vector<DrawVector> vectors{}; // list of vectors

protected:
    DrawVector BasisX{1}; // Basis Vector for the X Direction
    DrawVector BasisY{0, 1}; // Basis Vector for the Y Direction
    DrawVector BasisZ{0, 0, 1}; // Basis Vector for the Z Direction
    Matrix transformationMatrix = MatrixIdentity();

public:
    virtual int GetDimension() = 0;
    virtual void Draw() = 0;
    virtual void Update() = 0;
    virtual void AddVector(const DrawVector& vector) = 0;
    virtual void ApplyTransformation(Matrix transformationMatrix) = 0;
    RenderTexture* GetRenderTexture();
};

#endif //VECVIZ_VECTORSPACE_H
