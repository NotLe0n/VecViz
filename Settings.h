#ifndef VECVIZ_SETTINGS_H
#define VECVIZ_SETTINGS_H

#include "Singleton.h"
#include <string>

class Settings : public Singleton<Settings> {
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
    friend class Singleton<Settings>;

public:
    static Settings& GetSettings() {
        return GetInstance();
    }

    [[nodiscard]] std::string GetDecimalPrecisionStr() const;
};

#endif //VECVIZ_SETTINGS_H
