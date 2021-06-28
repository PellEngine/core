load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@com_grail_bazel_compdb//:aspects.bzl", "compilation_database")

filegroup(
  name = "build_all",
  srcs = [
    "//src/pellengine/graphics:graphics",
    "//src/pellengine/io:io",
    "//src/pellengine/vulkan:vulkan",
    "//src/pellengine/systems:sprite_batch",
  ]
)

compilation_database(
  name = "pellengine_compdb",
  targets = [
    "//src/pellengine:libpellengine.so"
  ]
)