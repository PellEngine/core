load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@com_grail_bazel_compdb//:aspects.bzl", "compilation_database")

cc_binary(
  name = "pellengine",
  linkshared = True,
  srcs = [
    "//src/pellengine/graphics:libgraphics",
    "//src/pellengine/io:libio",
    "//src/pellengine/systems:libsystems"
  ]
)

compilation_database(
  name = "pellengine_compdb",
  targets = [
    "//src/pellengine:libpellengine.so"
  ]
)