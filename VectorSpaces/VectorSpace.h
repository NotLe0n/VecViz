#ifndef VECVIZ_VECTORSPACE_H
#define VECVIZ_VECTORSPACE_H

#include "DrawVector.h"
#include "vector"
#include "raymath.h"

// Abstract class
class VectorSpace {
public:
    static Vector2 drawOffset;

    virtual ~VectorSpace() = default;

protected:
    RenderTexture rt = LoadRenderTexture(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));

public:
    float t = 0; // Transformation percentage
    std::vector<DrawVector> vectors{}; // list of vectors
    float transformationMatrix[9] = {1, 0, 0,
                                     0, 1, 0,
                                     0, 0, 1};

protected:
    DrawVector BasisX{1}; // Basis vector for the X direction
    DrawVector BasisY{0, 1}; // Basis vector for the Y direction
    DrawVector BasisZ{0, 0, 1}; // Basis vector for the Z direction

public:
    RenderTexture* GetRenderTexture();
    DrawVector GetBasisX();
    DrawVector GetBasisY();
    DrawVector GetBasisZ();
    Matrix GetTransformationMatrix();
    bool windowOpen = true;

public:
    virtual int GetDimension() = 0;

    virtual void Draw() = 0;
    virtual void Update() = 0;

    virtual void ApplyTransformation(Matrix transformationMatrix) = 0;

    virtual Vector3 VecToWorldSpace(Vector3 pos) = 0;
    virtual Vector3 WorldVecToScreenSpace(Vector3 pos) = 0;
};

#endif //VECVIZ_VECTORSPACE_H
