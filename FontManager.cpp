#include <sstream>
#include <vector>
#include "FontManager.h"
#include "fstream"
#include "utils.h"

void FontManager::LoadFonts() {
    std::vector<int> notoGlyphs = ParseGlyphFile(NotoGlyphsLocation);
    std::vector<int> freeSerifGlyphs = ParseGlyphFile(FreeSerifGlyphsLocation);

    mathFont = LoadFontEx(FreeSerifFontLocation, 64, freeSerifGlyphs.data(), (int)freeSerifGlyphs.size());
    font = LoadFontEx(NotoFontLocation, 64, notoGlyphs.data(), (int)notoGlyphs.size());
}

std::vector<int> FontManager::ParseGlyphFile(const std::string& path) {
    std::ifstream t(path);
    if (!t.is_open()) {
       throw std::exception((std::string("Couldn't open ") + path).c_str());
    }

    std::stringstream buffer;
    buffer << t.rdbuf();

    std::vector<std::string> glyphsStr = split(buffer.str(), ", ");
    std::vector<int> glyphs = std::vector<int>();
    for (const auto &item: glyphsStr) {
        glyphs.push_back(std::stoi(item));
    }

    return glyphs;
}

std::u16string FontManager::NumToSubscript(int n) {
    std::u16string s;
    do {
        int lastDigit = n % 10;
        n /= 10;

        s = ToSubscript(0x30 | lastDigit) + s;
    } while (n != 0);

    return s;
}

char16_t FontManager::ToSubscript(char16_t c) {
    if (isdigit(c)) {
        return 0x2080 | c - '0';
    }

    char32_t a;
    switch (c) {
        case '+':
            a = 0x8A;
            break;
        case '-':
            a = 0x8B;
            break;
        case '=':
            a = 0x8C;
            break;
        case '(':
            a = 0x8D;
            break;
        case ')':
            a = 0x8E;
            break;
        case 'a':
            a = 0x90;
            break;
        case 'e':
            a = 0x91;
            break;
        case 'o':
            a = 0x92;
            break;
        case 'x':
            a = 0x95;
            break;
        case 'k':
            a = 0x96;
            break;
        case 'l':
            a = 0x97;
            break;
        case 'm':
            a = 0x98;
            break;
        case 'n':
            a = 0x99;
            break;
        case 'p':
            a = 0x9A;
            break;
        case 's':
            a = 0x9B;
            break;
        default:
            throw std::exception("No subscript character found!");
    }

    return 0x20 | a;
}

