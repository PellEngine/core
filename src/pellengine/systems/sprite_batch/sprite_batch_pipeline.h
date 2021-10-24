#ifndef _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_PIPELINE_H_
#define _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_PIPELINE_H_

#include "src/pellengine/graphics/graphics_pipeline.h"
#include "src/pellengine/graphics/shader.h"
#include "src/pellengine/graphics/descriptor_builder.h"
#include "src/pellengine/graphics/uniform_buffer.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

namespace pellengine {

struct SpriteBatchVertex {
  glm::vec3 pos;
  glm::vec4 color;
  glm::vec2 texCoord;
};

class SpriteBatchPipeline : public GraphicsPipeline {
 public:
  SpriteBatchPipeline(std::shared_ptr<Window> window, std::shared_ptr<UniformBuffer> uniformBuffer);
  ~SpriteBatchPipeline();

  SpriteBatchPipeline(const SpriteBatchPipeline&) = delete;
  SpriteBatchPipeline& operator=(const SpriteBatchPipeline&) = delete;

  void initialize() override;
  void terminate() override;

 private:
  std::shared_ptr<UniformBuffer> uniformBuffer;
  Texture* texture;
};

}

#endif