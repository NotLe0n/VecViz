#include <sstream>
#include "VectorSpace2D.h"
#include "../Settings.h"
#include "../Drawing.h"
#include "../FontManager.h"
#include "../utils.h"

VectorSpace2D::VectorSpace2D()
{
    camera.zoom = 50.0;
    camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
}

VectorSpace2D::~VectorSpace2D()
{
    UnloadRenderTexture(rt);
    UnloadRenderTexture(textTexture);
}

int VectorSpace2D::GetDimension()
{
    return 2;
}

void VectorSpace2D::Update()
{
    // dragging code
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f / camera.zoom);

        // Move target by delta
        // invert Y because RenderTarget is flipped
        camera.target = Vector2Add(camera.target, {delta.x, -delta.y});
    }

    // zooming code
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        Vector2 rtMousePos = GetMousePosition();
        rtMousePos.y = GetMonitorHeight(GetCurrentMonitor()) - rtMousePos.y - drawOffset.y;

        // Get the world point that is under the mouse
        Vector2 mouseWorldPos = GetScreenToWorld2D(rtMousePos, camera);

        // Set the offset to where the mouse is
        camera.offset = Vector2Subtract(rtMousePos, {drawOffset.x, -drawOffset.y});

        // Set the target to match, so that the camera maps the world space point
        // under the cursor to the screen space point under the cursor at any zoom
        camera.target = Vector2Subtract(mouseWorldPos, Vector2Scale({drawOffset.x, -drawOffset.y}, 1 / camera.zoom));

        // Zoom increment
        camera.zoom = Clamp(camera.zoom + wheel / (step / 2.0f), 5, 300);
    }

    if (camera.zoom < 15) {
        step = 8;
    }
    else if (camera.zoom < 35) {
        step = 2;
    }
    else {
        step = 1;
    }

    Vector3 worldStartV3 = VecToWorldSpace({0, 0, 0});
    Vector3 worldEndV3 = VecToWorldSpace({(float)GetScreenWidth(), (float)GetScreenHeight(), 0});
    worldStart = {worldStartV3.x, worldStartV3.y};
    worldEnd = {worldEndV3.x, worldEndV3.y};
}

void VectorSpace2D::Draw()
{
    BeginTextureMode(rt);
    {
        ClearBackground(Color{20, 20, 20, 255});

        BeginMode2D(camera);
        {
            Settings& settings = Settings::GetSettings();
            if (settings.drawGrid) {
                if (settings.drawOrigGrid) {
                    DrawOrigGrid();
                }
                if (settings.drawTransformedGrid) {
                    DrawTransformedGrid();
                }
            }

            if (settings.drawAxis) {
                DrawYAxis();
                DrawXAxis();

                // Origin label
                Drawing::DrawToOtherRt(camera, rt, textTexture, [this]{
                    Vector2 textSize = Drawing::MeasureText("0", labelFontSize);
                    Vector3 pos = WorldVecToScreenSpace({0, 0, 0});
                    Drawing::DrawText("0", pos.x - textSize.x - 5, pos.y + 5, WHITE, labelFontSize);
                });
            }

            DrawVectors();
        }
        EndMode2D();

        DrawTexture(textTexture.texture, 0, 0, WHITE);
    }
    EndTextureMode();

    BeginTextureMode(textTexture);
    ClearBackground({0, 0, 0, 0});
    EndTextureMode();
}

void VectorSpace2D::ApplyTransformation(Matrix transformationMatrix)
{
    auto mf = MatrixToFloatV(transformationMatrix);
    for (int i = 0; i < 3; ++i) {
        this->transformationMatrix[i] = mf.v[i];
        this->transformationMatrix[i] = mf.v[i+4];
        this->transformationMatrix[i] = mf.v[i+8];
    }

    BasisX.vector = {transformationMatrix.m0, transformationMatrix.m1, 0};
    BasisY.vector = {transformationMatrix.m4, transformationMatrix.m5, 0};
}

