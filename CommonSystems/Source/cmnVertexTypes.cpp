#include <cmnVertexTypes.h>

namespace cmn{
    std::vector<VkVertexInputBindingDescription> VertexData::vertexInputBinding()
    {
        std::vector<VkVertexInputBindingDescription> bindings;
        VkVertexInputBindingDescription vertexDesc = {};
        vertexDesc.binding = 0;
        vertexDesc.stride = sizeof(VertexData);
        vertexDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        bindings.emplace_back(vertexDesc);
        return bindings;
    }

    std::vector<VkVertexInputAttributeDescription> VertexData::vertexInputAttribute()
    {
        std::vector<VkVertexInputAttributeDescription> attributes;
        VkVertexInputAttributeDescription posAttrib = {};
        posAttrib.binding = 0;
        posAttrib.location = 0;
        posAttrib.format = VK_FORMAT_R32G32B32_SFLOAT;
        posAttrib.offset = offsetof(VertexData, pos);

        VkVertexInputAttributeDescription colorAttrib = {};
        colorAttrib.binding = 0;
        colorAttrib.location = 1;
        colorAttrib.format = VK_FORMAT_R32G32B32_SFLOAT;
        colorAttrib.offset = offsetof(VertexData, color);

        VkVertexInputAttributeDescription normalAttrib = {};
        normalAttrib.binding = 0;
        normalAttrib.location = 2;
        normalAttrib.format = VK_FORMAT_R32G32B32_SFLOAT;
        normalAttrib.offset = offsetof(VertexData, normal);

        attributes.emplace_back(posAttrib);
        attributes.emplace_back(colorAttrib);
        attributes.emplace_back(normalAttrib);
        return attributes;
    }
}