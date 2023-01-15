#include "VectorSpace.h"

void VectorSpace::Render() {
    DrawTexture(rt.texture, 300, 0, WHITE); // draw render texture
    Update(); // update camera
    Draw(); // draw to render texture (and text outside of it)
}
