#ifndef VECVIZ_SUBWINDOWS_H
#define VECVIZ_SUBWINDOWS_H

#include <memory>
#include "../VectorSpaces/VectorSpace.h"

void DrawTransformationWindow(std::unique_ptr<VectorSpace>& currentVs);
void DrawVectorSpaceWindow(std::unique_ptr<VectorSpace>& currentVs);
void DrawVectorsWindow(std::unique_ptr<VectorSpace>& currentVs);

#endif //VECVIZ_SUBWINDOWS_H
