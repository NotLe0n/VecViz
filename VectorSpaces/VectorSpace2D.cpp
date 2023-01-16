#include <locale>
#include "VectorSpace2D.h"
#include "../Drawing.h"
#include "../UI/UIWindow.h"
#include "codecvt"
//#include "format"
#include "../utils.h"

Camera2D camera;

VectorSpace2D::VectorSpace2D() {
    camera = Camera2D();
    camera.zoom = 50.0;
    camera.offset = {(GetScreenWidth() - 300) / 2.0f , GetScreenHeight() / 2.0f};
}

void VectorSpace2D::Update() {
    // dragging code
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f / camera.zoom);

        camera.target = Vector2Add(camera.target, {delta.x, -delta.y}); // invert Y because RenderTarget is flipped
    }

    // zooming code
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        Vector2 rtMousePos = GetMousePosition();
        rtMousePos.y = (GetRealWindowHeight()) - rtMousePos.y;

        // Get the world point that is under the mouse
        Vector2 mouseWorldPos = GetScreenToWorld2D(rtMousePos, camera);

        // Set the offset to where the mouse is
        camera.offset = Vector2Subtract(rtMousePos, {drawOffset, 0});

        // Set the target to match, so that the camera maps the world space point
        // under the cursor to the screen space point under the cursor at any zoom
        camera.target = Vector2Subtract(mouseWorldPos, {drawOffset / camera.zoom, 0});

        // Zoom increment
        camera.zoom = Clamp(camera.zoom + wheel * 2, 25, 300);
    }
}

Vector2 worldStart;
Vector2 worldEnd;
float labelFontSize;

void DrawDebugInfo();

void VectorSpace2D::Draw() {
    BasisX.vector = {1, 0, 0};
    BasisY.vector = {0, 1, 0};

    ApplyTransformation(Matrix{
            2, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
    });

    worldStart = VecToWorldSpace({drawOffset, 0}, camera);
    worldEnd = VecToWorldSpace({(float)GetScreenWidth(), (float)GetScreenHeight()}, camera);
    labelFontSize = sqrtf(camera.zoom * 3);
    if (labelFontSize > 48) {
        labelFontSize = 48;
    }

    BeginTextureMode(rt);
    {
        ClearBackground(UIWindow::GetWindow().GetBackgroundColor());
        BeginMode2D(camera);
        {
            //DrawRectangle(0, 0, 1, 1, RED);
            DrawOrigGrid();
            DrawYAxis();
            DrawXAxis();

            DrawVectors();
        }
        EndMode2D();

        DrawTexture(textTexture.texture, -drawOffset, 0, WHITE);
    }
    EndTextureMode();

    BeginTextureMode(textTexture);
    ClearBackground({0, 0, 0, 0});
   // DrawDebugInfo();
    EndTextureMode();
}

void VectorSpace2D::AddVector(const DrawVector& vector) {
    this->vectors.push_back(vector);
}

void VectorSpace2D::ApplyTransformation(Matrix transformationMatrix) {
    BasisX.vector = {
        BasisX.X() * transformationMatrix.m0 + BasisX.Y() * transformationMatrix.m4,
        BasisX.X() * transformationMatrix.m1 + BasisX.Y() * transformationMatrix.m5, 0
    };
    BasisY.vector = {
        BasisY.X() * transformationMatrix.m0 + BasisY.Y() * transformationMatrix.m4,
        BasisY.X() * transformationMatrix.m1 + BasisY.Y() * transformationMatrix.m5, 0
    };
}

/*void DrawDebugInfo() {
    Drawing::DrawText(
            std::format("Monitor dim: [{}, {}]\nMonitor pysical dim: [{}, {}]\nScreen dim: [{}, {}]\nRender dim: [{}, {}]\nWindow scale DPI: [{}, {}]\ndrawOffset: {}",
                        GetMonitorWidth(GetCurrentMonitor()),
                        GetMonitorHeight(GetCurrentMonitor()),
                        GetMonitorPhysicalWidth(GetCurrentMonitor()),
                        GetMonitorPhysicalHeight(GetCurrentMonitor()),
                        GetScreenWidth(),
                        GetScreenHeight(),
                        GetRenderWidth(),
                        GetRenderHeight(),
                        GetWindowScaleDPI().x,
                        GetWindowScaleDPI().y,
                        VectorSpace::drawOffset),
                        VectorSpace::drawOffset, 0, YELLOW, 20);
}*/

