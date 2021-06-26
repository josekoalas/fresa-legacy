//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "fmath.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#include "log.h"

using namespace Verse;

void Math::perlinNoise(Vec2 size, Vec2 offset, float freq, int levels, ui8 *noise_data, bool reset) {
    float f = freq * 0.001f;
    
    //TODO: Change to scrolling texture instead of rewrite
    
    for (ui32 y = 0; y < size.y; y++) {
        for (ui32 x = 0; x < size.x; x++) {
            if (not reset and x + offset.x + (y + offset.y)*size.x < size.x * size.y - 1) {
                noise_data[x + y*size.x] = noise_data[x + offset.x + (y + offset.y)*size.x];
                continue;
            }
            
            float n = stb_perlin_noise3((x + offset.x) * f, (y + offset.y) * f, 0.0f, 0, 0, 0);
            for (ui8 l = 1; l <= std::min(std::max(levels, 1), 16); l++) {
                float m = stb_perlin_noise3(pow(2.0f, l) * (x + offset.x) * f, pow(2.0f, l) * (y + offset.y) * f, 0.0f, 0, 0, 0);
                n += m / pow(2.0f, l);
            }
            noise_data[x + y*size.x] = (ui8)round((n + 1.0f) * 127.5f); //TODO: It maybe works differently for web, check it
        }
    }
}

bool Math::checkAABB(Rect2 &a, Rect2 &b) {
    return (*a.x < *b.x + *b.w) and
           (*a.x + *a.w > *b.x) and
           (*a.y < *b.y + *b.h) and
           (*a.y + *a.h > *b.y);
}
