#ifndef VECVIZ_UIWINDOW_H
#define VECVIZ_UIWINDOW_H

#include <functional>

namespace UIWindow {
    bool Init();
    // Begins draw loop
    void Draw(const std::function<bool()>&);
    void CloseCurrentWindow();
}

#endif //VECVIZ_UIWINDOW_H
