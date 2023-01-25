#include "VectorSpace.h"

Vector2 VectorSpace::drawOffset = {};

RenderTexture* VectorSpace::GetRenderTexture()
{
    return &rt;
}

DrawVector VectorSpace::GetBasisX()
{
    return BasisX;
}

DrawVector VectorSpace::GetBasisY()
{
    return BasisY;
}

DrawVector VectorSpace::GetBasisZ()
{
    return BasisZ;
}

Matrix VectorSpace::GetTransformationMatrix()
{
    return Matrix{
        transformationMatrix[0], transformationMatrix[3], transformationMatrix[6], 0,
        transformationMatrix[1], transformationMatrix[4], transformationMatrix[7], 0,
        transformationMatrix[2], transformationMatrix[5], transformationMatrix[8], 0,
        0, 0, 0, 1};
}
