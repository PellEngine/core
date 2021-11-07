#include "sprite_batch_pipeline.h"

namespace pellengine {

SpriteBatchPipeline::SpriteBatchPipeline(std::shared_ptr<Window> window, std::shared_ptr<UniformBuffer> uniformBuffer, std::shared_ptr<DescriptorAllocator> allocator, uint32_t maxTextures) : GraphicsPipeline(window), uniformBuffer(uniformBuffer), allocator(allocator), maxTextures(maxTextures) {
  for(uint32_t i=0;i<maxTextures;i++) {
    emptyTextures.push_back(new TextureEmpty(window));
  }
}
SpriteBatchPipeline::~SpriteBatchPipeline() {
  terminate();
  for(TextureEmpty* texture : emptyTextures) {
    delete texture;
  }
  emptyTextures.clear();
}

void SpriteBatchPipeline::initialize() {
  if(initialized) return;

  // Initialize empty texture placeholder
  for(TextureEmpty* texture : emptyTextures) {
    texture->initialize();
  }

  // Create shaders
  std::shared_ptr<Shader> vertexShader = std::make_shared<Shader>(window, "shaders/test.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
  std::shared_ptr<Shader> fragmentShader = std::make_shared<Shader>(window, "shaders/test.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
  addShader(vertexShader);
  addShader(fragmentShader);

  // Setup vertex description
  VkVertexInputBindingDescription bindingDescription{};
  bindingDescription.binding = 0;
  bindingDescription.stride = sizeof(SpriteBatchVertex);
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  addBindingDescription(bindingDescription);

  // Position attribute
  addAttributeDescription(
    VkVertexInputAttributeDescription{
      .binding = 0,
      .location = 0,
      .format = VK_FORMAT_R32G32_SFLOAT,
      .offset = offsetof(SpriteBatchVertex, pos)
    }
  );

  // Color attribute
  addAttributeDescription(
    VkVertexInputAttributeDescription{
      .binding = 0,
      .location = 1,
      .format = VK_FORMAT_R32G32B32A32_SFLOAT,
      .offset = offsetof(SpriteBatchVertex, color)
    }
  );

  // Tex coord attribute
  addAttributeDescription(
    VkVertexInputAttributeDescription{
      .binding = 0,
      .location = 2,
      .format = VK_FORMAT_R32G32_SFLOAT,
      .offset = offsetof(SpriteBatchVertex, texCoord)
    }
  );

  // Texture ID attribute
  addAttributeDescription(
    VkVertexInputAttributeDescription{
      .binding = 0,
      .location = 3,
      .format = VK_FORMAT_R32_UINT,
      .offset = offsetof(SpriteBatchVertex, textureID)
    }
  );

  // Setup descriptors
  std::vector<VkDescriptorBufferInfo> bufferInfos;
  std::vector<VkDescriptorImageInfo> imageInfos;
  createBufferInfos(bufferInfos);
  createImageInfos(imageInfos);

  addDescriptorBuilder(
    DescriptorBuilder::begin(window, allocator)
      .bindBuffer(0, bufferInfos.data(), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
      .bindTexture(1, imageInfos.data(), static_cast<uint32_t>(imageInfos.size()), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
  );

  // Setup pipeline configuration
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

  PipelineConfiguration configuration{};
  configuration.inputAssembly = &inputAssembly;
  configuration.viewportState = &viewportState;
  configuration.rasterizer = &rasterizer;
  configuration.multisampling = &multisampling;
  configuration.colorBlending = &colorBlending;

  // Initiaize the pipeline
  createPipeline(configuration);
  initialized = true;
}

void SpriteBatchPipeline::createBufferInfos(std::vector<VkDescriptorBufferInfo>& bufferInfos) {
  bufferInfos.resize(window->getSwapChainImages().size());
  for(size_t i=0;i<window->getSwapChainImages().size();i++) {
    bufferInfos[i] = uniformBuffer->getDescriptorBufferInfo(i);
  }
}

void SpriteBatchPipeline::createImageInfos(std::vector<VkDescriptorImageInfo>& imageInfos) {
  for(uint32_t i=0;i<maxTextures;i++) {
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    
    if(i >= spriteSheets.size()) {
      imageInfo.imageView = emptyTextures[i]->getImageView();
      imageInfo.sampler = emptyTextures[i]->getSampler();
    } else {
      imageInfo.imageView = spriteSheets[i]->getTexture()->getImageView();
      imageInfo.sampler = spriteSheets[i]->getTexture()->getSampler();
    }

    imageInfos.push_back(imageInfo);
  }
}

void SpriteBatchPipeline::addSpriteSheet(std::shared_ptr<SpriteSheet> spriteSheet) {
  spriteSheets.push_back(spriteSheet);
  
  if(initialized) {
    std::vector<VkDescriptorBufferInfo> bufferInfos;
    std::vector<VkDescriptorImageInfo> imageInfos;
    createBufferInfos(bufferInfos);
    createImageInfos(imageInfos);

    DescriptorBuilder::begin(window, allocator)
      .bindBuffer(0, bufferInfos.data(), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
      .bindTexture(1, imageInfos.data(), static_cast<uint32_t>(imageInfos.size()), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      .build(window->getSwapChainImages().size(), getDescriptorSets()[0].data(), getDescriptorSetLayouts()[0]);
  }
}

void SpriteBatchPipeline::terminate() {
  if(!initialized) return;
  terminatePipeline();
  for(TextureEmpty* texture : emptyTextures) {
    texture->terminate();
  }
  initialized = false;
}

}