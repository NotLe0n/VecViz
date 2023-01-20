#ifndef VECVIZ_FONTMANAGER_H
#define VECVIZ_FONTMANAGER_H

#include "raylib.h"
#include "imgui.h"

namespace FontManager {
    //static Font mathFont;
    Font& GetFont();

    void LoadFonts();
    void UnloadFonts();

    std::u16string NumToSubscript(int n);
    char16_t ToSubscript(char16_t c);
};


#endif //VECVIZ_FONTMANAGER_H
