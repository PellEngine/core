package(
  default_visibility = ["//visibility:public"],
)

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
  name = "ecs",
  hdrs = [
    "component_array.h",
    "component_manager.h",
    "entity_component_system.h",
    "entity_manager.h",
    "system_manager.h",
    "types.h",
  ],
  deps = [
    "//src/pellengine/graphics:graphics",
    "@ctti//:ctti"
  ]
)