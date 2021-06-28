#ifndef _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_COMMAND_BUFFER_H_
#define _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_COMMAND_BUFFER_H_

#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include "src/pellengine/graphics/command_buffer.h"
#include "src/pellengine/graphics/window.h"
#include "src/pellengine/graphics/buffer.h"
#include <memory>

namespace pellengine {

class SpriteBatchCommandBuffer : public CommandBuffer {
 public:
  SpriteBatchCommandBuffer(std::shared_ptr<Window> window, PipelineConfiguration pipelineConfiguration, Buffer* vertexBuffer, Buffer* indexBuffer, uint32_t numIndices);  
  ~SpriteBatchCommandBuffer();

  SpriteBatchCommandBuffer(const SpriteBatchCommandBuffer&) = delete;
  SpriteBatchCommandBuffer& operator=(const SpriteBatchCommandBuffer&) = delete;

 protected:
  void draw(VkCommandBuffer& commandBuffer) override;

 private:
  uint32_t numIndices;
  Buffer* vertexBuffer;
  Buffer* indexBuffer;
};

}

#endif