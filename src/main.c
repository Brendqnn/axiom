#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define WIDTH 1200
#define HEIGHT 800

typedef struct {
    uint32_t graphics_family[10];
    uint32_t present_family[10];
    bool has_value;
} QueueFamilyIndices;

/*
  
   ▒███▒      ▒██      ██▒   ▒██████▒   ▄██████▄    ▒███       ███▒
  ███ ███      ███    ███      ▀██▀    ███    ███    ████     ████
 ███   ███      ███  ███        ██     ███    ███    ██ ██▄ ▄██ ██
███     ███      ▒████▒         ██     ▒██    ██▒    ██  ▀███▀  ██
▒█████████▒     ███  ███        ██     ███    ███    ██    ▀    ██
███     ███    ███    ███      ▄██▄    ███    ███    ██         ██
███     ███   ▒██      ██▒   ▒██████░   ▀██████▀   ▄████▄     ▄████▄

*/

bool is_complete(QueueFamilyIndices *family)
{
    return family->has_value;
}

const char *validation_layers[] = {
    "VK_LAYER_KHRONOS_validation"
};

const char *device_extensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* pUserData)
{
    printf("%s\n", pCallbackData->pMessage);
    return VK_FALSE;
}

VkResult create_debug_utils_messengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                         const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != NULL) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void destroy_debug_utils_messengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != NULL) {
        func(instance, debugMessenger, pAllocator);
    }
}

void populate_debug_messenger(VkDebugUtilsMessengerCreateInfoEXT *createInfo)
{
    createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo->pNext = NULL;
    createInfo->flags = 0;
    createInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo->pfnUserCallback = debugCallback;
}

void setup_debug_messenger(VkInstance *instance, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populate_debug_messenger(&createInfo);

    if (create_debug_utils_messengerEXT(*instance, &createInfo, NULL, pDebugMessenger) != VK_SUCCESS) {
        fprintf(stderr, "Axiom failed to initialize debug messenger\n");
        return;
    }
}

bool check_validation_layer_support()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties availableLayers[layerCount];
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    for (size_t i = 0; i < sizeof(validation_layers) / sizeof(validation_layers[0]); i++) {
        bool layerFound = false;
        for (uint32_t j = 0; j < layerCount; j++) {
            if (strcmp(validation_layers[i], availableLayers[j].layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            return false;
        }
    }

    return true;
}

void create_vulkan_instance(VkInstance *instance)
{
    if (enableValidationLayers && !check_validation_layer_support()) {
        fprintf(stderr, "Validation layers requested, but not available!\n");
        exit(EXIT_FAILURE);
    }

    VkApplicationInfo app = {};
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pApplicationName = "Axiom-v1.0(Vulkan)";
    app.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app.pEngineName = "No Engine";
    app.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info.pApplicationInfo = &app;

    // Might need to move later
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    const char** extensions = (const char**)malloc((glfwExtensionCount + 1) * sizeof(const char*));
    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
        extensions[i] = glfwExtensions[i];
    }

    if (enableValidationLayers) {
        extensions[glfwExtensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
        glfwExtensionCount++;
    }

    info.enabledExtensionCount = glfwExtensionCount;
    info.ppEnabledExtensionNames = extensions;

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    if (enableValidationLayers) {
        info.enabledLayerCount = sizeof(validation_layers)/sizeof(validation_layers[0]);
        info.ppEnabledLayerNames = validation_layers;

        populate_debug_messenger(&debugCreateInfo);
        info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    } else {
        info.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&info, NULL, instance) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create Vulkan instance.\n");
        exit(EXIT_FAILURE);
    }
    
    free(extensions);
}

void create_vulkan_surface(VkInstance instance, GLFWwindow *window, VkSurfaceKHR *surface)
{
    VkResult result = glfwCreateWindowSurface(instance, window, NULL, surface);
    if (result != VK_SUCCESS) {
        fprintf(stderr, "Failed to create Vulkan surface. Error code: %d\n", result);
        exit(EXIT_FAILURE);
    }
}

QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;
    indices.has_value = false;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);

    VkQueueFamilyProperties *queue_families = (VkQueueFamilyProperties*)malloc(queue_family_count * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    for (unsigned int i = 0; i < queue_family_count; ++i) {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics_family[0] = i;
            indices.has_value = true;
        }

        if (surface != VK_NULL_HANDLE) {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport) {
                indices.present_family[0] = i;
            }

            if (is_complete(&indices)) {
                break;
            }
        }
    }

    free(queue_families);

    return indices;
}

bool is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices = find_queue_families(device, surface);

    if (is_complete(&indices)) {
        return true;
    }

    return false;
}

void pick_physical_device(VkPhysicalDevice *physicalDevice, VkInstance instance, VkSurfaceKHR surface)
{
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, NULL);

    if (device_count == 0) {
        fprintf(stderr, "Failed to find GPUs with Vulkan support.\n");
        exit(EXIT_FAILURE);
    }

    VkPhysicalDevice *devices = (VkPhysicalDevice*)malloc(device_count * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(instance, &device_count, devices);

    for (unsigned int i = 0; i < device_count; ++i) {
        if (is_device_suitable(devices[i], surface)) {
            *physicalDevice = devices[i];
            break;
        }
    }

    free(devices);

    if (*physicalDevice == VK_NULL_HANDLE) {
        fprintf(stderr, "Failed to find a suitable GPU.\n");
        exit(EXIT_FAILURE);
    }
}

void create_logical_device(VkPhysicalDevice physical_device, VkDevice *device, VkSurfaceKHR surface)
{
    VkQueue gfx_q;
    VkQueue present_q;
    
    QueueFamilyIndices indices = find_queue_families(physical_device, surface);

    VkDeviceQueueCreateInfo queueCreateInfos[15];

    uint32_t unique_queue_families[] = {
        indices.graphics_family[0],
        indices.present_family[0]
    };

    float queuePriority = 1.0f;

    int size = sizeof(unique_queue_families)/sizeof(unique_queue_families[0]);
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    
    for (uint32_t i = 0; i < size; ++i) {
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = i;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos[i] = queueCreateInfo;
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    VkDeviceCreateInfo createInfo = {};
    
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = size;
    createInfo.pQueueCreateInfos = queueCreateInfos;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = sizeof(validation_layers)/sizeof(validation_layers[0]);
        createInfo.ppEnabledLayerNames = validation_layers;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physical_device, &createInfo, NULL, device) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create logical device.\n");
        exit(EXIT_FAILURE);
    }

    vkGetDeviceQueue(*device, indices.graphics_family[0], 0, &gfx_q);
    vkGetDeviceQueue(*device, indices.present_family[0], 0, &present_q);
}

void clean_vulkan(VkInstance instance, VkSurfaceKHR surface, VkDebugUtilsMessengerEXT debug, VkDevice device)
{
    vkDestroyDevice(device, NULL);
    vkDestroySurfaceKHR(instance, surface, NULL);
    destroy_debug_utils_messengerEXT(instance, debug, NULL);
    vkDestroyInstance(instance, NULL);
}

int main(void)
{
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Axiom-v1.0(Vulkan)", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    VkInstance instance;
    create_vulkan_instance(&instance);

    VkSurfaceKHR surface;
    create_vulkan_surface(instance, window, &surface);

    VkDebugUtilsMessengerEXT debug_messenger;
    setup_debug_messenger(&instance, &debug_messenger);

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    pick_physical_device(&physicalDevice, instance, surface);

    VkDevice device = {};
    create_logical_device(physicalDevice, &device, surface);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    clean_vulkan(instance, surface, debug_messenger, device);
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}



/* NONE VULKAN LOOP                                                                                                                                                 */



/* #include <imgui.h> */
/* #include <imgui_impl_glfw.h> */
/* #include <imgui_impl_opengl3.h> */

/* #include <vulkan/vulkan.h> */

/* #include <pthread.h> */
/* #include <windows.h> */

/* #include <streamline.h> */

