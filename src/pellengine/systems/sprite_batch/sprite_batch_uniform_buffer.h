#ifndef _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_UNIFORM_BUFFER_H_
#define _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_UNIFORM_BUFFER_H_

#include "src/pellengine/vulkan/vulkan_wrapper.h"
#include "src/pellengine/graphics/uniform_buffer.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace pellengine {

struct SpriteBatchUniformBufferObject {
  glm::mat4 proj;
};

class SpriteBatchUniformBuffer : public UniformBuffer {
 public:
  SpriteBatchUniformBuffer(std::shared_ptr<Window> window, int binding);
  ~SpriteBatchUniformBuffer();

  SpriteBatchUniformBuffer(const SpriteBatchUniformBuffer&) = delete;
  SpriteBatchUniformBuffer& operator=(const SpriteBatchUniformBuffer&) = delete;

  size_t getUniformSize() override {
    return sizeof(SpriteBatchUniformBufferObject);
  }

  void updateUniformBuffer(uint32_t imageIndex);

 protected:
  void createDescriptorSetLayouts() override;
  void createBuffers() override;
};

}

#endif