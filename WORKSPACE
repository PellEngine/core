load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

http_archive(
  name = "com_grail_bazel_compdb",
  strip_prefix = "bazel-compilation-database-master",
  urls = ["https://github.com/grailbio/bazel-compilation-database/archive/master.tar.gz"],
)

git_repository(
  name = "rules_vulkan",
  remote = "https://github.com/PellEngine/rules_vulkan.git",
  tag = "v0.0.3",
)

new_git_repository(
  name = "ctti",
  remote = "https://github.com/PellEngine/ctti.git",
  build_file = "@//src/third_party:BUILD.ctti",
  branch = "master"
)

new_git_repository(
  name = "glm",
  remote = "https://github.com/PellEngine/glm.git",
  build_file = "@//src/third_party:BUILD.glm",
  branch = "master"
)

new_git_repository(
  name = "stb",
  remote = "https://github.com/PellEngine/stb.git",
  build_file = "@//src/third_party:BUILD.stb",
  branch = "master"
)

load("@rules_vulkan//vulkan:repositories.bzl", "vulkan_repositories")
vulkan_repositories()