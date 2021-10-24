#include "shader.h"

namespace pellengine {

Shader::Shader(std::shared_ptr<Window> window, std::string fileName, VkShaderStageFlagBits shaderStage) : window(window), fileName(fileName), shaderStage(shaderStage) {}
Shader::~Shader() {}

void Shader::initialize() {
  // Read shader code
  size_t shaderSize = AssetReader::getFileLength(fileName);
  std::vector<char> shaderCode;
  shaderCode.resize(shaderSize);
  AssetReader::getFileBuffer(fileName, shaderCode);

  // Create shader module
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.pNext = nullptr;
  createInfo.flags = 0;
  createInfo.codeSize = shaderCode.size();
  createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

  if(vkCreateShaderModule(window->getInstance()->getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create shader module");
  }

  // Create shader stage
  shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shaderStageCreateInfo.stage = shaderStage;
  shaderStageCreateInfo.module = shaderModule;
  shaderStageCreateInfo.pName = "main";
}

void Shader::terminate() {
  vkDestroyShaderModule(window->getInstance()->getDevice(), shaderModule, nullptr);
}

}