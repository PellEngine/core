load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@com_grail_bazel_compdb//:aspects.bzl", "compilation_database")

filegroup(
  name = "build_all",
  srcs = [
    "//src/pellengine:core",
    "//src/pellengine/graphics:graphics",
    "//src/pellengine/io:io",
    "//src/pellengine/vulkan:vulkan",
    "//src/pellengine/systems:sprite_batch",
    "//src/pellengine/renderers:renderers"
  ]
)

compilation_database(
  name = "pellengine_compdb",
  targets = [
    "//src/pellengine:core",
    "//src/pellengine/graphics:libgraphics.so",
    "//src/pellengine/io:libio.so",
    "//src/pellengine/vulkan:libpellengine_vulkan.so",
    "//src/pellengine/systems:libsprite_batch.so",
    "//src/pellengine/ecs:ecs",
    "//src/pellengine/renderers:librenderers.so"
  ]
)

cc_library(
  name = "core",
  srcs = [
    "app.ipp",
  ],
  hdrs = [
    "app.h",
    "app.ipp",
  ],
  deps = [
    "//src/pellengine/ecs:ecs",
    "//src/pellengine/graphics:graphics_fwd",
    "//src/pellengine/io:io",
    "@glm//:glm",
    "@ctti//:ctti",
    "@stb//:stb",
  ],
)