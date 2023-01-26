#ifndef VECVIZ_SUBWINDOWS_H
#define VECVIZ_SUBWINDOWS_H

#include <memory>
#include "../VectorSpaces/VectorSpace.h"

void DrawTransformationWindow(int currentVs, const std::vector<std::unique_ptr<VectorSpace>>& vectorSpaces);
void DrawVectorSpaceWindow(int& currentVs, const std::vector<std::unique_ptr<VectorSpace>>& vectorSpaces);
void DrawVectorsWindow(int currentVs, const std::vector<std::unique_ptr<VectorSpace>>& vectorSpaces);
bool DrawMenuBar(int& currentVs, std::vector<std::unique_ptr<VectorSpace>>& vectorSpaces);
void DrawOptionsWindow();

#endif //VECVIZ_SUBWINDOWS_H
