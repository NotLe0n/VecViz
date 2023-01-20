#include "Settings.h"
#include "sstream"

std::string Settings::GetDecimalPrecisionStr() const
{
    std::stringstream stringBuilder;
    stringBuilder << "%." << decimalPrecision << "f";
    return stringBuilder.str();
}

