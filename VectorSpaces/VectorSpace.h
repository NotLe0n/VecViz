#include "DrawVector.h"
#include "vector"

#ifndef VECVIZ_VECTORSPACE_H
#define VECVIZ_VECTORSPACE_H

class VectorSpace {
public:
    static float drawOffset;

protected:
    RenderTexture rt = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

protected:
    std::vector<DrawVector> vectors{};
    DrawVector BasisX{1};
    DrawVector BasisY{0, 1};
    DrawVector BasisZ{0, 0, 1};

public:
    virtual void Draw() = 0;
    virtual void Update() = 0;
    virtual void AddVector(const DrawVector& vector) = 0;
    virtual void ApplyTransformation(Matrix transformationMatrix) = 0;
    RenderTexture* GetRenderTexture();
    std::vector<DrawVector> GetVectors();
};

#endif //VECVIZ_VECTORSPACE_H
