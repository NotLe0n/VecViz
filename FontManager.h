#ifndef VECVIZ_FONTMANAGER_H
#define VECVIZ_FONTMANAGER_H

#include "raylib.h"
#include "imgui.h"

class FontManager {
    static constexpr auto NotoFontLocation = "./Fonts/NotoSans.ttf";
    static constexpr auto FreeSerifFontLocation = "./Fonts/FreeSerif.ttf";
    static constexpr auto NotoGlyphsLocation = "./Fonts/NotoSans.glyphs";
    static constexpr auto FreeSerifGlyphsLocation = "./Fonts/FreeSerif.glyphs";

public:
    //static inline Font mathFont;
    static inline Font font;

private:
    static std::vector<int> ParseGlyphFile(const std::string&);

public:
    static void LoadFonts();
    static void UnloadFonts();

    static std::u16string NumToSubscript(int n);
    static char16_t ToSubscript(char16_t c);
};


#endif //VECVIZ_FONTMANAGER_H
