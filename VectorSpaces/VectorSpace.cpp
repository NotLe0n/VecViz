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
    return transformationMatrix;
}