/* #include "util/axcamera.h" */
/* #include "gfx/axwindow.h" */
/* #include "util/axutil.h" */
/* #include "models/axmodel.h" */
/* #include "gfx/axshader.h" */
/* #include "gfx/axskybox.h" */

/* /\* */
  
/*    ▒███▒      ▒██      ██▒   ▒██████▒   ▄██████▄    ▒███       ███▒ */
/*   ███ ███      ███    ███      ▀██▀    ███    ███    ████     ████ */
/*  ███   ███      ███  ███        ██     ███    ███    ██ ██▄ ▄██ ██ */
/* ███     ███      ▒████▒         ██     ▒██    ██▒    ██  ▀███▀  ██ */
/* ▒█████████▒     ███  ███        ██     ███    ███    ██         ██ */
/* ███     ███    ███    ███      ▄██▄    ███    ███    ██         ██ */
/* ███     ███   ▒██      ██▒   ▒██████░   ▀██████▀   ▄████▄     ▄████▄ */

/* *\/ */

/* #define MAX_MODEL_BUFFER 20 */

/* typedef struct { */
/*     const char *model_path; */
/*     AXModel *model; */
/*     pthread_mutex_t *mutex; */
/* } ModelLoadArgs; */

/* void check_vulkan() { */
/*     VkInstance instance; */
/*     VkApplicationInfo appInfo; */
/*     VkInstanceCreateInfo createInfo; */
/*     VkResult result; */

/*     appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; */
/*     appInfo.pNext = NULL; */
/*     appInfo.pApplicationName = "Vulkan Check"; */
/*     appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); */
/*     appInfo.pEngineName = "No Engine"; */
/*     appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); */
/*     appInfo.apiVersion = VK_API_VERSION_1_0; */

/*     createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; */
/*     createInfo.pNext = NULL; */
/*     createInfo.flags = 0; */
/*     createInfo.pApplicationInfo = &appInfo; */
/*     createInfo.enabledLayerCount = 0; */
/*     createInfo.ppEnabledLayerNames = NULL; */
/*     createInfo.enabledExtensionCount = 0; */
/*     createInfo.ppEnabledExtensionNames = NULL; */

/*     result = vkCreateInstance(&createInfo, NULL, &instance); */
/*     if (result != VK_SUCCESS) { */
/*         printf("Failed to create Vulkan instance\n"); */
/*     } else { */
/*         printf("Vulkan instance created successfully\n"); */
/*         vkDestroyInstance(instance, NULL); */
/*     } */
/* } */

/* void *play_audio(void *args) */
/* { */
/*     SLAudioDevice *device = ((SLAudioDevice**)args)[0]; */

/*     Sleep(15000); */
/*     sl_play(device); */
/*     sl_free_device(device); */

/*     return NULL; */
/* } */

/* bool is_camera_near_edge(AXCamera *camera) */
/* { */
/*     float terrian_size = TERRAIN_SCALE*TERRAIN_SCALE/2.0f; */
/*     float abs_x = fabs(camera->position[0]); */
/*     float abs_y = fabs(camera->position[2]); */

/*     if (abs_x + camera->view_distance > terrian_size || abs_y + camera->view_distance > terrian_size) { */
/*         return true; */
/*     } */

/*     return false; */
/* } */

/* static inline float get_terrain_frequency(float min, float max) */
/* { */
/*     return min + ((float)rand()/RAND_MAX) * (max - min); */
/* } */

/* void gen_terrain(float map[TERRAIN_WIDTH][TERRAIN_HEIGHT], float vertices[]) */
/* { */
/*     float x_start = -TERRAIN_WIDTH / 2.0f * TERRAIN_SCALE; */
/*     float z_start = -TERRAIN_HEIGHT / 2.0f * TERRAIN_SCALE; */

/*     int index = 0; */
/*     float peak_scale = 1.0f; */

