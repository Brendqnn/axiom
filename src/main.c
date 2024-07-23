#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define WIDTH 1200
#define HEIGHT 800

typedef struct {
    uint32_t graphics_family[15];
    bool has_value;
} QueueFamilyIndices;

bool is_complete(QueueFamilyIndices *family)
{
    return family->has_value;
}

const char *validation_layers[] = {
    "VK_LAYER_KHRONOS_validation"
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

QueueFamilyIndices find_queue_families(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;
    indices.has_value = false;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);

    VkQueueFamilyProperties queue_families[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    for (unsigned int i = 0; i < queue_family_count; ++i) {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics_family[0] = i;
            indices.has_value = true;
            break;
        }
    }

    return indices;
}

bool is_device_suitable(VkPhysicalDevice device)
{   
    QueueFamilyIndices indices = find_queue_families(device);

    if (is_complete(&indices)) {
        return true;
    }

    return false;
}

void pick_physical_device(VkPhysicalDevice *physicalDevice, VkInstance instance)
{
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, NULL);

    if (device_count == 0) {
        fprintf(stderr, "Failed to find GPUs with Vulkan support.\n");
        exit(EXIT_FAILURE);
    }

    VkPhysicalDevice devices[device_count];
    vkEnumeratePhysicalDevices(instance, &device_count, devices);

    for (unsigned int i = 0; i < device_count; ++i) {
        if (is_device_suitable(devices[i])) {
            *physicalDevice = devices[i];
            break;
        }
    }

    if (*physicalDevice == VK_NULL_HANDLE) {
        fprintf(stderr, "Failed to find a suitable GPU.\n");
        exit(EXIT_FAILURE);
    }
}

void create_logical_device(VkPhysicalDevice physical_device, VkDevice *device)
{
    VkQueue gfx_q;
    QueueFamilyIndices indices = find_queue_families(physical_device);

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphics_family[0];
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

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
    pick_physical_device(&physicalDevice, instance);

    VkDevice device;
    create_logical_device(physicalDevice, &device);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    clean_vulkan(instance, surface, debug_messenger, device);
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
