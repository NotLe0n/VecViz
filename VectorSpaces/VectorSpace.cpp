#include "VectorSpace.h"

Vector2 VectorSpace::drawOffset = {};

RenderTexture* VectorSpace::GetRenderTexture()
{
    return &rt;
}
