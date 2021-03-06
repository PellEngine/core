#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
  mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in uint inTextureID;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out uint fragTextureID;

void main() {
  gl_Position = ubo.proj * vec4(inPosition, 1.0);
  fragColor = inColor;
  fragTexCoord = inTexCoord;
  fragTextureID = inTextureID;
}