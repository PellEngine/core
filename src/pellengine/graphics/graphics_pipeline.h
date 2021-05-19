#ifndef _PELLENGINE_GRAPHICS_GRAPHICS_PIPELINE_H_
#define _PELLENGINE_GRAPHICS_GRAPHICS_PIPELINE_H_

#include <pellengine/vulkan/vulkan_wrapper.h>
#include <pellengine/graphics/window.h>
#include <pellengine/io/asset_reader.h>
#include <string>
#include <optional>
#include <memory>

namespace pellengine {

struct ShaderConfiguration {
  std::optional<std::string> vertexShader;
  std::optional<std::string> fragmentShader;
  std::optional<std::string> tesselationShader;
  std::optional<std::string> geometryShader;

  static const ShaderConfiguration test() {
    return {
      "shaders/test.vert.spv",
      "shaders/test.frag.spv"
    };
  }
};

class GraphicsPipeline {
 public:
  GraphicsPipeline(std::shared_ptr<Window> window, ShaderConfiguration shaderConfiguration);
  ~GraphicsPipeline();

  GraphicsPipeline(const GraphicsPipeline&) = delete;
  GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

  void initialize();
  void terminate();

  VkPipeline getPipeline() {
    return pipeline;
  }

 private:
  ShaderConfiguration shaderConfiguration;
  std::shared_ptr<Window> window;
  VkPipelineLayout pipelineLayout;
  VkPipeline pipeline;

  void createShaderModule(std::vector<char>& code, VkShaderModule* shaderModule);
};

}

#endif