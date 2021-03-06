//project fresa, 2017-2022
//by jose pazos perez
//licensed under GPLv3 uwu

#pragma once

#include "types.h"

namespace Fresa {
    struct Config {
        static const str name;
        static const ui8 version[3];
        static const Vec2<ui32> window_size;
        static const Vec2<ui32> resolution;
        
        static const float timestep;
        static float game_speed;
        
        static str renderer_description_path;
        static bool draw_indirect;
        static ui8 multisampling;
    };
}
