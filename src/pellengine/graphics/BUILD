package(
  default_visibility = ["//visibility:public"],
)

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_binary(
  name = "libgraphics.so",
  linkshared = True,
  srcs = [
    "buffer.cpp",
    "buffer.h",
    "command_buffer.cpp",
    "command_buffer.h",
    "graphics_pipeline.cpp",
    "graphics_pipeline.h",
    "instance.cpp",
    "instance.h",
    "renderer.cpp",
    "renderer.h",
    "swapchain_recreator.cpp",
    "swapchain_recreator.h",
    "window.cpp",
    "window.h",
  ],
  deps = [
    "//src/pellengine/vulkan:vulkan",
    "//src/pellengine/io:io",
    "@ctti//:ctti"
  ]
)

cc_library(
  name = "graphics",
  srcs = ["libgraphics.so"],
  hdrs = [
    "buffer.h",
    "command_buffer.h",
    "graphics_pipeline.h",
    "instance.h",
    "renderer.h",
    "swapchain_recreator.h",
    "window.h",
  ]
)