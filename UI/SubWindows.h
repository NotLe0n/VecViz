#ifndef VECVIZ_SUBWINDOWS_H
#define VECVIZ_SUBWINDOWS_H

#include <memory>
#include "../VectorSpaces/VectorSpace.h"

void DrawTransformationWindow(unsigned int currentVs, const std::vector<std::unique_ptr<VectorSpace>>& vectorSpaces);
void DrawVectorSpaceWindow(unsigned int& currentVs, std::vector<std::unique_ptr<VectorSpace>>& vectorSpaces);
void DrawVectorsWindow(unsigned int currentVs, const std::vector<std::unique_ptr<VectorSpace>>& vectorSpaces);
bool DrawMenuBar(unsigned int& currentVs, std::vector<std::unique_ptr<VectorSpace>>& vectorSpaces);
void DrawOptionsWindow();
void DrawCalcWindow(unsigned int& currentVs, std::vector<std::unique_ptr<VectorSpace>>& vectorSpaces);

#endif //VECVIZ_SUBWINDOWS_H