/*     for (int x = 0; x < TERRAIN_WIDTH; ++x) { */
/*         for (int z = 0; z < TERRAIN_HEIGHT; ++z) { */
/*             vertices[index++] = x_start + x * TERRAIN_SCALE; */
/*             vertices[index++] = peak_scale * map[x][z] * TERRAIN_SCALE; */
/*             vertices[index++] = z_start + z * TERRAIN_SCALE; */
/*         } */
/*     } */
/* } */

/* void gen_texture_coordinates(float texture_coords[], float scale_factor) */
/* { */
/*     int index = 0; */
/*     for (int x = 0; x < TERRAIN_WIDTH; ++x) { */
/*         for (int z = 0; z < TERRAIN_HEIGHT; ++z) { */
/*             float s = (float)x / TERRAIN_WIDTH * scale_factor; */
/*             float t = (float)z / TERRAIN_HEIGHT * scale_factor; */
            
/*             texture_coords[index++] = s; */
/*             texture_coords[index++] = t; */
/*         } */
/*     } */
/* } */

/* void gen_terrain_indices(unsigned int indices[]) */
/* { */
/*     int index = 0; */
/*     for (int x = 0; x < TERRAIN_WIDTH - 1; ++x) { */
/*         for (int z = 0; z < TERRAIN_HEIGHT - 1; ++z) { */
/*             int top_left = x * TERRAIN_HEIGHT + z; */
/*             int top_right = top_left + 1; */
/*             int bottom_left = (x + 1) * TERRAIN_HEIGHT + z; */
/*             int bottom_right = bottom_left + 1; */

/*             indices[index++] = top_left; */
/*             indices[index++] = bottom_left; */
/*             indices[index++] = top_right; */

/*             indices[index++] = top_right; */
/*             indices[index++] = bottom_left; */
/*             indices[index++] = bottom_right; */
/*         } */
/*     } */
/* } */

/* void generate_heightmap(float map[TERRAIN_WIDTH][TERRAIN_HEIGHT]) */
/* { */
/*     for (int x = 0; x < TERRAIN_WIDTH + 1; x++) { */
/*         for (int z = 0; z < TERRAIN_HEIGHT + 1; z++) { */
/*             float height = pnoise3(x * 0.1f, 1.0f, z * 0.1f, 1024, 1024, 1024); */
/*             map[x][z] = height; */
/*         } */
/*     } */
/* } */

/* int main(void) */
/* { */
/*     AXWindow window = ax_window_create(WINDOW_WIDTH, WINDOW_HEIGHT, "Axiom v1.0"); */
/*     window.fullscreen = true; */
/*     ax_toggle_fullscreen(&window); */

/*     check_vulkan(); */

/*     ImGui::CreateContext(); */
/*     ImGuiIO& io = ImGui::GetIO(); */
/*     io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; */
/*     io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; */

/*     ImGui_ImplGlfw_InitForOpenGL(window.handle, true); */
/*     ImGui_ImplOpenGL3_Init("#version 460"); */
/*     ImGui::StyleColorsDark(); */

/*     bool vsync = true; */

/*     const char* skybox_face_paths[6] = { */
/*         "res/skybox/px.jpg", */
/*         "res/skybox/nx.jpg", */
/*         "res/skybox/py.jpg", */
/*         "res/skybox/ny.jpg", */
/*         "res/skybox/pz.jpg", */
/*         "res/skybox/nz.jpg" */
/*     }; */

/*     AXModel model_t2 = ax_load_model("res/models/trees/t1/Prunus_Pendula.obj"); */
/*     AXModel model_t1 = ax_load_model("res/models/trees/t4/scene.gltf"); */

/*     AXTexture mud = ax_load_texture("res/terrain/brown_mud_diff_4k.jpg"); */
/*     AXTexture dirt = ax_load_texture("res/terrain/forest_ground_04_diff_4k.jpg"); */
/*     AXTexture dirt_grass = ax_load_texture("res/terrain/forrest_ground_01_diff_4k.jpg"); */
    
/*     AXShader model_shader = ax_create_shader("res/shaders/default.vert", "res/shaders/default.frag"); */
/*     AXShader terrain_shader = ax_create_shader("res/shaders/terrain.vert", "res/shaders/terrain.frag"); */
/*     AXShader model_instance_shader = ax_create_shader("res/shaders/modelinstance.vert", "res/shaders/modelinstance.frag"); */

