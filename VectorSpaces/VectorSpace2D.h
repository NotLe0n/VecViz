#ifndef VECVIZ_VECTORSPACE2D_H
#define VECVIZ_VECTORSPACE2D_H

#include <string>
#include "VectorSpace.h"

class VectorSpace2D : public VectorSpace {
private:
    RenderTexture textTexture = LoadRenderTexture(rt.texture.width, rt.texture.height);

public:
    VectorSpace2D();

public:
    void Draw() override;
    void Update() override;
    void AddVector(const DrawVector& vector) override;
    void ApplyTransformation(Matrix transformationMatrix) override;

private:
    void DrawOrigGrid();
    void DrawYAxis();
    void DrawYAxisTicks();
    void DrawXAxis();
    void DrawXAxisTicks();
    void DrawVectors();
    void DrawAVector(DrawVector vector, const std::u16string& name, Color color);
    Vector2 VecToWorldSpace(Vector2 pos, Camera2D cam);
    static Vector2 WorldVecToScreenSpace(Vector2 pos, Camera2D cam);
};


#endif //VECVIZ_VECTORSPACE2D_H
