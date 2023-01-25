#ifndef VECVIZ_VECTORSPACE1D_H
#define VECVIZ_VECTORSPACE1D_H

#include <string>
#include "VectorSpace.h"

class VectorSpace1D : public VectorSpace {
public:
    VectorSpace1D();
    ~VectorSpace1D() override;

private:
    float worldStart = 0;
    float worldEnd = 0;
    Camera2D camera{};
    RenderTexture textTexture = LoadRenderTexture(rt.texture.width, rt.texture.height);
    int step = 1;
    const float labelFontSize = 18;

public:
    int GetDimension() override;

    void Draw() override;
    void Update() override;

    void ApplyTransformation(Matrix transformationMatrix) override;

    Vector3 VecToWorldSpace(Vector3 pos) override;
    Vector3 WorldVecToScreenSpace(Vector3 pos) override;

private:
    void DrawVectors();
    void DrawAVector(DrawVector vector, const std::u16string& name, Color color, float t);
    void DrawAxis();
    void DrawAxisTicks();
};


#endif //VECVIZ_VECTORSPACE1D_H
