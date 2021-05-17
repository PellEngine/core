#include "window.h"

namespace pellengine {

Window::Window(const std::string name, bool enableValidationLayers) {
  this->instance = new Instance(name, enableValidationLayers);
  this->initialized = false;
  this->enableValidationLayers = enableValidationLayers;
}

Window::~Window() {
  delete instance;
}

void Window::initialize() {
  instance->initialize();

  if(initialized) {
    return;
  }

  createSwapChain();
  createImageViews();
  createRenderPass();
  createFramebuffers();
  createCommandPool();

  initialized = true;
}

void Window::terminate() {
  vkDestroyCommandPool(instance->getDevice(), commandPool, nullptr);

  for(auto framebuffer : swapChainFrameBuffers) {
    vkDestroyFramebuffer(instance->getDevice(), framebuffer, nullptr);
  }

  vkDestroyRenderPass(instance->getDevice(), renderPass, nullptr);

 for(auto imageView : swapChainImageViews) {
    vkDestroyImageView(instance->getDevice(), imageView, nullptr);
  }

  vkDestroySwapchainKHR(instance->getDevice(), swapChain, nullptr);
  instance->terminate();
  
  initialized = false;
}

/*
  ----------------------
  Window setup functions
  ----------------------
*/

void Window::createSwapChain() {
  SwapChainSupportDetails swapChainSupport = querySwapChainSupport(instance);

  VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

  if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = instance->getSurface();
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = instance->findQueueFamilies(instance->getPhysicalDevice());
  uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

  if(indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = nullptr;
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;

  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if(vkCreateSwapchainKHR(instance->getDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create swap chain");
  }

  vkGetSwapchainImagesKHR(instance->getDevice(), swapChain, &imageCount, nullptr);

  swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(instance->getDevice(), swapChain, &imageCount, swapChainImages.data());

  swapChainImageFormat = surfaceFormat.format;
  swapChainExtent = extent;
}

void Window::createImageViews() {
  swapChainImageViews.resize(swapChainImages.size());

  for(size_t i=0;i<swapChainImages.size();i++) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swapChainImages[i];

    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swapChainImageFormat;

    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if(vkCreateImageView(instance->getDevice(), &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create image views.");
    }
  }
}

void Window::createRenderPass() {
  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = swapChainImageFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;

  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;

  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &colorAttachment;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  if(vkCreateRenderPass(instance->getDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create render pass");
  }
}

void Window::createFramebuffers() {
  swapChainFrameBuffers.resize(swapChainImageViews.size());

  for(size_t i=0;i<swapChainImageViews.size();i++) {
    VkImageView attachments[] = {
      swapChainImageViews[i]
    };

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = swapChainExtent.width;
    framebufferInfo.height = swapChainExtent.height;
    framebufferInfo.layers = 1;

    if(vkCreateFramebuffer(instance->getDevice(), &framebufferInfo, nullptr, &swapChainFrameBuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create framebuffer.");
    }
  }
}

void Window::createCommandPool() {
  QueueFamilyIndices queueFamilyIndices = instance->findQueueFamilies(instance->getPhysicalDevice());

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  
  if(vkCreateCommandPool(instance->getDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create command pool.");
  }
}

/*
  -----------------------------
  Window setup helper functions
  -----------------------------
*/

SwapChainSupportDetails Window::querySwapChainSupport(Instance* instance) {
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(instance->getPhysicalDevice(), instance->getSurface(), &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(instance->getPhysicalDevice(), instance->getSurface(), &formatCount, nullptr);

  if(formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(instance->getPhysicalDevice(), instance->getSurface(), &formatCount, details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(instance->getPhysicalDevice(), instance->getSurface(), &presentModeCount, nullptr);

  if(presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(instance->getPhysicalDevice(), instance->getSurface(), &presentModeCount, details.presentModes.data());
  }

  return details;
}

VkSurfaceFormatKHR Window::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
  for(const auto& format : availableFormats) {
    if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return format;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR Window::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
  for(const auto& presentMode : availablePresentModes) {
    if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return presentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Window::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
  if(capabilities.currentExtent.width != UINT32_MAX) {
    return capabilities.currentExtent;
  } else {
    int width = instance->getWidth();
    int height = instance->getHeight();

    VkExtent2D actualExtent = {
      static_cast<uint32_t>(width),
      static_cast<uint32_t>(height)
    };

    actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
  }
}

}