#ifndef _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_PIPELINE_H_
#define _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_PIPELINE_H_

#include "src/pellengine/graphics/graphics_pipeline.h"
#include "src/pellengine/graphics/shader.h"
#include "src/pellengine/graphics/descriptor_builder.h"
#include "src/pellengine/graphics/uniform_buffer.h"
#include "src/pellengine/graphics/sprite_sheet.h"
#include "src/pellengine/graphics/texture_empty.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

namespace pellengine {

struct SpriteBatchVertex {
  glm::vec3 pos;
  glm::vec4 color;
  glm::vec2 texCoord;
  uint16_t textureID;
};

class SpriteBatchPipeline : public GraphicsPipeline {
 public:
  SpriteBatchPipeline(std::shared_ptr<Window> window, std::shared_ptr<UniformBuffer> uniformBuffer, std::shared_ptr<DescriptorAllocator> allocator, uint32_t maxTextures);
  ~SpriteBatchPipeline();

  SpriteBatchPipeline(const SpriteBatchPipeline&) = delete;
  SpriteBatchPipeline& operator=(const SpriteBatchPipeline&) = delete;

  void initialize() override;
  void terminate() override;

  void addSpriteSheet(std::shared_ptr<SpriteSheet> spriteSheet);

  std::vector<std::shared_ptr<SpriteSheet>>& getSpriteSheets() {
    return spriteSheets;
  }

 private:
  std::shared_ptr<DescriptorAllocator> allocator;
  std::shared_ptr<UniformBuffer> uniformBuffer;
  std::vector<std::shared_ptr<SpriteSheet>> spriteSheets;
  std::vector<TextureEmpty*> emptyTextures;

  uint32_t maxTextures;

  void createBufferInfos(std::vector<VkDescriptorBufferInfo>& bufferInfos);
  void createImageInfos(std::vector<VkDescriptorImageInfo>& imageInfos);
};

}

#endif