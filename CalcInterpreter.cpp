#include "CalcInterpreter.h"
#include "utils.h"
#include <string>
#include <queue>
#include <cassert>
#include <cmath>
#include "raymath.h"
#include "sstream"

namespace CalcInterpreter
{
    std::variant<Vector3, float> Interpret(char text[], std::vector<DrawVector> vectors)
    {
        auto isFloat = [](const CalcVal& v) { return v.index() == 1; };
        auto isVec = [](const CalcVal& v) { return v.index() == 0; };
        auto getVec = [](const CalcVal& v) { return std::get<Vector3>(v); };
        auto getFloat = [](const CalcVal& v) { return std::get<float>(v); };

        std::queue<MathToken> tokens = Scan(text);
        Sort(tokens);

        std::stack<CalcVal> nums;
        while (!tokens.empty()) {
            MathToken token = tokens.front();

            switch (token.type) {
                case NUMBER: {
                    float n = std::strtof(token.lexeme.c_str(), nullptr);
                    nums.emplace(n);
                    break;
                }
                case VEC: {
                    if (nums.empty()) {
                        nums.emplace(Vector3Zero());
                        break;
                    }
                    float z = getFloat(pop(nums));
                    if (nums.empty()) {
                        nums.emplace(Vector3{z, 0, 0});
                        break;
                    }

                    float y = getFloat(pop(nums));
                    if (nums.empty()) {
                        nums.emplace(Vector3{y, z, 0});
                        break;
                    }

                    float x = getFloat(pop(nums));
                    nums.emplace(Vector3{x, y, z});
                    break;
                }
                case VAR: {
                    auto sn = token.lexeme.substr(1);
                    auto n = (unsigned int)std::atoi(sn.c_str());
                    if (n < vectors.size()) {
                        nums.emplace(vectors[n].vector);
                    }
                    break;
                }
                case NEG: {
                    std::variant<Vector3, float> v = pop(nums);
                    if (isFloat(v)) {
                        nums.emplace(-getFloat(v));
                    } else {
                        nums.emplace(Vector3Negate(getVec(v)));
                    }
                    break;
                }
                case PLUS: {
                    std::variant<Vector3, float> a = pop(nums);
                    std::variant<Vector3, float> b = pop(nums);
                    if (isFloat(a) && isFloat(b)) {
                        nums.emplace(getFloat(b) + getFloat(a));
                    }
                    else if (isVec(a) && isVec(b)) {
                        nums.emplace(Vector3Add(getVec(b), getVec(a)));
                    }
                    break;
                }
                case MINUS: {
                    std::variant<Vector3, float> a = pop(nums);
                    std::variant<Vector3, float> b = pop(nums);
                    if (isFloat(a) && isFloat(b)) {
                        nums.emplace(getFloat(b) - getFloat(a));
                    }
                    else if (isVec(a) && isVec(b)) {
                        nums.emplace(Vector3Subtract(getVec(b), getVec(a)));
                    }
                    break;
                }
                case MULT: {
                    std::variant<Vector3, float> a = pop(nums);
                    std::variant<Vector3, float> b = pop(nums);
                    if (isFloat(a) && isFloat(b)) {
                        nums.emplace(getFloat(b) * getFloat(a));
                    }
                    else if (isVec(a) && isVec(b)){
                        nums.emplace(Vector3Multiply(getVec(b), getVec(a)));
                    }
                    else {
                        if (isVec(a)) {
                            nums.emplace(Vector3Scale(getVec(a), getFloat(b)));
                        }
                        else {
                            nums.emplace(Vector3Scale(getVec(b), getFloat(a)));
                        }
                    }
                    break;
                }
                case DIV: {
                    std::variant<Vector3, float> a = pop(nums);
                    std::variant<Vector3, float> b = pop(nums);
                    if (isFloat(a) && isFloat(b)) {
                        nums.emplace(getFloat(b) / getFloat(a));
                    }
                    else if (isVec(a) && isVec(b)){
                        nums.emplace(Vector3Divide(getVec(b), getVec(a)));
                    }
                    else {
                        if (isVec(a)) {
                            nums.emplace(Vector3Scale(getVec(a), 1/getFloat(b)));
                        }
                        else {
                            nums.emplace(Vector3Scale(getVec(b), 1/getFloat(a)));
                        }
                    }
                    break;
                }
                case EXP: {
                    std::variant<Vector3, float> a = pop(nums);
                    std::variant<Vector3, float> b = pop(nums);
                    if (isFloat(a) && isFloat(b)) {
                        nums.emplace(pow(getFloat(b), getFloat(a)));
                    }
                    break;
                }
                case DOT: {
                    std::variant<Vector3, float> a = pop(nums);
                    std::variant<Vector3, float> b = pop(nums);
                    if (isVec(a) && isVec(b)) {
                        Vector3 vb = getVec(b);
                        Vector3 va = getVec(a);
                        if (va.z == 0 && vb.z == 0) {
                            nums.emplace(vb.x*va.x + vb.y*va.y);
                        }
                        else {
                            nums.emplace(Vector3DotProduct(getVec(b), getVec(a)));
                        }
                    }
                    break;
                }
                case CROSS: {
                    std::variant<Vector3, float> a = pop(nums);
                    std::variant<Vector3, float> b = pop(nums);
                    if (isVec(a) && isVec(b)) {
                        Vector3 vb = getVec(b);
                        Vector3 va = getVec(a);

                        if (va.z == 0 && vb.z == 0) {
                            nums.emplace(vb.x * va.y - vb.y * va.x);
                        }
                        else {
                            nums.emplace(Vector3CrossProduct(vb, va));
                        }
                    }
                    break;
                }
                default:
                    break;
            }

            tokens.pop();
        }
        if (nums.empty()) {
            return 0.0f;
        }

        return nums.top();
    }

