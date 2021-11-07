package(
  default_visibility = ["//visibility:public"],
)

load("@rules_cc//cc:defs.bzl", "cc_library")

hdrs = [
  "buffer.h",
  "clear_command_buffer.h",
  "command_buffer.h",
  "descriptor_allocator.h",
  "descriptor_builder.h",
  "graphics_pipeline.h",
  "image.h",
  "instance_android.h",
  "instance.h",
  "renderer.h",
  "shader.h",
  "sprite_sheet.h",
  "swapchain_recreator.h",
  "texture_empty.h",
  "texture.h",
  "uniform_buffer.h",
  "window.h",
]

srcs = [
  "buffer.cpp",
  "clear_command_buffer.cpp",
  "command_buffer.cpp",
  "descriptor_allocator.cpp",
  "descriptor_builder.cpp",
  "graphics_pipeline.cpp",
  "image.cpp",
  "instance_android.cpp",
  "instance.cpp",
  "renderer.cpp",
  "shader.cpp",
  "sprite_sheet.cpp",
  "swapchain_recreator.cpp",
  "texture_empty.cpp",
  "texture.cpp",
  "uniform_buffer.cpp",
  "window.cpp",
]

cc_binary(
  name = "libgraphics.so",
  linkshared = True,
  srcs = srcs + hdrs,
  deps = [
    "//src/pellengine/vulkan:vulkan",
    "//src/pellengine/io:io",
    "@glm//:glm",
    "@ctti//:ctti",
    "@stb//:stb",
  ]
)

cc_library(
  name = "graphics",
  srcs = ["libgraphics.so"],
  hdrs = hdrs,
)

cc_library(
  name = "graphics_fwd",
  hdrs = hdrs
)