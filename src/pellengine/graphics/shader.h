#ifndef _PELLENGINE_GRAPHICS_SHADER_H_
#define _PELLENGINE_GRAPHICS_SHADER_H_

#include "src/pellengine/graphics/window.h"
#include "src/pellengine/io/asset_reader.h"
#include <memory>
#include <string>

namespace pellengine {

class Shader {
 public:
  Shader(std::shared_ptr<Window> window, std::string fileName, VkShaderStageFlagBits shaderStage);
  ~Shader();

  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  void initialize();
  void terminate();

  VkPipelineShaderStageCreateInfo getShaderStageCreateInfo() {
    return shaderStageCreateInfo;
  }

 private:
  std::shared_ptr<Window> window;
  std::string fileName;
  VkShaderStageFlagBits shaderStage;
  VkShaderModule shaderModule;
  VkPipelineShaderStageCreateInfo shaderStageCreateInfo{};
};

}

#endif