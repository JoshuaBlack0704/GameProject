#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace cmn{

    struct VertexData
    {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec3 normal;
        static std::vector<VkVertexInputBindingDescription> vertexInputBinding();
        static std::vector<VkVertexInputAttributeDescription> vertexInputAttribute();
    };

}