/*     AXSkybox skybox = ax_init_skybox(skybox_face_paths); */

/*     AXCamera camera; */
/*     vec3 position = {0.0f, 0.0f, 100.0f}; */
/*     vec3 up = {0.0f, 50.0f, 0.0f}; */

/*     ax_init_camera(&camera, position, up, -90.0f, 0.0f, CAMERA_FOV); */

/*     SLAudioDevice *device = sl_setup_audio_device("res/sfx/cicadas.mp3", 0.1f); */

/*     pthread_t audio_thread; */
/*     void *args[] = {device}; */

/*     if (pthread_create(&audio_thread, NULL, play_audio, (void *)args)) { */
/*         fprintf(stderr, "Error creating thread\n"); */
/*         return 1; */
/*     } */

/*     int num_vertices = TERRAIN_WIDTH * TERRAIN_HEIGHT * 3; */
/*     int num_indices = (TERRAIN_WIDTH - 1) * (TERRAIN_HEIGHT - 1) * 6; */
    
/*     float map[TERRAIN_WIDTH][TERRAIN_HEIGHT]; */
/*     float vertices[num_vertices]; */
/*     float texture_coords[TERRAIN_WIDTH * TERRAIN_HEIGHT * 2]; */
    
/*     unsigned int indices[num_indices]; */

/*     generate_heightmap(map); */
/*     gen_terrain(map, vertices); */
    
/*     float texture_scale = TERRAIN_SCALE/2; */
    
/*     gen_texture_coordinates(texture_coords, texture_scale); */
/*     gen_terrain_indices(indices); */

/*     AXVao terrain_vao = ax_create_vao(); */
/*     AXVbo terrain_vbo = ax_create_vbo(GL_ARRAY_BUFFER, false); */
/*     AXVbo terrain_tbo = ax_create_vbo(GL_ARRAY_BUFFER, false); */
/*     AXEbo terrain_ebo = ax_create_ebo(); */

/*     ax_bind_vao(terrain_vao); */

/*     ax_bind_vbo_buffer(terrain_vbo, vertices, sizeof(vertices)); */
/*     ax_link_attrib(terrain_vao, terrain_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), 0); */

/*     ax_bind_vbo_buffer(terrain_tbo, texture_coords, sizeof(texture_coords)); */
/*     ax_link_attrib(terrain_vao, terrain_tbo, 1, 2, GL_FLOAT, 2 * sizeof(float), 0); */

/*     ax_bind_vbo_buffer(terrain_tbo, texture_coords, sizeof(texture_coords)); */
/*     ax_link_attrib(terrain_vao, terrain_tbo, 2, 2, GL_FLOAT, 2 * sizeof(float), 0); */

/*     ax_bind_ebo_buffer(terrain_ebo, indices, sizeof(indices)); */

/*     int num_instances = 5; */

/*     vec2 matrices[num_instances] = { */
/*         {0.0f, 0.0f}, */
/*         {50.0f, 0.0f} */
/*     }; */
  
/*     glBindVertexArray(model_t1.meshes[0].VAO); */
    
/*     unsigned int instanceVBO; */
/*     glGenBuffers(1, &instanceVBO); */
/*     glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); */
/*     glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * num_instances, &matrices[0], GL_STATIC_DRAW); */
/*     glBindBuffer(GL_ARRAY_BUFFER, 0); */

/*     glEnableVertexAttribArray(3); */
/*     glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); */
/*     glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); */
/*     glBindBuffer(GL_ARRAY_BUFFER, 0); */
/*     glVertexAttribDivisor(3, 1); */
    
/*     float lightDir[3] = {0.0f, -1.0f, 1.0f}; */
/*     glm_vec3_normalize(lightDir); */
/*     float lightColor[3] = {1.0f, 1.0f, 1.0f}; */
/*     float ambientColor[3] = {0.46f, 0.46f, 0.46f}; */

