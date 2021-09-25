package(
  default_visibility = ["//visibility:public"],
)

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_binary(
  name = "libsprite_batch.so",
  linkshared = True,
  srcs = [
    "sprite_batch/sprite_batch_command_buffer.cpp",
    "sprite_batch/sprite_batch_command_buffer.h",
    "sprite_batch/sprite_batch_layer.cpp",
    "sprite_batch/sprite_batch_layer.h",
    "sprite_batch/sprite_batch_uniform_buffer.cpp",
    "sprite_batch/sprite_batch_uniform_buffer.h",
    "sprite_batch/sprite_batch.cpp",
    "sprite_batch/sprite_batch.h",
  ],
  deps = [
    "//src/pellengine/graphics:graphics",
    "//src/pellengine/vulkan:vulkan",
    "//src/pellengine/components:components",
    "//src/pellengine/ecs:ecs",
    "//src/pellengine/io:io",
    "@glm//:glm",
    "@stb//:stb"
  ]
)

cc_library(
  name = "sprite_batch",
  srcs = ["libsprite_batch.so"],
  hdrs = [
    "sprite_batch/sprite_batch_command_buffer.h",
    "sprite_batch/sprite_batch_layer.h",
    "sprite_batch/sprite_batch_uniform_buffer.h",
    "sprite_batch/sprite_batch.h",
  ]
)