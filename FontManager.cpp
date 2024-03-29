#include <sstream>
#include <vector>
#include "FontManager.h"
#include "fstream"
#include "utils.h"
#include "rlImGui.h"
#include "raymath.h"

namespace FontManager
{
    static constexpr auto NotoFontLocation = "./Fonts/NotoSans.ttf";
    static constexpr auto FreeSerifFontLocation = "./Fonts/FreeSerif.ttf";
    static constexpr auto NotoGlyphsLocation = "./Fonts/NotoSans.glyphs";
    static constexpr auto FreeSerifGlyphsLocation = "./Fonts/FreeSerif.glyphs";

    static Font font;
    Font& GetFont() {
        return font;
    }

    static std::vector<int> ParseGlyphFile(const std::string& path);

    // Needs to be called after rlImGuiSetup()
    void LoadFonts()
    {
        std::vector<int> notoGlyphs = ParseGlyphFile(NotoGlyphsLocation);
        //std::vector<int> freeSerifGlyphs = ParseGlyphFile(FreeSerifGlyphsLocation);

        font = LoadFontEx(NotoFontLocation, 128, notoGlyphs.data(), (int)notoGlyphs.size());
        //mathFont = LoadFontEx(FreeSerifFontLocation, 64, freeSerifGlyphs.data(), (int)freeSerifGlyphs.size());

        SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
        //SetTextureFilter(mathFont.texture, TEXTURE_FILTER_BILINEAR);

        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig config;
        config.GlyphOffset.y -= 1;

        float dpi = sqrtf(GetWindowScaleDPI().x);
        ImFont* noto = io.Fonts->AddFontFromFileTTF("Fonts/NotoSans.ttf", 18 * dpi, &config);
        io.FontDefault = noto;

        rlImGuiReloadFonts(); // required
    }

    void UnloadFonts()
    {
        UnloadFont(font);
        //UnloadFont(mathFont);
    }

    static std::vector<int> ParseGlyphFile(const std::string& path)
    {
        std::ifstream t(path);
        if (!t.is_open()) {
            throw std::runtime_error((std::string("Couldn't open ") + path).c_str());
        }

        std::stringstream buffer;
        buffer << t.rdbuf();

        std::vector<std::string> glyphsStr = split(buffer.str(), ", ");
        std::vector<int> glyphs = std::vector<int>();
        for (const auto& item: glyphsStr) {
            glyphs.push_back(std::stoi(item));
        }

        return glyphs;
    }

    std::u16string NumToSubscript(int n)
    {
        std::u16string s;
        do {
            int lastDigit = n % 10;
            n /= 10;

            s = ToSubscript(0x30 | lastDigit) + s;
        } while (n != 0);

        return s;
    }

    char16_t ToSubscript(char16_t c)
    {
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
                throw std::runtime_error("No subscript character found!");
        }

        return 0x20 | a;
    }
}
