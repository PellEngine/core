#include "sprite_batch_command_buffer.h"

namespace pellengine {

SpriteBatchCommandBuffer::SpriteBatchCommandBuffer(std::shared_ptr<Window> window, PipelineConfiguration pipelineConfiguration, Buffer* vertexBuffer, Buffer* indexBuffer, uint32_t numIndices) : CommandBuffer(window, pipelineConfiguration), numIndices(numIndices), vertexBuffer(vertexBuffer), indexBuffer(indexBuffer) {}
SpriteBatchCommandBuffer::~SpriteBatchCommandBuffer() {}

void SpriteBatchCommandBuffer::draw(VkCommandBuffer& buffer) {
  VkBuffer vertexBuffers[] = {this->vertexBuffer->buffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(buffer, 0, 1, vertexBuffers, offsets);
  vkCmdBindIndexBuffer(buffer, indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT16);
  vkCmdDrawIndexed(buffer, numIndices, 1, 0, 0, 0);
}

}