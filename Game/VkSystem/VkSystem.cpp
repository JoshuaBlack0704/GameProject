#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#include <VkSystem.h>
#include <spdlog/spdlog.h>

vks::VkModule::VkModule(flecs::world &world) {

    //The initialization function
    world.system<vks::NeedsCreationTag, vks::VkSystemData>().each([&world](vks::VkSystemData& data){
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        data.window = glfwCreateWindow(100, 100, "Game", nullptr, nullptr);
        auto instRet = vkb::InstanceBuilder()
                .require_api_version(VK_API_VERSION_1_3)
                .set_app_name("GameProject")
                .set_app_version(0,0,1)
                .set_engine_name("VkSystem")
                .set_engine_version(0,0,1)
#ifndef NDEBUG
                .enable_validation_layers()
                .build();
        spdlog::info("Enabling validation layers");
#else
        .build();
#endif
        if (!instRet){
            spdlog::error("Could not make a Vulkan Instance");
            abort();
        }

        data.instance = instRet.value();

        glfwCreateWindowSurface(data.instance.instance, data.window, nullptr, &data.surface);

        VkPhysicalDeviceFeatures features{};
        features.samplerAnisotropy = VK_TRUE;
        VkPhysicalDeviceVulkan11Features features11{};
        VkPhysicalDeviceVulkan12Features features12{};
        features12.timelineSemaphore = VK_TRUE;
        VkPhysicalDeviceVulkan13Features features13{};
        features13.dynamicRendering = VK_TRUE;


        auto pDeviceRet = vkb::PhysicalDeviceSelector(data.instance)
                .set_surface(data.surface)
                .add_required_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)
                .set_required_features(features)
                .set_required_features_11(features11)
                .set_required_features_12(features12)
                .set_required_features_13(features13)
                .require_present()
                .select();
        if (!pDeviceRet){
            spdlog::error("Could not select physical device");
            abort();
        }

        data.pDevice = pDeviceRet.value();

        auto lDevRet = vkb::DeviceBuilder(data.pDevice).build();
        if (!lDevRet){
            spdlog::error("Could not build logical device");
            abort();
        }
        data.lDevice = lDevRet.value();

        auto swapRet = vkb::SwapchainBuilder(data.lDevice)
                .set_old_swapchain(data.swapchain)
                .build();
        if (!swapRet){
            spdlog::error("Could not create swapchain");
            abort();
        }

        data.swapchain = swapRet.value();

        data.world = &world;

        glfwSetWindowUserPointer(data.window, &data);
        glfwSetWindowSizeCallback(data.window, [](GLFWwindow* window, int width, int height){
            auto data = reinterpret_cast<vks::VkSystemData*>(glfwGetWindowUserPointer(window));
            data->world->each([window, width, height](flecs::entity e, vks::ResizeCallback& resizeRequest){resizeRequest.callback(window, width, height, e);});
        });

        VmaAllocatorCreateInfo alocCreateInfo {};
        alocCreateInfo.instance = data.instance.instance;
        alocCreateInfo.device = data.lDevice.device;
        alocCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
        alocCreateInfo.physicalDevice = data.pDevice.physical_device;
        vmaCreateAllocator(&alocCreateInfo, &data.allocator);
    });


    /*world.system<vks::NeedsCreationTag, vks::VkSystemData, vks::ImGuiData>().each([](vks::VkSystemData& vkData, vks::ImGuiData imguiData){
        VkDescriptorPoolSize pool_sizes[] =
                {
                        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
                        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
                        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
                        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
                        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
                        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
                        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
                        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
                        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
                        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
                        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
                };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000;
        pool_info.poolSizeCount = std::size(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        vkCreateDescriptorPool(vkData.lDevice.device, &pool_info, nullptr, &imguiData.pool);

        VkAttachmentDescription colorDescription = {};
        colorDescription.format = vkData.swapchain.image_format;
        colorDescription.samples = VK_SAMPLE_COUNT_1_BIT;
        colorDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpassDescription = {};
        subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        VkAttachmentReference colorRef = {};
        colorRef.attachment = 0;
        colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        subpassDescription.colorAttachmentCount = 1;
        subpassDescription.pColorAttachments = &colorRef;

        VkAttachmentDescription desc[] = {};
        VkRenderPassCreateInfo rpCreateInfo = {};
        rpCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        rpCreateInfo.attachmentCount = 1;
        rpCreateInfo.pAttachments = &colorDescription;
        rpCreateInfo.subpassCount = 1;
        rpCreateInfo.pSubpasses = &subpassDescription;

        vkCreateRenderPass(vkData.lDevice.device, &rpCreateInfo, nullptr, &imguiData.renderPass);

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForVulkan(vkData.window, true);
        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = vkData.instance;
        initInfo.PhysicalDevice = vkData.pDevice.physical_device;
        initInfo.Device = vkData.lDevice.device;
        initInfo.QueueFamily = vkData.lDevice.get_queue_index(vkb::QueueType::graphics).value();
        initInfo.Queue = vkData.lDevice.get_queue(vkb::QueueType::graphics).value();
        initInfo.PipelineCache = nullptr;
        initInfo.DescriptorPool = imguiData.pool;
        initInfo.Subpass = 0;
        initInfo.MinImageCount = vkData.swapchain.image_count;
        initInfo.ImageCount = vkData.swapchain.image_count;
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        ImGui_ImplVulkan_Init(&initInfo, imguiData.renderPass);

        VkCommandPool cmdPool;
        VkCommandPoolCreateInfo poolCreateInfo;
        poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolCreateInfo.queueFamilyIndex = vkData.lDevice.get_queue_index(vkb::QueueType::graphics).value();
        vkCreateCommandPool(vkData.lDevice.device, &poolCreateInfo, nullptr, &cmdPool);
        VkCommandBuffer cmd;
        VkCommandBufferAllocateInfo cmdAloc = {};
        cmdAloc.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmdAloc.commandBufferCount = 1;
        cmdAloc.commandPool = cmdPool;
        cmdAloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        vkAllocateCommandBuffers(vkData.lDevice.device, &cmdAloc, &cmd);
        VkCommandBufferBeginInfo bgInfo = {};
        bgInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        vkBeginCommandBuffer(cmd, &bgInfo);
        ImGui_ImplVulkan_CreateFontsTexture(cmd);
        ImGui_ImplVulkan_DestroyFontUploadObjects();



    });*/

    world.system<vks::NeedsCreationTag>().each([](flecs::entity& e){e.remove<vks::NeedsCreationTag>();});



}