load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@com_grail_bazel_compdb//:aspects.bzl", "compilation_database")

cc_library(
  name = "pellengine",
  srcs = [
    "//src/pellengine/graphics:libgraphics",
    "//src/pellengine/io:libio",
    "//src/pellengine/systems:libsystems",
    #"//src/pellengine/io:io",
    #"//src/pellengine/systems:sprite_batch",
    #"//src/pellengine/components:components",
    #"//src/pellengine/ecs:ecs",
  ]
)

#filegroup(
 # name = "libpellengine",
  #srcs = [":pellengine"],
  #output_group = "dynamic_library"
#)
#
#genrule(
#  name = "readelf_pellengine",
#  srcs = [":libpellengine"],
#  outs = ["readelf_pellengine.txt"],
#  cmd = "readelf -a $(SRCS) > $@",
#)

compilation_database(
  name = "pellengine_compdb",
  targets = [
    "//src/pellengine:libpellengine.so"
  ]
)