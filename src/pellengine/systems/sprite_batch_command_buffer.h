#ifndef _PELLENGINE_SYSTEMS_SPRITE_BATCH_COMMAND_BUFFER_H_
#define _PELLENGINE_SYSTEMS_SPRITE_BATCH_COMMAND_BUFFER_H_

#include <pellengine/graphics/window.h>
#include <pellengine/graphics/command_buffer.h>
#include <pellengine/graphics/buffer.h>
#include <memory>

namespace pellengine {

class SpriteBatchCommandBuffer : public CommandBuffer {
 public:
  SpriteBatchCommandBuffer(std::shared_ptr<Window> window, PipelineConfiguration pipelineConfiguration, Buffer* vertexBuffer, Buffer* indexBuffer, uint32_t numIndices);
  ~SpriteBatchCommandBuffer();

  SpriteBatchCommandBuffer(const SpriteBatchCommandBuffer&) = delete;
  SpriteBatchCommandBuffer& operator=(const SpriteBatchCommandBuffer&) = delete;

 protected:
  void draw(VkCommandBuffer& buffer) override;

 private:
  uint32_t numIndices;
  Buffer* vertexBuffer;
  Buffer* indexBuffer;
};

}

#endif