void VectorSpace2D::DrawOrigGrid()
{
    Vector3 unitX = {1, 0, 0};
    Vector3 unitY = {0, 1, 0};

    Settings& settings = Settings::GetSettings();
    if (settings.drawMinorGridLines) {
        Color minorLineColor = {33, 33, 33, 255};
        for (int i = 0; i <= 3; ++i) {
            for (int x = (int)worldStart.x - ((int)worldStart.x % step); x < worldEnd.x; x += step) {
                DrawRay(Ray{{x + 0.25f * i * step, worldEnd.y, 0}, unitY}, minorLineColor);
            }
            for (int y = (int)worldStart.y - ((int)worldStart.y % step); y > worldEnd.y; y -= step) {
                DrawRay(Ray{{worldStart.x, y + 0.25f * i * step, 0}, unitX}, minorLineColor);
            }
        }
    }

    // Draw major grid lines
    Color majorLineColor = DARKGRAY;
    for (int y = (int)worldStart.y - ((int)worldStart.y % step); y > worldEnd.y; y -= step) {
        DrawRay(Ray{{worldStart.x, (float)y, 0}, unitX}, majorLineColor);
    }
    for (int x = (int)worldStart.x - ((int)worldStart.x % step); x < worldEnd.x; x += step) {
        DrawRay(Ray{{(float)x, worldEnd.y, 0}, unitY}, majorLineColor);
    }
}

void VectorSpace2D::DrawTransformedGrid()
{
    Vector2 dirX1 = Vector2Transform({1, 0}, MatrixLerp(GetTransformationMatrix(), t));
    Vector2 dirX2 = Vector2Transform({-1, 0}, MatrixLerp(GetTransformationMatrix(), t));

    Vector2 dirY1 = Vector2Transform({0, 1}, MatrixLerp(GetTransformationMatrix(), t));
    Vector2 dirY2 = Vector2Transform({0, -1}, MatrixLerp(GetTransformationMatrix(), t));

    Color col = {96, 125, 139, 255};
    for (int y = GetScreenHeight() - (GetScreenHeight() % step); y > -GetScreenHeight(); y -= step) {
        Vector2 pos = Vector2Transform({0, float(y)}, MatrixLerp(GetTransformationMatrix(), t));

        DrawRay(Ray{V2ToV3(pos), V2ToV3(dirX1)}, col);
        DrawRay(Ray{V2ToV3(pos), V2ToV3(dirX2)}, col);
    }
    for (int x = GetScreenWidth() - (GetScreenWidth() % step); x > -GetScreenWidth(); x -= step) {
        Vector2 pos = Vector2Transform({float(x), 0}, MatrixLerp(GetTransformationMatrix(), t));

        DrawRay(Ray{V2ToV3(pos), V2ToV3(dirY1)}, col);
        DrawRay(Ray{V2ToV3(pos), V2ToV3(dirY2)}, col);
    }
}

void VectorSpace2D::DrawYAxis()
{
    Vector3 dir1 = {0, 1, 0};
    Vector3 dir2 = {0, -1, 0};

    DrawRay(Ray{Vector3Zero(), dir1}, WHITE);
    DrawRay(Ray{Vector3Zero(), dir2}, WHITE);

    DrawYAxisTicks();
}

void VectorSpace2D::DrawYAxisTicks()
{
    int start = (int)worldStart.y - ((int)worldStart.y % step); // corrected for uneven worldStart on step = 2
    for (int y = start; y > worldEnd.y; y -= step) {
        Vector2 a = {.2f, (float)y};
        Vector2 b = {-.2f, (float)y};

        // Draw Y axis ticks
        DrawLineV(a, b, WHITE);

        // Draw Y axis tick labels
        if (y == 0) continue;
        Drawing::DrawToOtherRt(camera, rt, textTexture, [&y, &b, this] {
            Vector2 textSize = Drawing::MeasureText(std::to_string(y), labelFontSize);
            Vector3 pos = WorldVecToScreenSpace(V2ToV3(b));
            Drawing::DrawText(std::to_string(y), pos.x - textSize.x - 5, pos.y - textSize.y / 2, WHITE, labelFontSize);
        });
    }
}

void VectorSpace2D::DrawXAxis()
{
    Vector3 dir1 = {1, 0, 0};
    Vector3 dir2 = {-1, 0, 0};

    DrawRay(Ray{Vector3Zero(), dir1}, WHITE);
    DrawRay(Ray{Vector3Zero(), dir2}, WHITE);

    DrawXAxisTicks();
}

