#include <codecvt>
#include <locale>
#include <sstream>
#include "VectorSpace2D.h"
#include "../Drawing.h"
#include "../utils.h"
#include "../Settings.h"
#include "../FontManager.h"

Camera2D camera;

VectorSpace2D::VectorSpace2D()
{
    camera = Camera2D();
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
        camera.zoom = Clamp(camera.zoom + wheel * 2, 25, 300);
    }
}

Vector2 worldStart;
Vector2 worldEnd;
float labelFontSize;

//void DrawDebugInfo();

void VectorSpace2D::Draw()
{
    worldStart = VecToWorldSpace({0, 0}, camera);
    worldEnd = VecToWorldSpace({(float)GetScreenWidth(), (float)GetScreenHeight()}, camera);
    labelFontSize = sqrtf(camera.zoom * 3);
    if (labelFontSize > 48) {
        labelFontSize = 48;
    }

    BeginTextureMode(rt);
    {
        ClearBackground(Color{20, 20, 20, 255});
        BeginMode2D(camera);
        {
            Settings& settings = Settings::GetSettings();
            if (settings.drawGrid) {
                if (settings.drawOrigGrid){
                    DrawOrigGrid();
                }
                if (settings.drawTransformedGrid) {
                    DrawTransformedGrid();
                }
            }

            if (settings.drawAxis) {
                DrawYAxis();
                DrawXAxis();
                Drawing::DrawToOtherRt(camera, rt, textTexture, []{
                    Vector2 textSize = Drawing::MeasureText("0", labelFontSize);
                    Vector2 pos = VectorSpace2D::WorldVecToScreenSpace({0, 0}, camera);
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
    //DrawDebugInfo();
    EndTextureMode();
}


void VectorSpace2D::ApplyTransformation(Matrix transformationMatrix)
{
    this->transformationMatrix = transformationMatrix;

    BasisX.vector = {transformationMatrix.m0, transformationMatrix.m1, 0};
    BasisY.vector = {transformationMatrix.m4, transformationMatrix.m5, 0};
}

/*void DrawDebugInfo() {
    std::stringstream stringBuilder;
    stringBuilder.precision(3);
    stringBuilder <<
    "Monitor dim: [" << GetMonitorWidth(GetCurrentMonitor()) << ", " << GetMonitorHeight(GetCurrentMonitor()) << "]\n" <<
    "Monitor pysical dim: [" << GetMonitorPhysicalWidth(GetCurrentMonitor()) << ", " << GetMonitorPhysicalHeight(GetCurrentMonitor()) << "]\n" <<
    "Screen dim: [" << GetScreenWidth() << ", " << GetScreenHeight() << "]\n" <<
    "Render dim: [" << GetRenderWidth() << ", " << GetRenderHeight() << "]\n" <<
    "Window scale DPI: [" << GetWindowScaleDPI().x << ", " << GetWindowScaleDPI().y << "]\n";

    Drawing::DrawText(stringBuilder.str(), VectorSpace::drawOffset.x, 0, YELLOW, 20);
}*/

void VectorSpace2D::DrawOrigGrid()
{
    for (int i = (int)worldStart.y; i > worldEnd.y; i--) {
        DrawRay(Ray{{worldStart.x, float(i), 0}, {1, 0, 0}}, DARKGRAY);
    }
    for (int i = (int)worldEnd.x; i > worldStart.x; i--) {
        DrawRay(Ray{{float(i), worldEnd.y, 0}, {0, 1, 0}}, DARKGRAY);
    }
}

void VectorSpace2D::DrawTransformedGrid()
{
    Vector2 dirX1 = V2Transform({1, 0}, MatrixLerp(transformationMatrix, t));
    Vector2 dirX2 = V2Transform({-1, 0}, MatrixLerp(transformationMatrix, t));

    Vector2 dirY1 = V2Transform({0, 1}, MatrixLerp(transformationMatrix, t));
    Vector2 dirY2 = V2Transform({0, -1}, MatrixLerp(transformationMatrix, t));

    Color col = {164, 170, 70, 255};
    for (int y = GetScreenHeight(); y > -GetScreenHeight(); y--) {
        Vector2 pos = V2Transform({0, float(y)}, MatrixLerp(transformationMatrix, t));

        DrawRay(Ray{V2ToV3(pos), V2ToV3(dirX1)}, col);
        DrawRay(Ray{V2ToV3(pos), V2ToV3(dirX2)}, col);
    }
    for (int x = GetScreenWidth(); x > -GetScreenWidth(); x--) {
        Vector2 pos = V2Transform({float(x), 0}, MatrixLerp(transformationMatrix, t));

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
    for (int i = (int)worldStart.y; i > worldEnd.y; i--) {
        Vector2 a = {.2f, (float)i};
        Vector2 b = {-.2f, (float)i};

        // Draw Y axis ticks
        DrawLineV(a, b, WHITE);

        // Draw Y axis tick labels
        if (i == 0) continue;
        Drawing::DrawToOtherRt(camera, rt, textTexture, [&i, &b] {
            Vector2 textSize = Drawing::MeasureText(std::to_string(i), labelFontSize);
            Vector2 pos = VectorSpace2D::WorldVecToScreenSpace(b, camera);
            Drawing::DrawText(std::to_string(i), pos.x - textSize.x - 5, pos.y - textSize.y / 2, WHITE, labelFontSize);
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
    for (int i = (int)worldStart.x; i < worldEnd.x; i++) {
        Vector2 a = {(float)i, .2f};
        Vector2 b = {(float)i, -.2f};

        // Draw X axis ticks
        DrawLineV(a, b, WHITE);

        // Draw X axis tick labels
        if (i == 0) continue;
        Drawing::DrawToOtherRt(camera, rt, textTexture, [&i, &b] {
            Vector2 textSize = Drawing::MeasureText(std::to_string(i), labelFontSize);
            Vector2 pos = VectorSpace2D::WorldVecToScreenSpace(b, camera);
            Drawing::DrawText(std::to_string(i), pos.x - textSize.x / 2, pos.y, WHITE, labelFontSize);
        });
    }
}

Vector2 VectorSpace2D::VecToWorldSpace(Vector2 pos, Camera2D cam)
{
    return GetScreenToWorld2D({pos.x, GetMonitorHeight(GetCurrentMonitor()) - pos.y}, cam);
}

Vector2 VectorSpace2D::WorldVecToScreenSpace(Vector2 pos, Camera2D cam)
{
    Vector2 trans = GetWorldToScreen2D(pos, cam);
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
    Vector2 transformedPos = V2Transform({vector.vector.x, vector.vector.y}, MatrixLerp(transformationMatrix, t));

    if (settings.drawVectorPoint) {
        DrawCircleV(transformedPos, 3 / camera.zoom, color);
    }

    if (settings.drawVectorArrow) {
        Drawing::DrawArrow2D(transformedPos, {vector.origin.x, vector.origin.y}, 10 / camera.zoom, color);
    }

    if (settings.drawVectorLabel) {
        Drawing::DrawToOtherRt(camera, rt, textTexture, [&name, &vector, &transformedPos, &color, &settings] {
            Vector2 screenPos = VectorSpace2D::WorldVecToScreenSpace(transformedPos, camera);
            Vector2 labelPosition = {screenPos.x + 10.0f / camera.zoom, screenPos.y - 20.0f / camera.zoom};

            if (settings.drawVectorName) {
                std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
                std::string utf8_name = convert.to_bytes(name);
                Drawing::DrawText(utf8_name, labelPosition.x, labelPosition.y, color, labelFontSize * 2);
            }

            if (settings.drawVectorCoords) {
                std::stringstream stringBuilder;
                stringBuilder.precision(settings.decimalPrecision);
                stringBuilder << "(" << vector.X() << ", " << vector.Y() << ")";

                Drawing::DrawText(stringBuilder.str(), labelPosition.x + .3f * camera.zoom, labelPosition.y, color, labelFontSize);
            }

            if (settings.showCoordinateVector) {
                std::stringstream stringBuilder;
                stringBuilder.precision(settings.decimalPrecision);
                stringBuilder << "(" << transformedPos.x << ", " << transformedPos.y << ")";

                float yOffset = 0;
                if (settings.drawVectorCoords) {
                    yOffset += .15;
                }
                Drawing::DrawText(stringBuilder.str(), labelPosition.x + .3f * camera.zoom, labelPosition.y + yOffset * camera.zoom, color, labelFontSize);
            }
        });
    }
}
