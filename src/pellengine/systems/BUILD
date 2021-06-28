package(
  default_visibility = ["//visibility:public"],
)

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "sprite_batch",
  srcs = [
    "sprite_batch/sprite_batch_command_buffer.cpp",
    "sprite_batch/sprite_batch_layer.cpp",
    "sprite_batch/sprite_batch.cpp",
  ],
  hdrs = [
    "sprite_batch/sprite_batch_command_buffer.h",
    "sprite_batch/sprite_batch_layer.h",
    "sprite_batch/sprite_batch.h",
  ],
  deps = [
    "//src/pellengine/graphics:graphics",
    "//src/pellengine/vulkan:vulkan",
    "//src/pellengine/components:components",
    "//src/pellengine/ecs:ecs",
  ]
)

filegroup(
  name = "libsystems",
  srcs = [
    ":sprite_batch",
  ],
  output_group = "dynamic_library",
)

genrule(
  name = "readelf_graphics",
  srcs = [":libsystems"],
  outs = ["reafelf_systems.txt"],
  cmd = "readelf -a $(SRCS) > $@",
)