#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <cmnVertexTypes.h>

namespace cmn{

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

}
