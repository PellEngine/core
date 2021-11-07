#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSamplers[8];

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in flat uint fragTextureID;

layout(location = 0) out vec4 outColor;

void main() {
  outColor = texture(texSamplers[fragTextureID], fragTexCoord);
}