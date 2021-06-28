rm ./compile_commands.json
bazel build //src/pellengine:pellengine_compdb --config=android_arm64_config
outfile="$(bazel info bazel-bin --config=android_arm64_config )/src/pellengine/compile_commands.json"

execroot="core"
sed -i.bak "s@__EXEC_ROOT__@${execroot}@" "${outfile}"

cp $outfile ./compile_commands.json