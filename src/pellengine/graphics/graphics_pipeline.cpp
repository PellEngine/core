#include "graphics_pipeline.h"

namespace pellengine {

GraphicsPipeline::GraphicsPipeline(std::shared_ptr<Window> window) : window(window) {}
GraphicsPipeline::~GraphicsPipeline() {}

void GraphicsPipeline::addShader(std::shared_ptr<Shader> shader) {
  shaders.push_back(shader);
}

void GraphicsPipeline::addDescriptorBuilder(DescriptorBuilder builder) {
  descriptorBuilders.push_back(builder);
}

void GraphicsPipeline::addBindingDescription(VkVertexInputBindingDescription bindingDescription) {
  bindingDescriptions.push_back(bindingDescription);
}

void GraphicsPipeline::addAttributeDescription(VkVertexInputAttributeDescription attributeDescription) {
  attributeDescriptions.push_back(attributeDescription);
}

void GraphicsPipeline::createPipeline(PipelineConfiguration& pipelineConfiguration) {
  // 1) Initialize the shaders
  for(std::shared_ptr<Shader>& shader : shaders) {
    shader->initialize();
    shaderStages.push_back(shader->getShaderStageCreateInfo());
  }

  // 2) Setup pipeline fixed functions
  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount = bindingDescriptions.size();
  vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
  vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
  vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
  
  // 3) Create descriptor set layout and descriptor sets
  descriptorSetLayouts.resize(descriptorBuilders.size());
  descriptorSets.resize(descriptorBuilders.size());

  for(int i=0;i<descriptorBuilders.size();i++) {
    DescriptorBuilder& builder = descriptorBuilders[i];
    descriptorSets[i].resize(window->getSwapChainImages().size());
    builder.build(window->getSwapChainImages().size(), descriptorSets[i].data(), descriptorSetLayouts[i]);
  }

  // 4) Create pipeline layout
  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
  pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
  pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
  pipelineLayoutCreateInfo.setLayoutCount = descriptorSetLayouts.size();
  pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();

  if(vkCreatePipelineLayout(window->getInstance()->getDevice(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create pipeline layout");
  }

  // 5) Create pipeline
  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = shaderStages.size();
  pipelineInfo.pStages = shaderStages.data();

  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = pipelineConfiguration.inputAssembly;
  pipelineInfo.pViewportState = pipelineConfiguration.viewportState;
  pipelineInfo.pRasterizationState = pipelineConfiguration.rasterizer;
  pipelineInfo.pMultisampleState = pipelineConfiguration.multisampling;
  pipelineInfo.pDepthStencilState = nullptr;
  pipelineInfo.pColorBlendState = pipelineConfiguration.colorBlending;
  pipelineInfo.pDynamicState = nullptr;

  pipelineInfo.layout = pipelineLayout;
  pipelineInfo.renderPass = window->getRenderPass();
  pipelineInfo.subpass = 0;

  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineInfo.basePipelineIndex = -1;
  
  if(vkCreateGraphicsPipelines(window->getInstance()->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create graphics pipeline");
  }

  // 6) Destroy shader modules
  for(std::shared_ptr<Shader>& shader : shaders) {
    shader->terminate();
  }
  shaders.clear();
}

void GraphicsPipeline::terminatePipeline() {
  vkDestroyPipeline(window->getInstance()->getDevice(), pipeline, nullptr);
  vkDestroyPipelineLayout(window->getInstance()->getDevice(), pipelineLayout, nullptr);
  for(VkDescriptorSetLayout layout : descriptorSetLayouts) {
    vkDestroyDescriptorSetLayout(window->getInstance()->getDevice(), layout, nullptr);
  }
  shaderStages.clear();
  bindingDescriptions.clear();
  attributeDescriptions.clear();
  descriptorSetLayouts.clear();
  descriptorSets.clear();
  shaders.clear();
  descriptorBuilders.clear();
}

}