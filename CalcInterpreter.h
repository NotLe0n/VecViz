#ifndef VECVIZ_CALCINTERPRETER_H
#define VECVIZ_CALCINTERPRETER_H

#include <stack>
#include "queue"
#include <string>
#include <variant>
#include "raylib.h"
#include "VectorSpaces/DrawVector.h"

namespace CalcInterpreter
{
    using CalcVal = std::variant<Vector3, float>;

    enum Associativity {
        NONE, LEFT, RIGHT
    };

    enum MathTokenType {
        PLUS, MINUS, MULT, DIV, EXP, PAREN, NUMBER, FUNC, VEC, NEG, VAR, DOT, CROSS,
    };

    struct MathToken {
        MathTokenType type;
        std::string lexeme;
        int prec; // precedence
        Associativity assoc; // associativity
    };

    std::queue<MathToken> Scan(char text[]); // tokenizes the string
    void Sort(std::queue<MathToken>& tokens); // sorts the tokens in RPN
    CalcVal Interpret(char text[], std::vector<DrawVector> vectors);

    std::string calVal_to_string(CalcVal val);
}
#endif //VECVIZ_CALCINTERPRETER_H
