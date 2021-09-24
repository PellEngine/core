#include "sprite_batch_command_buffer.h"

namespace pellengine {

SpriteBatchCommandBuffer::SpriteBatchCommandBuffer(std::shared_ptr<Window> window, PipelineConfiguration pipelineConfiguration, Buffer* vertexBuffer, Buffer* indexBuffer, uint32_t numIndices, std::shared_ptr<GraphicsPipeline> graphicsPipeline) : 
  CommandBuffer(window, pipelineConfiguration), 
  vertexBuffer(vertexBuffer), 
  indexBuffer(indexBuffer), 
  numIndices(numIndices),
  graphicsPipeline(graphicsPipeline) {}

SpriteBatchCommandBuffer::~SpriteBatchCommandBuffer() {}

void SpriteBatchCommandBuffer::draw(VkCommandBuffer& commandBuffer, uint32_t imageIndex) {
  VkBuffer vertexBuffers[] = {this->vertexBuffer->buffer};
  VkDeviceSize offsets[] = {0};

  vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->getPipelineLayout(), 0, 1, &graphicsPipeline->getDescriptorSets()[0], 0, nullptr);
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
  vkCmdBindIndexBuffer(commandBuffer, indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT16);
  vkCmdDrawIndexed(commandBuffer, numIndices, 1, 0, 0, 0);
}

}