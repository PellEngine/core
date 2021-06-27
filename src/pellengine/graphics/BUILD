package(
  default_visibility = ["//visibility:public"],
)

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "graphics",
  srcs = [
    "buffer.cpp",
    "command_buffer.cpp",
    "graphics_pipeline.cpp",
    "instance.cpp",
    "renderer.cpp",
    "swapchain_recreator.cpp",
    "window.cpp",
  ],
  hdrs = [
    "buffer.h",
    "command_buffer.h",
    "graphics_pipeline.h",
    "instance.h",
    "renderer.h",
    "swapchain_recreator.h",
    "window.h",
  ],
  deps = [
    "//src/pellengine/vulkan:vulkan",
    "//src/pellengine/io:io",
  ]
)