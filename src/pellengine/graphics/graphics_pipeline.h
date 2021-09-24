#ifndef _PELLENGINE_GRAPHICS_GRAPHICS_PIPELINE_H_
#define _PELLENGINE_GRAPHICS_GRAPHICS_PIPELINE_H_

#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/uniform_buffer.h"
#include "src/pellengine/graphics/descriptor_set_provider.h"
#include "src/pellengine/io/asset_reader.h"
#include <string>
#include <optional>
#include <memory>
#include <set>
#include <unordered_map>

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

struct VertexConfiguration {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions;
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
};

struct DescriptorSetConfiguration {
  std::vector<std::shared_ptr<DescriptorSetProvider>> descriptorSetProviders;
};

class GraphicsPipeline {
 public:
  GraphicsPipeline(std::shared_ptr<Window> window, ShaderConfiguration shaderConfiguration, VertexConfiguration vertexConfiguration, DescriptorSetConfiguration descriptorSetConfiguration);
  ~GraphicsPipeline();

  GraphicsPipeline(const GraphicsPipeline&) = delete;
  GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

  void initialize();
  void terminate();

  VkPipeline getPipeline() {
    return pipeline;
  }

  VkPipelineLayout getPipelineLayout() {
    return pipelineLayout;
  }

  std::vector<VkDescriptorSet>& getDescriptorSets() {
    return descriptorSets;
  }

 private:
  bool initialized = false;;
  ShaderConfiguration shaderConfiguration;
  VertexConfiguration vertexConfiguration;
  DescriptorSetConfiguration descriptorSetConfiguration;
  std::shared_ptr<Window> window;
  VkPipelineLayout pipelineLayout;
  VkPipeline pipeline;
  VkDescriptorSetLayout descriptorSetLayout;
  std::vector<VkDescriptorSet> descriptorSets;
  VkDescriptorPool descriptorPool;

  void createDescriptorSetLayout();
  void createDescriptorPool();
  void createDescriptorSets();
  void createShaderModule(std::vector<char>& code, VkShaderModule* shaderModule);
};

}

#endif