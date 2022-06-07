#pragma once
#include <vulkan/vulkan.h>


namespace vks{
    class BLAS{
    public:
        BLAS(VkAccelerationStructureGeometryKHR geoData);
        VkAccelerationStructureGeometryKHR geoData;
        VkAccelerationStructureInstanceKHR d;
    private:

    };



}