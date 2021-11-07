#ifndef _PELLENGINE_GRAPHICS_GRAPHICS_PIPELINE_H_
#define _PELLENGINE_GRAPHICS_GRAPHICS_PIPELINE_H_

#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/descriptor_builder.h"
#include "src/pellengine/graphics/shader.h"
#include <memory>
#include <vector>

namespace pellengine {

struct PipelineConfiguration {
  VkPipelineInputAssemblyStateCreateInfo* inputAssembly;
  VkPipelineViewportStateCreateInfo* viewportState;
  VkPipelineRasterizationStateCreateInfo* rasterizer;
  VkPipelineMultisampleStateCreateInfo* multisampling;
  VkPipelineColorBlendStateCreateInfo* colorBlending;
};

class GraphicsPipeline {
 public:
  GraphicsPipeline(std::shared_ptr<Window> window);
  virtual ~GraphicsPipeline();

  GraphicsPipeline(const GraphicsPipeline&) = delete;
  GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

  virtual void initialize() = 0;
  virtual void terminate() = 0;

  VkPipeline getPipeline() {
    return pipeline;
  }

  VkPipelineLayout getPipelineLayout() {
    return pipelineLayout;
  }

  std::vector<std::vector<VkDescriptorSet>>& getDescriptorSets() {
    return descriptorSets;
  }

 std::vector<VkDescriptorSetLayout>& getDescriptorSetLayouts() {
    return descriptorSetLayouts;
  }

 protected:
  bool initialized = false;
  std::shared_ptr<Window> window;

  void createPipeline(PipelineConfiguration& pipelineConfiguration);
  void terminatePipeline();
  
  void addShader(std::shared_ptr<Shader> shader);
  void addDescriptorBuilder(DescriptorBuilder builder);
  void addBindingDescription(VkVertexInputBindingDescription bindingDescription);
  void addAttributeDescription(VkVertexInputAttributeDescription attributeDescription);

 private:
  std::vector<std::shared_ptr<Shader>> shaders;
  std::vector<DescriptorBuilder> descriptorBuilders;

  VkPipeline pipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
  std::vector<VkVertexInputBindingDescription> bindingDescriptions;
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
  std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
  std::vector<std::vector<VkDescriptorSet>> descriptorSets;
};

}

#endif