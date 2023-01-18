#include "Settings.h"
#include "sstream"

Settings& Settings::GetSettings()
{
    static Settings settings;
    return settings;
}

std::string Settings::GetDecimalPrecisionStr() const
{
    std::stringstream stringBuilder;
    stringBuilder << "%." << decimalPrecision << "f";
    return stringBuilder.str();
}

