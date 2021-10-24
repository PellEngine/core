#include "texture.h"

namespace pellengine {

Texture::Texture(std::shared_ptr<Window> window, std::string fileName) : window(window), fileName(fileName) {}
Texture::~Texture() {
  terminate();
}

void Texture::initialize() {
  if(initialized) return;
  // Load image pixels
  std::vector<char> textureBuffer;
  size_t textureBufferLength = AssetReader::getFileLength(fileName);
  textureBuffer.resize(textureBufferLength);
  AssetReader::getFileBuffer(fileName, textureBuffer);

  int width, height, channels;
  stbi_uc* pixels = stbi_load_from_memory((stbi_uc*)textureBuffer.data(), textureBufferLength, &width, &height, &channels, STBI_rgb_alpha); 
  VkDeviceSize imageSize = width * height * 4;

  if(!pixels) {
    throw std::runtime_error("Failed to load texture");
  }

  // Create staging buffer
  Buffer stagingBuffer; 
  createBuffer(
    window,
    &stagingBuffer,
    imageSize,
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VK_SHARING_MODE_EXCLUSIVE,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
  );

  void* data = stagingBuffer.map(window, 0, imageSize);
  memcpy(data, pixels, static_cast<size_t>(imageSize));
  stagingBuffer.unmap(window);
  stbi_image_free(pixels);

  textureImage = new Image(
    window,
    static_cast<uint32_t>(width),
    static_cast<uint32_t>(height),
    ImageConfiguration{
      .format = VK_FORMAT_R8G8B8A8_SRGB,
      .tiling = VK_IMAGE_TILING_OPTIMAL,
      .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
      .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    }
  );

  // Initialize image and transfer staging buffer to image.
  textureImage->initialize();
  textureImage->transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  textureImage->copyBuffer(stagingBuffer.buffer);
  textureImage->transitionImageLayout(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  terminateBuffer(window, &stagingBuffer);

  // Create image view
  VkImageViewCreateInfo viewInfo{};
  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image = this->textureImage->getImage();
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
  viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  if(vkCreateImageView(window->getInstance()->getDevice(), &viewInfo, nullptr, &textureImageView) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create image view.");
  }

  // Create sampler
  VkSamplerCreateInfo samplerInfo{};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;
  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.anisotropyEnable = VK_TRUE;

  VkPhysicalDeviceProperties properties{};
  vkGetPhysicalDeviceProperties(window->getInstance()->getPhysicalDevice(), &properties);
  samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;
  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.minLod = 0.0f;
  samplerInfo.maxLod = 0.0f;

  if(vkCreateSampler(window->getInstance()->getDevice(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create texture sampler.");
  }

  initialized = true;
}

void Texture::terminate() {
  if(!initialized) return;
  vkDestroySampler(window->getInstance()->getDevice(), textureSampler, nullptr);
  vkDestroyImageView(window->getInstance()->getDevice(), textureImageView, nullptr);
  delete textureImage;
  initialized = false;
}

}