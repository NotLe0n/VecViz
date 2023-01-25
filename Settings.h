#ifndef VECVIZ_SETTINGS_H
#define VECVIZ_SETTINGS_H

#include "Singleton.h"
#include <string>

class Settings : public Singleton<Settings> {
public:
    // general options
    int targetFPS = 60;
    int decimalPrecision = 3;

    // vector space options
    bool drawOrigGrid = true;
    bool drawTransformedGrid = false;
    bool drawGrid = true;
    bool drawMinorGridLines = true;
    bool drawAxis = true;
    bool drawVectorCoords = true;
    bool drawVectorName = true;
    bool drawVectorLabel = true;
    bool drawVectorArrow = true;
    bool drawVectorPoint = true;
    bool drawBasisVectors = false;
    bool drawCoordinateVectorPos = true;

    // view settings
    bool showVectorsWindow = true;
    bool showTransformationsWindow = true;
    bool showOptionsWindow = false;

private:
    friend class Singleton<Settings>;

public:
    static Settings& GetSettings() {
        return GetInstance();
    }

    [[nodiscard]] std::string GetDecimalPrecisionStr() const;
};

#endif //VECVIZ_SETTINGS_H
