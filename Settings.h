#ifndef VECVIZ_SETTINGS_H
#define VECVIZ_SETTINGS_H

#include <string>

class Settings {
public:
    // options settings
    bool drawGrid = true;
    bool drawAxis = true;
    bool drawVectorCoords = true;
    bool drawVectorName = true;
    bool drawVectorLabel = true;
    bool drawVectorArrow = true;
    bool drawVectorPoint = true;
    bool drawBasisVectors = false;
    int decimalPrecision = 3;

    // view settings
    bool showVectorsWindow = true;
    bool showTransformationsWindow = true;

private:
    Settings() = default;

public:
    static Settings& GetSettings();

    Settings(Settings&&) = delete;
    Settings(Settings&) = delete;

    void operator=(const Settings&) = delete;
    void operator=(const Settings&&) = delete;

public:
    [[nodiscard]] std::string GetDecimalPrecisionStr() const;
};

#endif //VECVIZ_SETTINGS_H