void VectorSpace2D::DrawOrigGrid() {
    for (int i = (int)worldStart.y; i > worldEnd.y; i--) {
        DrawRay(Ray{{worldStart.x, float(i), 0}, {1, 0, 0}}, DARKGRAY);
    }
    for (int i = (int)worldEnd.x; i > worldStart.x; i--) {
        DrawRay(Ray{{float(i), worldEnd.y, 0}, {0, 1, 0}}, DARKGRAY);
    }
}

void VectorSpace2D::DrawYAxis() {
    Vector3 dir1 = {0, 1, 0};
    Vector3 dir2 = {0, -1, 0};

    DrawRay(Ray{Vector3Zero(), dir1}, WHITE);
    DrawRay(Ray{Vector3Zero(), dir2}, WHITE);

    DrawYAxisTicks();
}

void VectorSpace2D::DrawYAxisTicks() {
    for (int i = (int)worldStart.y; i > worldEnd.y; i--) {
        Vector2 a = {10 / camera.zoom, (float)i};
        Vector2 b = {-10 / camera.zoom, (float)i};

        // Draw Y axis ticks
        DrawLineV(a, b, WHITE);

        // Draw X axis tick labels
        Drawing::DrawToOtherRt(camera, rt, textTexture, [&i, &b] {
            Vector2 pos = VectorSpace2D::WorldVecToScreenSpace(b, camera);
            Drawing::DrawText(std::to_string(i), pos, WHITE, labelFontSize);
        });
    }
}

void VectorSpace2D::DrawXAxis() {
    Vector3 dir1 = {1, 0, 0};
    Vector3 dir2 = {-1, 0, 0};

    DrawRay(Ray{Vector3Zero(), dir1}, WHITE);
    DrawRay(Ray{Vector3Zero(), dir2}, WHITE);

    DrawXAxisTicks();
}

void VectorSpace2D::DrawXAxisTicks() {
    for (int i = (int)worldStart.x; i < worldEnd.x; i++) {
        Vector2 a = {(float)i, 10 / camera.zoom};
        Vector2 b = {(float)i, -10 / camera.zoom};

        // Draw X axis ticks
        DrawLineV(a, b, WHITE);

        // Draw X axis tick labels
        if (i == 0) continue;
        Drawing::DrawToOtherRt(camera, rt, textTexture, [&i, &b] {
            Vector2 pos = VectorSpace2D::WorldVecToScreenSpace(b, camera);
            Drawing::DrawText(std::to_string(i), pos, WHITE, labelFontSize);
        });
    }
}

Vector2 VectorSpace2D::VecToWorldSpace(Vector2 pos, Camera2D cam) {
    return GetScreenToWorld2D({pos.x - drawOffset, GetRealWindowHeight() - pos.y}, cam);
}
Vector2 VectorSpace2D::WorldVecToScreenSpace(Vector2 pos, Camera2D cam) {
    Vector2 trans = GetWorldToScreen2D(pos, cam);
    return {trans.x + drawOffset, GetRealWindowHeight() - trans.y};
}

void VectorSpace2D::DrawVectors() {
    DrawAVector(BasisX, u"\U000000EE", GRAY); // i with hat
    DrawAVector(BasisY, u"\U00000135", GRAY); // j with hat

    for (int i = 0; i < vectors.size(); i++) {
        DrawAVector(vectors[i], u"\U0001D463" + FontManager::NumToSubscript(i), vectors[i].color); // v with subscript vector index
    }
}

void VectorSpace2D::DrawAVector(DrawVector vector, const std::u16string& name, Color color) {
    // draw vector point
    DrawCircleV({vector.vector.x, vector.vector.y}, 3 / camera.zoom, color);

    // draw vector arrow
    Drawing::DrawArrow2D({vector.vector.x, vector.vector.y}, {vector.origin.x, vector.origin.y}, 10 / camera.zoom, color);

    Drawing::DrawToOtherRt(camera, rt, textTexture, [&name, &vector, &color] {
        Vector2 screenPos = VectorSpace2D::WorldVecToScreenSpace({vector.vector.x, vector.vector.y}, camera);
        Vector2 labelPosition = {screenPos.x + .1f * camera.zoom, screenPos.y - .2f * camera.zoom};

        // Draw vector name
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        std::string utf8_name = convert.to_bytes(name);
        Drawing::DrawMathText(utf8_name, labelPosition.x, labelPosition.y, color, labelFontSize * 2);

        // Draw position label
        Drawing::DrawText("("+ std::to_string(vector.X()) + ", " + std::to_string(vector.Y()) + ")", labelPosition.x + 2, labelPosition.y + .35f * camera.zoom, color, labelFontSize);
    });
}