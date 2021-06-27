load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
  name = "libpellengine.so",
  linkshared = 1,
  linkstatic = 1,
  deps = [
    "//src/pellengine/graphics:graphics",
    "//src/pellengine/io:io",
  ]
)