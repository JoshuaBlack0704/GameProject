#pragma once
#include <glm/glm.hpp>

struct VertexData
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec3 normal;


    static std::vector<VkVertexInputBindingDescription> vertexInputBinding()
    {
        std::vector<VkVertexInputBindingDescription> bindings;
        VkVertexInputBindingDescription vertexDesc = {};
        vertexDesc.binding = 0;
        vertexDesc.stride = sizeof(VertexData);
        vertexDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        bindings.emplace_back(vertexDesc);
        return bindings;
    }
    static std::vector<VkVertexInputAttributeDescription> vertexInputAttribute()
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
};

inline glm::vec3 GetSurfaceNormal(glm::vec3 V1, glm::vec3 V2, glm::vec3 V3)
{
    return glm::cross((V2 - V1), (V3 - V2));
}

struct Tetrahedron
{
    glm::vec3 bottomLeft{ -1, 1, -1 };
    glm::vec3 bottomRight{ 1, 1, -1 };
    glm::vec3 frontTop{ 0, -1, -1 };
    glm::vec3 back{ 0, 0, 3 };
    glm::vec3 color{ 1,1,1 };
    std::vector<VertexData> vertices{
            //Front face
            {bottomRight,     color, GetSurfaceNormal(frontTop, bottomLeft, bottomRight)},
            {frontTop, color, GetSurfaceNormal(frontTop, bottomLeft, bottomRight)},
            {bottomLeft,  color, GetSurfaceNormal(frontTop, bottomLeft, bottomRight)},
            //Bottom face
            {bottomLeft,  color, GetSurfaceNormal(bottomLeft, back, bottomRight)},
            {back,        color, GetSurfaceNormal(bottomLeft, back, bottomRight)},
            {bottomRight, color, GetSurfaceNormal(bottomLeft, back, bottomRight)},
            //Top Left face
            {bottomLeft, color, GetSurfaceNormal(back, bottomLeft, frontTop)},
            {frontTop,        color, GetSurfaceNormal(back, bottomLeft, frontTop)},
            {back,       color, GetSurfaceNormal(back, bottomLeft, frontTop)},
            //Top Right face
            {bottomRight,       color, GetSurfaceNormal(back, frontTop, bottomRight)},
            {back,        color, GetSurfaceNormal(back, frontTop, bottomRight)},
            {frontTop,  color, GetSurfaceNormal(back, frontTop, bottomRight)}
    };
};
