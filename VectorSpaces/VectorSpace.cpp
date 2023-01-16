#include "VectorSpace.h"

float VectorSpace::drawOffset = 0;

RenderTexture* VectorSpace::GetRenderTexture() {
    return &rt;
}

std::vector<DrawVector> VectorSpace::GetVectors() {
    return vectors;
}
