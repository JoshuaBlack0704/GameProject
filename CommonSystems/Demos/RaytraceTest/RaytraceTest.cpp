#include <VkSystem.h>
#include <cmnCommon.h>
#include <spdlog/spdlog.h>
#include <flecs.h>
#include <functional>
#include <glm/glm.hpp>
#include <taskflow.hpp>
#include <bitset>

struct Light{
    glm::vec3 pos;
    Light(glm::vec3 pos) : pos(pos){}
};
struct Sphere{
    glm::vec3 pos;
    float radius;
    Sphere(glm::vec3 pos, float radius) : pos(pos), radius(radius){}
};
typedef union pixel_s {
    uint32_t uint32_value;
    uint8_t uint8_value[4];
} pixel_t;


bool IntersectSpheres(glm::vec3 origin, glm::vec3 ray, glm::vec3& intersection, glm::vec3& normal, std::vector<Sphere> const &spheres, float maxDistance){
    float minDistToIntersect = maxDistance;
    for (auto& sphere : spheres){
        float projectedDistance = glm::dot(sphere.pos - origin, ray);
        glm::vec3 projectedPoint = origin + projectedDistance * ray;
        float centerDist = glm::length(sphere.pos - projectedPoint);
        if  (projectedDistance > 0 && centerDist < sphere.radius){
            float x = sqrt(pow(sphere.radius, 2) - pow(centerDist, 2));
            float distanceToIntersect = projectedDistance - x;
            if(distanceToIntersect < minDistToIntersect){
                minDistToIntersect = distanceToIntersect;
                intersection = origin + distanceToIntersect * ray;
                normal = sphere.pos - intersection;
            }
        }
    }

    return minDistToIntersect != maxDistance;
}

struct RayTracer{
    RayTracer(std::vector<uint32_t>& data) : data(data), exe(1){
        traceRays = [this](GLFWwindow* window, int key, int scancode, int action, int mods){
            for(auto& sphere : spheres){
                if (sphere.radius != 80 && sphere.radius != .5f)
                sphere.pos.z -= .1;
            }
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            if(key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS){
                spdlog::info("TracingRays");
                if(width != pastExtent.width && height != pastExtent.height){
                    pastExtent.width = width; pastExtent.height = height;
                    float aspectRatio = static_cast<float>(pastExtent.width) / static_cast<float>(pastExtent.height);
                    aspectMat = {1*aspectRatio, 0, 0,
                                 0, 1, 0,
                                 0 ,0 ,0};
                    projectionMat = {2.0f/pastExtent.width, 0, 0,
                                     0, -2.0f/pastExtent.height, 0,
                                     0, 0, 0};
                    flow.clear();
                    for (int y = 0; y < height; ++y) {
                        for (int x = 0; x < width; ++x) {

                            flow.emplace([x, y, &projMat = this->projectionMat, &aspectMat = this->aspectMat, &pixel = this->data[x + y * width], this](){
                                glm::vec3 nearPlanePoint = {-1, 1, 0};
                                nearPlanePoint = (nearPlanePoint + projMat * glm::vec3{x, y, 0});
                                nearPlanePoint = aspectMat * nearPlanePoint;

                                //We are in world space here
                                glm::vec3 eyePoint(0, 0, -1);
                                glm::vec3 ray = glm::normalize(nearPlanePoint - eyePoint);
                                glm::vec3 intersection;
                                glm::vec3 normal;
                                int index = x + y;
                                pixel = 0x5f5fad;
                                if(IntersectSpheres(eyePoint, ray, intersection, normal, this->spheres, std::numeric_limits<float>::max())){

                                    //spdlog::info("Hit Sphere");
                                    for(auto& light : this->lights){
                                        ray = glm::normalize(light.pos - intersection);
                                        if (!IntersectSpheres(intersection, ray, intersection, normal, this->spheres, glm::length(light.pos - intersection))){
                                            //spdlog::info("Hit Light");
                                            glm::ivec3 color = {1, 1, 1};
                                            //color *= (255.0f*std::clamp(glm::dot(ray, normal), 0.0f, 1.0f));
                                            pixel_t pixelVal;
                                            pixelVal.uint8_value[0] = color.x;
                                            pixelVal.uint8_value[1] = color.y;
                                            pixelVal.uint8_value[2] = color.z;


                                            uint32_t value = 0xffffff;

                                            pixel = pixelVal.uint32_value;
                                        }
                                        else{
                                            pixel = 0;
                                        }
                                    }
                                }


                            });
                        }
                    }
                }
                exe.run(this->flow);
            }
        };
    }