void VectorSpace2D::DrawXAxisTicks()
{
    int start = (int)worldStart.x - ((int)worldStart.x % step); // corrected for uneven worldStart on step = 2
    for (int x = start; x < worldEnd.x; x += step) {
        Vector2 a = {(float)x, .2f};
        Vector2 b = {(float)x, -.2f};

        // Draw X axis ticks
        DrawLineV(a, b, WHITE);

        // Draw X axis tick labels
        if (x == 0) continue;
        Drawing::DrawToOtherRt(camera, rt, textTexture, [&x, &b, this] {
            Vector2 textSize = Drawing::MeasureText(std::to_string(x), labelFontSize);
            Vector3 pos = WorldVecToScreenSpace(V2ToV3(b));
            Drawing::DrawText(std::to_string(x), pos.x - textSize.x / 2, pos.y, WHITE, labelFontSize);
        });
    }
}

Vector3 VectorSpace2D::VecToWorldSpace(Vector3 pos)
{
    return V2ToV3(GetScreenToWorld2D({pos.x, GetMonitorHeight(GetCurrentMonitor()) - pos.y}, camera));
}

Vector3 VectorSpace2D::WorldVecToScreenSpace(Vector3 pos)
{
    Vector2 trans = GetWorldToScreen2D({pos.x, pos.y}, camera);
    return {trans.x, GetMonitorHeight(GetCurrentMonitor()) - trans.y};
}

void VectorSpace2D::DrawVectors()
{
    Settings& settings = Settings::GetSettings();
    if (settings.drawBasisVectors) {
        DrawAVector(BasisX, u"\U000000EE", GRAY, 0); // i with hat
        DrawAVector(BasisY, u"\U00000135", GRAY, 0); // j with hat
    }

    for (int i = 0; i < vectors.size(); i++) {
        DrawAVector(vectors[i], u"\U00000475" + FontManager::NumToSubscript(i), vectors[i].GetColor(), t); // v with subscript vector index
    }
}

void VectorSpace2D::DrawAVector(DrawVector vector, const std::u16string& name, Color color, float t)
{
    Settings& settings = Settings::GetSettings();
    Vector2 transformedPos = Vector2Transform({vector.vector.x, vector.vector.y}, MatrixLerp(GetTransformationMatrix(), t));

    if (settings.drawVectorPoint) {
        DrawCircleV(transformedPos, 3 / camera.zoom, color);
    }

    if (settings.drawVectorArrow) {
        Drawing::DrawArrow2D(transformedPos, {vector.origin.x, vector.origin.y}, 10 / camera.zoom, color);
    }

    if (settings.drawVectorLabel) {
        Drawing::DrawToOtherRt(camera, rt, textTexture, [&name, &vector, &transformedPos, &color, &settings, this] {
            Vector3 screenPos = WorldVecToScreenSpace(V2ToV3(transformedPos));
            Vector2 labelPosition = {screenPos.x + 10.0f / camera.zoom, screenPos.y - 20.0f / camera.zoom};

            if (settings.drawVectorName) {
                Drawing::DrawText(UTF16ToUTF8(name), labelPosition.x, labelPosition.y, color, labelFontSize * 2);
            }

            if (settings.drawVectorCoords) {
                std::stringstream stringBuilder;
                stringBuilder.precision(settings.decimalPrecision);
                stringBuilder << "(" << vector.X() << ", " << vector.Y() << ")";

                Drawing::DrawText(stringBuilder.str(), labelPosition.x + labelFontSize*2, labelPosition.y, color, 14);
            }

            if (settings.drawCoordinateVectorPos) {
                std::stringstream stringBuilder;
                stringBuilder.precision(settings.decimalPrecision);
                stringBuilder << "(" << transformedPos.x << ", " << transformedPos.y << ")";

                float yOffset = settings.drawVectorCoords ? 14 : 0;
                Drawing::DrawText(stringBuilder.str(), labelPosition.x + labelFontSize*2, labelPosition.y + yOffset, color, 14);
            }
        });
    }
}
