//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#ifdef USE_OPENGL

#ifdef __EMSCRIPTEN__ //WEB

    #include <GLES3/gl3.h>

#elif __APPLE__ //MACOS

    #define GL_SILENCE_DEPRECATION

    #include <OpenGL/OpenGL.h>
    #include <OpenGL/gl3.h>

#else //OTHERS (Not tested)

    #include <SDL2/SDL_opengl.h>
    #include <SDL2/SDL_opengl_glext.h>

#endif

#include "r_dtypes.h"

#include "gui.h"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define glCheckError() glCheckError_(__FILENAME__, __LINE__)

#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504

namespace Fresa::Graphics
{
    struct AttachmentData {
        ui32 tex;
        AttachmentType type;
    };
    
    struct SubpassData {
        ui32 framebuffer;
        bool has_depth;
        std::vector<AttachmentID> framebuffer_attachments = {};
        std::vector<AttachmentID> input_attachments = {};
    };
    
    struct OpenGL {
        SDL_GLContext context;
        
        std::map<Shaders, ShaderData> shaders;
        std::map<AttachmentID, AttachmentData> attachments;
        std::map<SubpassID, SubpassData> subpasses;
        
        #ifndef DISABLE_GUI
        ImGuiContext* imgui_context;
        ImGuiIO io;
        #endif
    };

    [[maybe_unused]] static void glCheckError_(std::string file, int line) {
        GLenum e_code;
        while ((e_code = glGetError()) != GL_NO_ERROR) {
            std::string e_message;
            switch (e_code)
            {
                case GL_INVALID_ENUM:                  e_message = "INVALID_ENUM"; break;
                case GL_INVALID_VALUE:                 e_message = "INVALID_VALUE"; break;
                case GL_INVALID_OPERATION:             e_message = "INVALID_OPERATION"; break;
                case GL_STACK_OVERFLOW:                e_message = "STACK_OVERFLOW"; break;
                case GL_STACK_UNDERFLOW:               e_message = "STACK_UNDERFLOW"; break;
                case GL_OUT_OF_MEMORY:                 e_message = "OUT_OF_MEMORY"; break;
                case GL_INVALID_FRAMEBUFFER_OPERATION: e_message = "INVALID_FRAMEBUFFER_OPERATION"; break;
            }
            std::cout << "[GL_ERROR]: " << e_message << " | " << file << " (" << line << ")" << std::endl;
        }
    }
}


#endif