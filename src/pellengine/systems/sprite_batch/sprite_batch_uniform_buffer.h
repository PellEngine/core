#ifndef _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_UNIFORM_BUFFER_H_
#define _PELLENGINE_SYSTEMS_SPRITE_BATCH_SPRITE_BATCH_UNIFORM_BUFFER_H_

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace pellengine {

struct SpriteBatchUniformBufferObject {
  glm::mat4 proj;
};

}

#endif