load("@rules_vulkan//glsl:defs.bzl", "glsl_shader")

glsl_shader(
  name = "test_vert",
  shader = "test.vert",
  visibility = ["//visibility:public"]
)

glsl_shader(
  name = "test_frag",
  shader = "test.frag",
  visibility = ["//visibility:public"]
)

filegroup(
  name = "build_all",
  srcs = [
    ":test_vert",
    ":test_frag"
  ]
)