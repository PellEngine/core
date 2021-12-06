load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "glm",
  hdrs = glob(["glm/**"]),
  include_prefix = "glm",
  strip_include_prefix = "glm",
  includes = ["glm"],
  copts = ["-Iglm/"],
  visibility = ["//visibility:public"]
)