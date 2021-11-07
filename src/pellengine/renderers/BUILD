package(
  default_visibility = ["//visibility:public"],
)

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_binary(
  name = "librenderers.so",
  linkshared = True,
  srcs = [
    "renderer2D/renderer2D_command_buffer.cpp",
    "renderer2D/renderer2D_command_buffer.h",
    "renderer2D/renderer2D.cpp",
    "renderer2D/renderer2D.h"
  ],
  deps = [
    "//src/pellengine/graphics:graphics",
    "//src/pellengine/vulkan:vulkan",
    "//src/pellengine/io:io",
    "@stb//:stb",
    "@glm//:glm",
  ],
)

cc_library(
  name = "renderers",
  srcs = ["librenderers.so"],
  hdrs = [
    "renderer2D/renderer2D_command_buffer.h",
    "renderer2D/renderer2D.h"
  ],
)