/*     glEnable(GL_DEPTH_TEST); */

/*     ax_set_cursor_pos_callback(&camera, &window); */

/*     mat4 terrain_model_mat; */

/*     glEnable(GL_BLEND); */
/*     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); */

/*     while (!ax_window_should_close(&window)) { */
/*         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */

/*         ax_update_camera(&camera, &window); */

/*         glm_mat4_identity(terrain_model_mat); */

/*         glActiveTexture(GL_TEXTURE0); */
/*         glBindTexture(GL_TEXTURE_2D, mud.id); */
/*         glActiveTexture(GL_TEXTURE1); */
/*         glBindTexture(GL_TEXTURE_2D, dirt.id); */
/*         glActiveTexture(GL_TEXTURE2); */
/*         glBindTexture(GL_TEXTURE_2D, dirt_grass.id); */

/*         ax_bind_texture(&mud); */
/*         ax_bind_texture(&dirt); */
/*         ax_bind_texture(&dirt_grass); */

/*         ax_use_shader(&terrain_shader); */
/*         glActiveTexture(GL_TEXTURE3); */
/*         glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemap.id); */
/*         GLuint skyboxLocation = glGetUniformLocation(terrain_shader.ID, "skybox"); */
/*         glUniform1i(skyboxLocation, 3); */

/*         ax_set_shader_vec3_from_array(&terrain_shader, "lightDir", lightDir); */
/*         ax_set_shader_vec3_from_array(&terrain_shader, "lightColor", lightColor); */
/*         ax_set_shader_vec3_from_array(&terrain_shader, "ambientColor", ambientColor); */
/*         ax_set_shader_vec3_from_array(&terrain_shader, "viewPos", camera.position); */

/*         ax_set_shader_int(&terrain_shader, "terrainTextures[0]", 0); */
/*         ax_set_shader_int(&terrain_shader, "terrainTextures[1]", 1); */
/*         ax_set_shader_int(&terrain_shader, "terrainTextures[2]", 2); */

/*         ax_uniform_shader_mat4(&terrain_shader, "view", camera.view); */
/*         ax_uniform_shader_mat4(&terrain_shader, "projection", camera.projection); */
/*         ax_uniform_shader_mat4(&terrain_shader, "model", terrain_model_mat); */

/*         ax_bind_vao(terrain_vao); */
/*         glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0); */
        
/*         ax_use_shader(&model_instance_shader); */

/*         ax_uniform_shader_mat4(&model_instance_shader, "view", camera.view); */
/*         ax_uniform_shader_mat4(&model_instance_shader, "projection", camera.projection); */

/*         /\* for (int i = 0; i < model_t1.meshes[0].num_textures; ++i) { *\/ */
/*         /\*     char uniform_name[64]; *\/ */
/*         /\*     snprintf(uniform_name, sizeof(uniform_name), "material.texture%d", i); *\/ */

/*         /\*     ax_set_shader_int(&model_instance_shader, uniform_name, i); *\/ */

/*         /\*     glActiveTexture(GL_TEXTURE0 + i); *\/ */
/*         /\*     glBindTexture(GL_TEXTURE_2D, model_t1.meshes[i].textures[i].id); *\/ */
/*         /\* } *\/ */

/*         /\* glBindVertexArray(model_t1.meshes[0].VAO); *\/ */
/*         /\* glDrawElementsInstanced(GL_TRIANGLES, model_t1.meshes[0].num_indices, GL_UNSIGNED_INT, 0, num_instances); *\/ */

/*         ax_draw_model_instances(model_t1, &model_instance_shader, num_instances); */
        
/*         // Render Model 2 */
/*         glm_mat4_identity(model_t2.matrix); */

/*         vec3 translate = {0.0f, 0.0f, -2400.0f}; */
/*         glm_translate(model_t2.matrix, translate); */
    
/*         vec3 scale_vector2 = {20.0f, 20.0f, 20.0f}; */
/*         glm_scale(model_t2.matrix, scale_vector2); */

/*         ax_use_shader(&model_shader); */

