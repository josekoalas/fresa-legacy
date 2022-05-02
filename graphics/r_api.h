//project fresa, 2017-2022
//by jose pazos perez
//licensed under GPLv3 uwu

#pragma once

#include "r_opengl.h"
#include "r_vulkan.h"

#include "r_shaders.h"
#include "r_buffers.h"

#include "events.h"
#include "bidirectional_map.h"

#include <set>

#define MAX_INDIRECT_COMMANDS 64

namespace Fresa::Graphics
{
    using GraphicsAPI = IF_VULKAN(Vulkan) IF_OPENGL(OpenGL);
    inline GraphicsAPI api;
}

//---API---
//      This coordinates the different rendering APIs. Here are defined the common functions that are later expanded in the respective source files
//      Right now it has full support for OpenGL and Vulkan.

namespace Fresa::Graphics
{
    //: TODO: REFACTOR
    
    //---API---
    void configureAPI();
    void createAPI();

    //---Drawing---
    TextureID registerTexture(Vec2<ui16> size, Channels ch, ui8* pixels);
    
    struct DrawDescription {
        TextureID texture;
        DrawUniformID uniform;
        MeshID mesh;
        InstancedBufferID instance = no_instance;
        IndirectBufferID indirect_buffer = no_indirect_buffer;
        ui32 indirect_offset = 0;
    };
    
    //: This is a hierarchical map for rendering
    //  - Instanced rendedring
    //      1: Global uniforms                          DrawUniformData
    //      2: Vertex buffer (geometry)                 GeometryBufferData
    //      3: Per instance buffer                      InstancedBufferID
    //      .: Textures not supported yet
    
    using DrawIQueueInstance = std::map<InstancedBufferID, DrawDescription*>;
    using DrawIQueueMesh = std::map<MeshID, DrawIQueueInstance>;
    using DrawIQueueUniform = std::map<DrawUniformID, DrawIQueueMesh>;
    using DrawIQueue = std::map<ShaderID, DrawIQueueUniform>;
    
    inline DrawIQueue draw_queue_instanced;
    
    inline std::vector<DrawDescription*> draw_descriptions;
    
    //---Indirect Drawing---
    IndirectBufferID registerIndirectCommandBuffer();
    void addIndirectDrawCommand(DrawDescription &description);
    void removeIndirectDrawCommand(DrawDescription &description);
    
    //---Render passes and attachments---
    void processRendererDescription();
    
    RenderPassID registerRenderPass(std::vector<SubpassID> subpasses);
    
    SubpassID registerSubpass(std::vector<AttachmentID> attachment_list, std::vector<AttachmentID> external_attachment_list = {});
    
    AttachmentID registerAttachment(AttachmentType type, Vec2<ui16> size);
    void recreateAttachments();
    bool hasMultisampling(AttachmentID attachment, bool check_samples = true);
    
    //---Mappings---
    namespace Map {
        inline map_AvB_BA<RenderPassID, SubpassID> renderpass_subpass;
        inline map_AvB_BvA<SubpassID, AttachmentID> subpass_attachment;
        inline map_AvB_BA<SubpassID, ShaderID> subpass_shader;
        
        inline auto renderpass_attachment = map_chain(renderpass_subpass, subpass_attachment);
        inline auto renderpass_shader = map_chain(renderpass_subpass, subpass_shader);
    };

    //---Shaders---
    void updateDrawDescriptorSets(const DrawDescription& draw, ShaderID shader);
    
    //---Other---
    void resize();

    void render();
    void present();

    void clean();

    //---Attributes---
    template <typename V, std::enable_if_t<Reflection::is_reflectable<V>, bool> = true>
    std::vector<VertexAttributeDescription> getAttributeDescriptions(ui32 binding = 0, ui32 previous_location = 0) {
        std::vector<VertexAttributeDescription> attribute_descriptions{};
        
        log::graphics("");
        log::graphics("Creating attribute descriptions...");
        
        ui32 offset = 0;
        for_<Reflection::as_type_list<V>>([&](auto i){
            using T = std::variant_alternative_t<i.value, Reflection::as_type_list<V>>;
            str name = "";
            if constexpr (Reflection::is_reflectable<V>)
                name = str(V::member_names.at(i.value));
            
            int l = (int)attribute_descriptions.size();
            attribute_descriptions.resize(l + 1);
            
            attribute_descriptions[l].binding = binding;
            attribute_descriptions[l].location = l + previous_location;
            
            int size = sizeof(T);
            if (size % 4 != 0 or size < 4 or size > 16)
                log::error("Vertex data has an invalid size %d", size);
            attribute_descriptions[l].format = (VertexFormat)(size / 4);
            
            attribute_descriptions[l].offset = offset;
            
            log::graphics(" - Attribute %s [%d:%d] - Format : %d - Size : %d - Offset %d", name.c_str(), attribute_descriptions[l].binding,
                          attribute_descriptions[l].location, attribute_descriptions[l].format, size, offset);
            
            offset += size;
        });
        
        log::graphics("");
        return attribute_descriptions;
    }
}