    void Sort(std::queue<MathToken>& tokens)
    {
        std::queue<MathToken> output;
        std::stack<MathToken> operatorStack;

        size_t tokenCount = tokens.size();
        for (size_t i = 0; i < tokenCount; ++i) {
            MathToken token = tokens.front();
            if (!tokens.empty()) tokens.pop();

            if (token.type == NUMBER) {
                output.push(token);
            }
            else if (token.type == PAREN) {
                if (token.lexeme == "(" || token.lexeme == "{") {
                    operatorStack.push(token);
                }
                else if (token.lexeme == ")") {
                    for (MathToken o = operatorStack.top(); o.lexeme != "("; o = operatorStack.top()) {
                        assert(!operatorStack.empty() && "operator Stack was empty!");
                        operatorStack.pop();
                        output.push(o);
                    }
                    assert(operatorStack.top().lexeme == "(" && "mismatched parentheses");
                    operatorStack.pop();
                }
                else if (token.lexeme == "}") {
                    for (MathToken o = operatorStack.top(); o.lexeme != "{"; o = operatorStack.top()) {
                        assert(!operatorStack.empty() && "operator Stack was empty!");
                        operatorStack.pop();
                        output.push(o);
                    }
                    assert(operatorStack.top().lexeme == "{" && "mismatched parentheses");
                    operatorStack.pop();
                    output.push({VEC});
                }
            }
            else {
                if (!operatorStack.empty()) {
                    MathToken o2 = operatorStack.top();
                    while (!operatorStack.empty() && (o2 = operatorStack.top()).type != PAREN
                            && (o2.prec > token.prec || o2.prec == token.prec && token.assoc == LEFT))
                    {
                        output.push(o2);
                        if (!operatorStack.empty()) operatorStack.pop();
                    }
                }
                operatorStack.push(token);
            }
        }

        while (!operatorStack.empty()) {
            output.push(operatorStack.top());
            operatorStack.pop();
        }

        tokens = output;
    }

    std::queue<MathToken> Scan(char text[])
    {
        std::queue<MathToken> tokens;
        MathToken prevToken;
        int current = 0;

        while (text[current] != '\0') {
            char c = text[current++];
            switch (c) {
                case '+':
                    tokens.push({PLUS, "+", 2, LEFT});
                    break;
                case '-':
                    if (current == 1 || prevToken.lexeme == "(" || prevToken.lexeme == "{" || prevToken.assoc != NONE || prevToken.type == MINUS) {
                        tokens.push({NEG, "-", 4, RIGHT});
                    } else {
                        tokens.push({MINUS, "-", 2, LEFT});
                    }
                    break;
                case '*':
                    tokens.push({MULT, "*", 3, LEFT});
                    break;
                case '/':
                    tokens.push({DIV, "/", 3, LEFT});
                    break;
                case '.':
                    tokens.push({DOT, ".", 3, LEFT});
                    break;
                case 'x':
                    tokens.push({CROSS, "x", 3, LEFT});
                    break;
                case '^':
                    tokens.push({EXP, "^", 4, RIGHT});
                    break;
                case '(':
                    tokens.push({PAREN, "(", 5});
                    break;
                case ')':
                    tokens.push({PAREN, ")", 5});
                    break;
                case '{':
                    tokens.push({PAREN, "{", 5});
                    break;
                case '}':
                    tokens.push({PAREN, "}", 5});
                    break;
                case ' ':
                    // ignore
                    break;
                case 'v': {
                    std::string n("v");
                    while (IsDigit(text[current])) {
                        n += text[current++];
                    }
                    tokens.push({VAR, n, 6});
                }
                default:
                    if (IsDigit(c)) {
                        std::string num(1, c);
                        while (IsDigit(text[current]) || text[current] == '.') {
                            if (text[current] == '.' && !IsDigit(text[current+1])){
                                current++;
                                break;
                            }
                            num += text[current++];
                        }
                        tokens.push({NUMBER, num});
                    }
                    break;
            }

            prevToken = tokens.back();
        }

        return tokens;
    }

    std::string calVal_to_string(CalcVal calVal) {
        std::stringstream stringBuilder;
        stringBuilder.precision(6);
        if (calVal.index() == 0) {
            Vector3 sv = std::get<Vector3>(calVal);
            stringBuilder << "{ " << sv.x << ", " << sv.y << ", " << sv.z << " }";
        }
        else {
            stringBuilder << std::get<float>(calVal);
        }
        return stringBuilder.str();
    }
}
