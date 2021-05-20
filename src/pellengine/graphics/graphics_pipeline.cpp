#include "graphics_pipeline.h"

namespace pellengine {

GraphicsPipeline::GraphicsPipeline(std::shared_ptr<Window> window, ShaderConfiguration shaderConfiguration) : window(window), shaderConfiguration(shaderConfiguration) {}

GraphicsPipeline::~GraphicsPipeline() {
  terminate();
}

void GraphicsPipeline::initialize() {
  if(initialized) return;

  size_t vertexShaderSize;
  size_t fragmentShaderSize;

  std::vector<char> vertexShaderCode;
  std::vector<char> fragmentShaderCode;

  VkShaderModule vertexShaderModule;
  VkShaderModule fragmentShaderModule;

  if(shaderConfiguration.vertexShader.has_value()) {
    vertexShaderSize = AssetReader::getFileLength(shaderConfiguration.vertexShader.value());
    vertexShaderCode.resize(vertexShaderSize);
    AssetReader::getFileBuffer(shaderConfiguration.vertexShader.value(), vertexShaderCode);
    createShaderModule(vertexShaderCode, &vertexShaderModule);
  }

  if(shaderConfiguration.fragmentShader.has_value()) {
    fragmentShaderSize = AssetReader::getFileLength(shaderConfiguration.fragmentShader.value());
    fragmentShaderCode.resize(fragmentShaderSize);
    AssetReader::getFileBuffer(shaderConfiguration.fragmentShader.value(), fragmentShaderCode);
    createShaderModule(fragmentShaderCode, &fragmentShaderModule);
  }

  std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

  if(shaderConfiguration.vertexShader.has_value()) {
    VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
    vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageInfo.module = vertexShaderModule;
    vertexShaderStageInfo.pName = "main";
    shaderStages.push_back(vertexShaderStageInfo);
  }

  if(shaderConfiguration.fragmentShader.has_value()) {
    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
    fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = fragmentShaderModule;
    fragmentShaderStageInfo.pName = "main";
    shaderStages.push_back(fragmentShaderStageInfo);
  }

  // Setup the pipeline fixed functions
  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount = 0;
  vertexInputInfo.pVertexAttributeDescriptions = nullptr;
  vertexInputInfo.vertexAttributeDescriptionCount = 0;
  vertexInputInfo.pVertexAttributeDescriptions = nullptr;

  // Setup input assembly
  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  // Setup viewport
  VkViewport viewport{};
  viewport.x = 0;
  viewport.y = 0;
  viewport.width = window->getSwapChainExtent().width;
  viewport.height = window->getSwapChainExtent().height;
  viewport.minDepth = 0;
  viewport.maxDepth = 1;

  // Setup viewport scissors
  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = window->getSwapChainExtent();

  // Setup viewport state
  VkPipelineViewportStateCreateInfo viewportState{};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports = &viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &scissor;

  // Setup rasterizer
  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 0;
  rasterizer.depthBiasClamp = 0;
  rasterizer.depthBiasSlopeFactor = 0;

  // Setup multisampling options
  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading = 1.0f;
  multisampling.pSampleMask = nullptr;
  multisampling.alphaToCoverageEnable = VK_FALSE;
  multisampling.alphaToOneEnable = VK_FALSE;

  // Setup color blending
  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_FALSE;
  colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
  colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

  VkPipelineColorBlendStateCreateInfo colorBlending{};
  colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f;
  colorBlending.blendConstants[1] = 0.0f;
  colorBlending.blendConstants[2] = 0.0f;
  colorBlending.blendConstants[3] = 0.0f;

  // Create pipeline layout
  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
  pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutCreateInfo.setLayoutCount = 0;
  pipelineLayoutCreateInfo.pSetLayouts = nullptr;
  pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
  pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

  if(vkCreatePipelineLayout(window->getInstance()->getDevice(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create pipeline layout");
  }

  // Create pipeline
  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = shaderStages.size();
  pipelineInfo.pStages = shaderStages.data();

  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pDepthStencilState = nullptr;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.pDynamicState = nullptr;

  pipelineInfo.layout = pipelineLayout;
  pipelineInfo.renderPass = window->getRenderPass();
  pipelineInfo.subpass = 0;

  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineInfo.basePipelineIndex = -1;

  if(vkCreateGraphicsPipelines(window->getInstance()->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create graphics pipeline");
  }

  if(shaderConfiguration.vertexShader.has_value())
    vkDestroyShaderModule(window->getInstance()->getDevice(), fragmentShaderModule, nullptr);

  if(shaderConfiguration.fragmentShader.has_value())
    vkDestroyShaderModule(window->getInstance()->getDevice(), vertexShaderModule, nullptr);

  initialized = true;
}

void GraphicsPipeline::terminate() {
  if(!initialized) return;
  vkDestroyPipeline(window->getInstance()->getDevice(), pipeline, nullptr);
  vkDestroyPipelineLayout(window->getInstance()->getDevice(), pipelineLayout, nullptr);
  initialized = false;
}

void GraphicsPipeline::createShaderModule(std::vector<char>& code, VkShaderModule* shaderModule) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.pNext = nullptr;
  createInfo.flags = 0;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

  if(vkCreateShaderModule(window->getInstance()->getDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create shader module");
  }

}

}