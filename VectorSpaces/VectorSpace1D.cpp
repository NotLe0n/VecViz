#include <sstream>
#include "VectorSpace1D.h"
#include "../Settings.h"
#include "../Drawing.h"
#include "../FontManager.h"
#include "../utils.h"

VectorSpace1D::VectorSpace1D()
{
    camera.zoom = 50;
    camera.offset = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
}

VectorSpace1D::~VectorSpace1D()
{
    UnloadRenderTexture(rt);
}

int VectorSpace1D::GetDimension()
{
    return 1;
}

void VectorSpace1D::Draw()
{
    BeginTextureMode(rt);
    {
        ClearBackground(Color{20, 20, 20, 255});
        BeginMode2D(camera);
        {
            Settings& settings = Settings::GetSettings();

            if (settings.drawAxis) {
                DrawAxis();
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

void VectorSpace1D::Update()
{
    // dragging code
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f / camera.zoom);

        // Move target by delta
        camera.target.x = camera.target.x + delta.x;
    }

    // zooming code
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        Vector2 rtMousePos = GetMousePosition();

        // Get the world point that is under the mouse
        Vector2 mouseWorldPos = GetScreenToWorld2D(rtMousePos, camera);

        // Set the offset to where the mouse is
        camera.offset.x = rtMousePos.x - drawOffset.x;

        // Set the target to match, so that the camera maps the world space point
        // under the cursor to the screen space point under the cursor at any zoom
        camera.target.x = mouseWorldPos.x - drawOffset.x / camera.zoom;

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

    worldStart = VecToWorldSpace({0, 0, 0}).x;
    worldEnd = VecToWorldSpace({(float)GetScreenWidth(), (float)GetScreenHeight(), 0}).x;
}

void VectorSpace1D::ApplyTransformation(Matrix transformationMatrix)
{
    auto mf = MatrixToFloatV(transformationMatrix);
    for (int i = 0; i < 3; ++i) {
        this->transformationMatrix[i] = mf.v[i];
        this->transformationMatrix[i] = mf.v[i+4];
        this->transformationMatrix[i] = mf.v[i+8];
    }

    BasisX.vector = {transformationMatrix.m0, 0, 0};
}

void VectorSpace1D::DrawAxis()
{
    Vector3 dir1 = {1, 0, 0};
    Vector3 dir2 = {-1, 0, 0};

    DrawRay(Ray{Vector3Zero(), dir1}, WHITE);
    DrawRay(Ray{Vector3Zero(), dir2}, WHITE);

    DrawAxisTicks();
}

void VectorSpace1D::DrawAxisTicks()
{
    int start = (int)worldStart - ((int)worldStart % step); // corrected for uneven worldStart on step = 2
    for (int x = start; x < worldEnd; x += step) {
        Vector2 a = {(float)x, 10 / camera.zoom};
        Vector2 b = {(float)x, -10 / camera.zoom};

        // Draw X axis ticks
        DrawLineV(a, b, WHITE);

        // Draw X axis tick labels
        Drawing::DrawToOtherRt(camera, rt, textTexture, [&x, &b, this] {
            Vector2 textSize = Drawing::MeasureText(std::to_string(x), labelFontSize);
            Vector3 pos = WorldVecToScreenSpace(V2ToV3(b));
            Drawing::DrawText(std::to_string(x), pos.x - textSize.x / 2, pos.y + 15, WHITE, labelFontSize);
        });
    }
}

Vector3 VectorSpace1D::VecToWorldSpace(Vector3 pos)
{
    return V2ToV3(GetScreenToWorld2D({pos.x}, camera));
}

Vector3 VectorSpace1D::WorldVecToScreenSpace(Vector3 pos)
{
    Vector2 trans = GetWorldToScreen2D({pos.x}, camera);
    return {trans.x, GetMonitorHeight(GetCurrentMonitor()) - trans.y};
}

void VectorSpace1D::DrawVectors()
{
    Settings& settings = Settings::GetSettings();
    if (settings.drawBasisVectors) {
        DrawAVector(BasisX, u"\U000000EE", GRAY, 0); // i with hat
    }

    for (int i = 0; i < vectors.size(); i++) {
        DrawAVector(vectors[i], u"\U00000475" + FontManager::NumToSubscript(i), vectors[i].GetColor(), t); // v with subscript vector index
    }
}

void VectorSpace1D::DrawAVector(DrawVector vector, const std::u16string& name, Color color, float t)
{
    Settings& settings = Settings::GetSettings();
    Vector2 transformedPos = {vector.vector.x * Lerp(1, GetTransformationMatrix().m0, t)};

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