/*         ax_set_shader_vec3_from_array(&model_shader, "lightDir", lightDir); */
/*         ax_set_shader_vec3_from_array(&model_shader, "lightColor", lightColor); */
/*         ax_set_shader_vec3_from_array(&model_shader, "ambientColor", ambientColor); */
/*         ax_set_shader_vec3_from_array(&model_shader, "viewPos", camera.position); */

/*         ax_uniform_shader_mat4(&model_shader, "view", camera.view); */
/*         ax_uniform_shader_mat4(&model_shader, "projection", camera.projection); */
/*         ax_uniform_shader_mat4(&model_shader, "model", model_t2.matrix); */

/*         ax_draw_model(model_t2, &model_shader); */

/*         ax_render_skybox(skybox, &camera); */

/*         if (glfwGetKey(window.handle, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(window.handle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) { */
/*             sl_free_device(device); */
/*             ax_free_texture(&dirt); */
/*             ax_free_texture(&dirt_grass); */
/*             ax_free_shader(&terrain_shader); */
/*             ax_free_shader(&model_shader); */
/*             ax_free_shader(&model_instance_shader); */
/*             ax_free_model(&model_t2); */
/*             exit(1); */
/*         } */

/*         // Debug UI */
/*         ImGui_ImplOpenGL3_NewFrame(); */
/*         ImGui_ImplGlfw_NewFrame(); */
/*         ImGui::NewFrame(); */

/*         ImGuiViewport* main_viewport = ImGui::GetMainViewport(); */
/*         ImGui::DockSpaceOverViewport(main_viewport->ID, main_viewport, ImGuiDockNodeFlags_PassthruCentralNode); */

/*         ImGui::Begin("Performance"); */
/*         double fps = ax_calculate_fps(); */
/*         ImGui::Text("FPS: %.00f\r", fps); */
/*         ImGui::Separator(); */

/*         ImGui::End(); */

/*         ImGui::Begin("Settings"); */
/*         ImGui::Text("Enable V-Sync"); */
/*         if (ImGui::Checkbox(" ", &vsync)) { */
/*             if (vsync) { */
/*                 ax_enable_vsync(); */
/*             } else { */
/*                 ax_disable_vsync(); */
/*             } */
/*         } */
        
/*         ImGui::Separator(); */
/*         ImGui::Text("Render Distance"); */
/*         if (ImGui::SliderFloat(" ", &(camera.view_distance), 1000.0f, 20000.0f)) { */
/*             ax_set_camera_view(&camera); */
/*         } */
/*         ImGui::End(); */

/*         ImGui::Begin("Camera"); */
/*         ImGui::Text("Camera Position: (%.02f, %.02f, %.02f)\n", camera.position[0], camera.position[1], camera.position[2]); */
/*         bool edges = is_camera_near_edge(&camera); */
/*         ImGui::Checkbox(" ", &edges); */
/*         ImGui::Separator(); */
/*         ImGui::Text("Movement Speed"); */
/*         ImGui::SliderFloat(" ", &(camera.movement_speed), 10.0f, 5000.0f); */
/*         ImGui::End(); */

/*         ImGui::Render(); */
/*         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); */

/*         if (glfwGetKey(window.handle, GLFW_KEY_R) == GLFW_PRESS) { */
/*             camera.debug_ui = true; */
/*             ax_disable_cursor_capture(window.handle); */
/*         } */

/*         if (glfwGetKey(window.handle, GLFW_KEY_T) == GLFW_PRESS) { */
/*             camera.debug_ui = false; */
/*             ax_enable_cursor_capture(&camera, &window); */
/*         } */

/*         ImGui::UpdatePlatformWindows(); */

/*         glfwSwapBuffers(window.handle); */
/*         glfwPollEvents(); */
/*     } */

/*     if (pthread_join(audio_thread, NULL)) { */
/*         fprintf(stderr, "Error joining thread\n"); */
/*         return 2; */
/*     } */
    
/*     glfwDestroyWindow(window.handle); */
/*     glfwTerminate(); */
    
/*     return 0; */
/* } */