    void AddSphere(glm::vec3 pos, float radius){
     spheres.emplace_back(Sphere{pos, radius});
    }
    void AddLight(glm::vec3 pos){
        lights.emplace_back(Light{pos});
    }

    std::vector<uint32_t>& data;
    tf::Taskflow flow;
    VkExtent2D pastExtent;
    std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> traceRays;
    std::vector<Sphere> spheres;
    std::vector<Light> lights;
    tf::Executor exe;
    glm::mat3 projectionMat;
    glm::mat3 aspectMat;


};

int main(){
    spdlog::info("Starting Raytrace test");
    vks::VkData vkData = vks::VkData(true);

    {
        cmn::GLFWSystem glfwSystem = cmn::GLFWSystem(vkData);
        std::vector<uint32_t> data;
        vks::Memory rayimageMemory = vks::Memory(vkData.lDevice.device, vkData.allocator, "Ray Image Memory");
        auto memoryCreate = [&vkData, &rayimageMemory, &data](GLFWwindow*,int width,int height){
            VkBufferCreateInfo bcInfo = {};
            bcInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            bcInfo.size = sizeof (uint32_t) * vkData.swapchain.extent.width * vkData.swapchain.extent.height;
            data.clear();
            data.resize(vkData.swapchain.extent.width*vkData.swapchain.extent.height, 0xff00ff);
            VmaAllocationCreateInfo acInfo = {};
            acInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
            rayimageMemory.SetAsBuffer(bcInfo, acInfo);

            spdlog::info("Resized ray image to {} bytes", bcInfo.size);
        };
        memoryCreate(nullptr,0,0);
        glfwSystem.AddResizeCallback(memoryCreate);
        RayTracer tracer(data);
        glfwSystem.AddKeyCallback(tracer.traceRays);
        tracer.AddSphere({0,0,20}, 1);
        tracer.AddSphere({0,-10,10}, 3);

        tracer.AddSphere({-5,0,20}, 1);
        tracer.AddSphere({5,0,20}, 1);
        tracer.AddSphere({0,-90,10}, 80);
        tracer.AddLight({0,10,20});
        tracer.AddSphere({0,12,20}, .5);

        VkCommandPoolCreateInfo cInfo = {};
        cInfo.queueFamilyIndex = vkData.lDevice.get_queue_index(vkb::QueueType::transfer).value();
        vks::CmdPoolWrapper cmdPool(vkData.lDevice.device, cInfo, "Main Command Pool");

        vks::CmdBufferSet& transferSet = cmdPool.MakeSet("Transfer Set");

        vks::Fence fence(vkData.lDevice.device, VK_FENCE_CREATE_SIGNALED_BIT);
        vks::Semaphore acquireImageSemaphore(vkData.lDevice.device);
        vks::Semaphore transferDoneSemaphore(vkData.lDevice.device);
        std::vector<VkSemaphore> waitSemaphores = {acquireImageSemaphore.semaphore};
        std::vector<VkPipelineStageFlags> waitStages = {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT};
        uint32_t imageIndex = 0;
        transferSet.SetFunctions({
                                         [&rayimageMemory, &data, &vkData, &imageIndex](VkCommandBuffer cmd){
                                             VkCommandBufferBeginInfo beginInfo = {};
                                             beginInfo.sType = vks::sType(beginInfo);
                                             vkBeginCommandBuffer(cmd,&beginInfo);



                                             VkImageSubresourceRange range = {};
                                             range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                                             range.levelCount = 1;
                                             range.baseMipLevel = 0;
                                             range.layerCount = 1;
                                             range.baseArrayLayer = 0;

                                             VkImageMemoryBarrier imgConversion = {};
                                             imgConversion.sType = vks::sType(imgConversion);
                                             imgConversion.srcAccessMask = VK_ACCESS_NONE;
                                             imgConversion.dstAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
                                             imgConversion.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                                             imgConversion.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                                             imgConversion.image = vkData.swapchain.get_images().value()[imageIndex];
                                             imgConversion.subresourceRange = range;

                                             vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_2_TRANSFER_BIT, VK_PIPELINE_STAGE_2_TRANSFER_BIT, {}, 0,
                                                                  nullptr, 0, nullptr, 1, &imgConversion);


                                             VkBufferImageCopy copy = {};
                                             VkImageSubresourceLayers layers = {};
                                             layers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                                             layers.baseArrayLayer = 0; layers.mipLevel = 0; layers.layerCount = 1;
                                             copy.imageSubresource = layers;
                                             copy.imageOffset = {0, 0};
                                             copy.imageExtent = VkExtent3D{vkData.swapchain.extent.width, vkData.swapchain.extent.height, 1};
                                             vkCmdCopyBufferToImage(cmd, rayimageMemory.buffer, vkData.swapchain.get_images().value()[imageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);



                                             imgConversion = {};
                                             imgConversion.sType = vks::sType(imgConversion);
                                             imgConversion.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
                                             imgConversion.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT;
                                             imgConversion.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                                             imgConversion.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
                                             imgConversion.image = vkData.swapchain.get_images().value()[imageIndex];
                                             imgConversion.subresourceRange = range;

                                             vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_2_TRANSFER_BIT, VK_PIPELINE_STAGE_2_TRANSFER_BIT, {}, 0,
                                                                  nullptr, 0, nullptr, 1, &imgConversion);

                                             vkEndCommandBuffer(cmd);
                                         }
                                 });


        while (!glfwWindowShouldClose(vkData.window)){
            glfwPollEvents();
            fence.Reset();
            vkAcquireNextImageKHR(vkData.lDevice.device, vkData.swapchain.swapchain, UINT64_MAX, acquireImageSemaphore.semaphore, nullptr, &imageIndex);

            rayimageMemory.TransferFromRam(data.data(), 0, 0, rayimageMemory.SizeInUse());
            cmdPool.Reset(true);

            VkSubmitInfo submit = {};
            submit.sType = vks::sType(submit);
            submit.waitSemaphoreCount = waitSemaphores.size();
            submit.pWaitSemaphores = waitSemaphores.data();
            submit.pWaitDstStageMask = waitStages.data();
            submit.commandBufferCount = 1;
            submit.pCommandBuffers = transferSet.pCommandBuffers();
            submit.signalSemaphoreCount = 1;
            submit.pSignalSemaphores = &transferDoneSemaphore.semaphore;


            VkPresentInfoKHR presentInfoKhr = {};
            presentInfoKhr.sType = vks::sType(presentInfoKhr);
            presentInfoKhr.waitSemaphoreCount = 1;
            presentInfoKhr.pWaitSemaphores = &transferDoneSemaphore.semaphore;
            presentInfoKhr.swapchainCount = 1;
            presentInfoKhr.pSwapchains = &vkData.swapchain.swapchain;
            presentInfoKhr.pImageIndices = &imageIndex;
            VkResult result;
            presentInfoKhr.pResults = &result;

            vkQueueSubmit(vkData.lDevice.get_queue(vkb::QueueType::transfer).value(), 1, &submit, fence.GetFence());

            vkQueuePresentKHR(vkData.lDevice.get_queue(vkb::QueueType::graphics).value(), &presentInfoKhr);
            fence.Wait();

        }
    }

}

